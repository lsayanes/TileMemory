
#pragma once

#define _CONSOLE_VERBOSE

#include <stdint.h>
#include <mutex>
#include <chrono>

#include <Image.h>
#include <EntityManager.h>

#include <Render.h>

#include "Tiles.h"
#include "Score.h"


class Game
{
public:
	static constexpr uint8_t maxLines{ 16 };
	static constexpr uint8_t playLine{ 2 };

private:
	Score		&score;
	bool		bPlaying;
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
	bool	timeOut(uint32_t milliseconds);
	uint8_t	checkInput();
	void	update();
	void	onTouch(int x, int y);
	void	mask();
	void	unmask();
	void	playLineRestore();
	void	doDown();
	void	updateGround();

	inline bool elapsed(std::chrono::steady_clock::time_point start, int milliseconds)
	{
		auto now = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
		return elapsed.count() >= milliseconds;
	}

public:
	explicit Game(sys::Render& render, Score &score);
	virtual ~Game();

	void play();

#if defined(_CONSOLE_VERBOSE)
	void printBoard() const;
	void printMembers() const;
#endif

};