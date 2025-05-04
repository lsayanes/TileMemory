

#include <string>
#include <algorithm>
#include "Score.h"


void Score::updateScore(uint8_t err)
{
	currScore += (err > level ? 0 : levelsPoint[level] - err);
	if (level < maxLevels && currScore > stages[level])
		level++;

	std::cout << "score :" << currScore << std::endl;
}




