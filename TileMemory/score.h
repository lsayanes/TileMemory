#pragma once

#include <stdint.h>
#include <Render.h>

#include <Image.h>
#include <vector>


class Score
{
private:
	sys::Render& render;

	std::vector<format::Image*> numbers;
	
public:

	static constexpr uint8_t maxLevels{ 8 };

	static constexpr uint32_t stages[maxLevels]{4, 8, 16, 128, 512, 1024, 4096, 8192};
	static constexpr uint8_t levelsPoint[maxLevels]{ 1, 2, 2, 4, 4, 8, 8, 16 };

	uint64_t	currScore{ 1234567890 };
	uint8_t		level{ 0 };


	explicit Score(sys::Render& rndr);
	~Score();

	void updateScore(uint8_t err = 0);
	void draw();
};
