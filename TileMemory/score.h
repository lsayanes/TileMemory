#pragma once

#include <stdint.h>


struct Score
{
	static constexpr uint8_t maxLevels{ 8 };

	static constexpr uint8_t stages[maxLevels]{4, 8, 8*2, 16*2, 16*4, 16*8, 32*8, 64*8};
	static constexpr uint8_t levelsPoint[maxLevels]{ 1, 2, 2, 4, 4, 8, 8, 16 };

	uint64_t	maxScore{ 0 };
	uint8_t		level;

	inline void updateScore(uint8_t err = 0)
	{
		maxScore += ((uint64_t)1 << (err > level ? 0 : levelsPoint[maxLevels] - err));
		if (level < maxLevels)
		{
			if (maxScore > stages[level])
			{
				level++;
			}
		}
#if defined(_CONSOLE_VERBOSE)
		std::cout << "level: " << static_cast<int>(level) << " score: " << score << " stages: " << static_cast<int>(stages) << " ground: " << static_cast<int>(ground) << std::endl;
#endif
	}
};
