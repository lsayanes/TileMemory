
#include <cstdlib>
#include <ctime>
#include <bitset>
#include <inttypes.h>

#include <memory>
#include <algorithm>
#include <thread>
#include <chrono>

#include "Game.h"

#if defined(_CONSOLE_VERBOSE)
#include <iostream>
#endif

Game::Game(sys::Render& rndr, Score& scr) :
	score{ scr },
	bPlaying{ false },
	lastAddedline{ 0 },
	ground{ maxLines },
	lines{ },
	guess{ Tile::Status::guess },
	updateMutex{ },
	render{ rndr },
	offImg{ rndr.loadPng("./Img/cyan.png") }, onImg{ rndr.loadPng("./Img/yellow.png") }, guessImg{ rndr.loadPng("./Img/guess.png") }, 
	deadImg{ rndr.loadPng("./Img/purple.png") }, gameOverImg{ rndr.loadPng("./Img/gameover.png") },
	entities{ },
	touchOrder{ 0 }, touches{ 0 },
	scoreFont{ std::make_unique<format::Font>(std::string("./fnts/WhiteOnBlack.ttf"), (float)32.0f)}
{
	srand(static_cast<unsigned int>(time(nullptr)));

	render.setTouch(std::bind(&Game::onTouch, this, std::placeholders::_1, std::placeholders::_2));

	for (auto l = 0; l < maxLines; l++)
	{
		for (uint8_t t = 0; t < Tiles::maxTiles; t++)
		{
			auto& tile = entities.add(offImg);
			tile.x = t * offImg.w;
			tile.y = l * offImg.h;
		}
	}

	scoreFont.x = 5;
	scoreFont.y = render.h() - 40;
	scoreFont.data->r = 0xFF;
	scoreFont.data->g = 0xFF;
	scoreFont.data->b = 0xFF;
}

Game::~Game()
{
	render.deletePng(offImg);
	render.deletePng(onImg);
	render.deletePng(guessImg);
	render.deletePng(deadImg);
	render.deletePng(gameOverImg);
}

void Game::play()
{

	uint8_t tileCnt = 0;

	bPlaying = true;

	std::thread updateThread(&Game::update, this);

	while (bPlaying)
	{
		newLine();
		tileCnt++;
		bPlaying = timeOut(500);

		if (bPlaying && tileCnt > score.level)
		{
			while (bPlaying && playLine != lastAddedline)
			{
				move();
				bPlaying = timeOut(500);
			}

			if (bPlaying)
			{
				mask();
				touches = 0;
				auto touchesTime = std::chrono::steady_clock::now();

				while (bPlaying && touches < (score.level + 1) && !elapsed(touchesTime, (2000 * (score.level + 1))))
					bPlaying = timeOut(500);

				if (bPlaying && (bPlaying = timeOut(200)))
				{
					auto err = checkInput();
					score.updateScore(err);
					playLineRestore();
					doDown();

					if (bPlaying)
					{
						updateGround();
						if (ground <= playLine)
						{
							std::cout << "GAME OVER" << std::endl;
							
							auto& it = entities.add(gameOverImg);
							it.x = 0;
							it.y = 260;

							while(render.doEvent())
								;
							bPlaying = false;
						}
					}

					if (bPlaying)
					{
						unmask();
						tileCnt = 0;
						lastAddedline = 0;
					}
				}
			}
		}
	}

	std::cout << "join " << std::endl;
	updateThread.join();
}

void Game::newLine()
{
	std::lock_guard<std::mutex> lock(updateMutex);
	lines->newTile();
}

void Game::move()
{
	std::lock_guard<std::mutex> lock(updateMutex);

	auto i = lastAddedline + 1;
	for (; i != 0; i--)
		lines[i] = lines[i - 1];

	lastAddedline++;

	*lines = 0;
}


bool Game::timeOut(uint32_t milliseconds)
{
	const uint32_t ms = 10;
	uint32_t cnt = 0;

	while (cnt < milliseconds)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
		cnt += ms;
		if (!render.doEvent())
			return false;
	}

	return true;
}


void Game::update()
{
	auto updateTime = std::chrono::steady_clock::now();

	while (bPlaying)
	{
		size_t imgCnt = 0;

		for (auto l = 0; l < maxLines; l++)
		{
			for (uint8_t t = 0; t < Tiles::maxTiles; t++)
			{				
				std::lock_guard<std::mutex> lock(updateMutex);
				auto& tile = lines[l].tiles[t];
				//auto pData = entities[imgCnt].data.get()->pData;

				if (Tile::Status::on == tile.status)
					entities[imgCnt].data->pData = onImg.pData;
				else if (Tile::Status::off == tile.status)
					entities[imgCnt].data->pData = offImg.pData;
				else if (Tile::Status::guess == tile.status)
					entities[imgCnt].data->pData = guessImg.pData;
				else if (Tile::Status::dead == tile.status)
					entities[imgCnt].data->pData = deadImg.pData;

				imgCnt++;
			}
		}
		
		if (elapsed(updateTime, 5))
		{
			render.setBackground();
			render.draw(entities);
			//score.draw();
			render.drawText(scoreFont, "Level: %u Score: %" PRId64, score.level + 1, score.currScore);

			render.flip();

			updateTime = std::chrono::steady_clock::now();
		}
	}
}

void Game::onTouch(int x, int y)
{
	auto l = (y / offImg.h);
	//int index = ((y / offImg.h) * (render.w() / offImg.w)) + (x / offImg.w);

	if (l == playLine)
	{
		auto t = (x / offImg.w);
		std::lock_guard<std::mutex> lock(updateMutex);
		auto& tile = lines[l].tiles[t];
		tile.status = tile.status == Tile::Status::guess ? Tile::Status::on : tile.status;
		tile.onOrder = touchOrder + 1;
		touchOrder++;
		touches++;

		std::cout << "onTouch order" << static_cast<int>(touchOrder) << " touches:" << static_cast<int>(touches) << std::endl;
	}
}

void Game::mask()
{
	std::lock_guard<std::mutex> lock(updateMutex);
	touchOrder = 0;

	for (auto i = 0; i < Tiles::maxTiles; i++)
	{
		auto& tile = lines[playLine].tiles[i];
		guess[i].status = Tile::Status::off == tile.status? Tile::Status::guess : Tile::Status::on;
		guess[i].onOrder = tile.onOrder;
		tile.status = Tile::Status::guess;
		tile.onOrder = 0;
	}
}

void Game::unmask()
{
	std::lock_guard<std::mutex> lock(updateMutex);
	for (auto i = 0; i < Tiles::maxTiles; i++)
		lines[playLine].tiles[i].status = Tile::Status::off;
}

void Game::playLineRestore()
{
	std::lock_guard<std::mutex> lock(updateMutex);
	for (auto i = 0; i < Tiles::maxTiles; i++)
	{
		auto& tile = lines[playLine].tiles[i];
		tile.status = (Tile::Status::dead == guess[i].status /* || Tile::Status::bad == guess[i].status*/) ? Tile::Status::on : Tile::Status::off;
		tile.onOrder = guess[i].onOrder;
	}
}

void Game::doDown()
{
	constexpr uint8_t limit = maxLines - 1;

	uint8_t downline = playLine;

	while (downline < limit && bPlaying)
	{
		for (auto i = 0; i < Tiles::maxTiles; i++)
		{
			std::lock_guard<std::mutex> lock(updateMutex);

			auto& tile = lines[downline].tiles[i];
			auto& botton = lines[downline + 1].tiles[i];

			if (Tile::Status::on == tile.status)
			{
				if (Tile::Status::dead != botton.status)
				{
					tile.status = Tile::Status::off;
					botton.status = Tile::Status::on;
				}
			}
		}

		downline++;
		bPlaying = timeOut(25);
	}
}

void Game::updateGround()
{
	uint8_t line = maxLines - 1;
	std::lock_guard<std::mutex> lock(updateMutex);

	uint8_t firstDead = line;
	while (line >= playLine)
	{
		for (auto i = 0; i < Tiles::maxTiles; i++)
		{
			if (Tile::Status::on == lines[line].tiles[i].status)
			{
				lines[line].tiles[i].status = Tile::Status::dead;
				if (line < firstDead)
					firstDead = line;
			}
		}

		line--;
	}

	ground = firstDead - 1;
}


uint8_t Game::checkInput()
{
	uint8_t ret = 0;
	std::lock_guard<std::mutex> lock(updateMutex);
	for (auto i = 0; i < Tiles::maxTiles; i++)
	{
		auto &tile = lines[playLine].tiles[i];
		auto& gss = guess[i];

		if (tile.status == gss.status)
		{
			if (tile.onOrder != gss.onOrder)
			{
				gss.status = Tile::Status::dead;
				ret++;
			}
		}
		else
		{
			gss.status = Tile::Status::dead;
			ret++;
		}
	}

	return ret;
}
