#pragma once

#include <stdint.h>
#include <ctime>
#include <cstdlib>

struct Tile
{
	enum Status {off, on, guess, dead };

	uint8_t onOrder;
	Status  status;

	Tile() : onOrder{ 0 }, status{ off } { }

	Tile(Status st)
		: Tile()
	{
		status = st;
	}


	Tile(const Tile& o)
	{
		onOrder = o.onOrder;
		status = o.status;

	}

	Tile& operator=(const Tile& o)
	{
		onOrder = o.onOrder;
		status = o.status;


		return *this;
	}

	~Tile() = default;

};

struct Tiles
{
	static constexpr int maxTiles = 8;

	Tile	tiles[maxTiles]{ };
	uint8_t weight{ 0 };

	Tiles()
	{
		srand(static_cast<unsigned int>(time(nullptr)));
	}

	Tiles& operator=(const Tiles& o)
	{
		for (int i = 0; i < maxTiles; i++)
			tiles[i] = o.tiles[i];

		weight = o.weight;

		return *this;
	}

	Tiles& operator=(uint8_t val)
	{
		for (int i = 0; i < maxTiles; i++)
		{
			tiles[i].status = static_cast<Tile::Status>(val);
			tiles[i].onOrder = val;
		}

		weight = 0;

		return *this;
	}

	~Tiles() = default;


	void newTile()
	{
		uint8_t i = 0;
		do
			i = rand() % maxTiles;
		while (Tile::Status::on == tiles[i].status);

		tiles[i].status = Tile::Status::on;
		tiles[i].onOrder = weight + 1;
		weight++;
	}
};