

#include <string>
#include <algorithm>
#include "Score.h"


Score::Score(sys::Render& rndr): 
	render{ rndr }, numbers{ }
{
	std::string path;
	for(int i = 0; i < 10; i++)
	{
		path = "./Numbers/" + std::to_string(i) + ".png";
		numbers.push_back(new format::Image{ rndr.loadPng(path.c_str()) });
	}
}

Score::~Score()
{
	std::for_each(numbers.begin(), numbers.end(), [&](auto* it)
		{
			delete it;
		});

	numbers.clear();
}

void Score::updateScore(uint8_t err)
{

	currScore += (err > level ? 0 : levelsPoint[level] - err);
	if (level < maxLevels && currScore > stages[level])
		level++;

	std::cout << "score :" << currScore << std::endl;
}


void Score::draw()
{
	constexpr int32_t margin = 395;

	ecs::EntityManager nums{ };

	std::string score = std::to_string(currScore);
	const char* szScore = score.c_str();
	auto len = strlen(szScore);

	for (auto i = 0; i < len; i++)
	{
		format::Image* index = numbers[szScore[i] - '0'];

		auto& it = nums.add(*index);
		it.x = (margin - index->w) - static_cast<int32_t>(((len - (i + 1)) * index->w));
		it.y = render.h() - 60;
	}

	render.draw(nums);
}

