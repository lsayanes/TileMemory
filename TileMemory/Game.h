
#pragma once

#define _CONSOLE_VERBOSE

#include <stdint.h>
#include <mutex>
#include <chrono>

#include <Image.h>
#include <EntityManager.h>

#include <Render.h>

#include "Tiles.h"


class Game
{
public:
	static constexpr uint8_t maxLines{ 16 };
	static constexpr uint8_t maxLevels{ 8 };
	static constexpr uint8_t playLine{ 2 };

private:
	uint64_t	score;
	uint64_t	stages;
	bool		bPlaying;
	uint8_t		level;
	uint8_t		lastAddedline;
	uint8_t		ground;
	Tiles		lines[maxLines];
	Tile		guess[Tiles::maxTiles];

	mutable std::mutex updateMutex;

	sys::Render& render;

	format::Image	offImg;
	format::Image	onImg;
	format::Image	guessImg;
	format::Image	deadImg;

	ecs::EntityManager	entities;

	uint8_t touchOrder;
	uint8_t touches;


private:
	void	newLine();
	void	move();
	void	leveUp();
	bool	timeOut(uint32_t milliseconds);
	uint8_t	checkInput();
	void	update();
	void	onTouch(int x, int y);
	void	mask();
	void	unmask();
	void	playLineRestore();
	void	doDown();
	void	updateGround();
	
	inline void updateScore(uint8_t err = 0) 
	{
		score += ((uint64_t)1 << (err>level?0:level - err));
		if (level < 8)
		{
			stages++;
			if (0 == (stages % 8))
				level++;
		}
#if defined(_CONSOLE_VERBOSE)
		std::cout << "level: " << static_cast<int>(level) << " score: " << score << " stages: " << static_cast<int>(stages) << " ground: " << static_cast<int>(ground) << std::endl;
#endif
	}

	inline bool elapsed(std::chrono::steady_clock::time_point start, int milliseconds)
	{
		auto now = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
		return elapsed.count() >= milliseconds;
	}

public:
	explicit Game(sys::Render& render);
	virtual ~Game();

	void play();

#if defined(_CONSOLE_VERBOSE)
	void printBoard() const;
	void printMembers() const;
#endif

};