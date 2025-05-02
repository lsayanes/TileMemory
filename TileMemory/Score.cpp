

#include <string>
#include <algorithm>
#include "Score.h"


Score::Score(sys::Render& rndr): 
	render{ rndr }, scoreBckImg{ rndr.loadPng("./Img/score.png") }, numbers{ }
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

	render.deletePng(scoreBckImg);
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
	ecs::EntityManager nums{ };

	auto& bck = nums.add(scoreBckImg);

	bck.x = 166;
	bck.y = render.h() - 70;

	std::string score = std::to_string(currScore);
	const char* szScore = score.c_str();
	size_t len = strlen(szScore);

	for (auto i = 0; i < len; i++)
	{
		format::Image* index = numbers[szScore[i] - '0'];

		auto& it = nums.add(*index);
		auto *pbck = &nums[0];
		it.x = pbck->x + 88 + ((i + 1) * it.data->w);
		it.y = pbck->y + 8;
	}

	render.draw(nums);
}

