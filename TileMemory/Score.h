#pragma once

#include <stdint.h>
#include <Render.h>

#include <Image.h>
#include <vector>


struct Score
{	
	static constexpr uint8_t maxLevels{ 8 };

	static constexpr uint32_t stages[maxLevels]{4, 8, 16, 128, 512, 1024, 4096, 8192};
	static constexpr uint8_t levelsPoint[maxLevels]{ 1, 2, 2, 4, 4, 8, 8, 16 };

	uint64_t	currScore{ 0 };
	uint8_t		level{ 0 };

	inline explicit Score() = default;
	virtual ~Score() = default;

	void updateScore(uint8_t err = 0);
};
