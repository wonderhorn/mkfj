#include"jumpnrun/ai/AIOptimizer.h"
#include"jumpnrun/ai/AITestScreen.h"
#include"jumpnrun/system/Parmanent.h"
#include"utils/MyCppUtil.h"
#include<algorithm>
#include<functional>
#include<limits.h>
#include<iostream>
using namespace jnr;
using namespace ai;

AIOptimizer::AIOptimizer()
{
	group_size = 3;
	for (int i = 0; i < group_size; i++)
	{
		group.push_back(AIParameters::random());
	}
}

std::vector<ai_and_fitness> AIOptimizer::test()
{
	std::vector<ai_and_fitness> rtn;
	int counter = 0;
	for (auto i = group.begin(); i != group.end(); i++)
	{
		double f = 0.0;
		for (int j = 0; j < 1; j++)
		{
			AITestScreen aitest(NULL);
			aitest.initialize(group[counter]);
			gfw::Screen::eScreenState state = gfw::Screen::eScreenState::Continuing;

			while (state == gfw::Screen::eScreenState::Continuing)
			{
				state = aitest.run($P.media);
			}
			f += aitest.ai_fitness;
		}
		std::cout << "ai " << counter << " fitness: " << f << std::endl;
		rtn.push_back(ai_and_fitness(f, *i));
		counter++;
	}
	std::sort(rtn.begin(), rtn.end(), std::greater<ai_and_fitness>());
	return rtn;
}

std::vector<AIParameters> AIOptimizer::generate()
{
	std::vector<AIParameters> rtn;
	for (int i = 0; i < group_size && i < fitness.size(); i++)
	{
		old_group.push_back(fitness[i]);
		rtn.push_back(AIParameters::random());
	}
	std::sort(old_group.begin(), old_group.end(), std::greater<ai_and_fitness>());
	old_group = myutil::slice(old_group, 0, group_size);
	group = rtn;
	std::cout << "max: " << old_group[0].first << std::endl;
	return rtn;
}


bool jnr::ai::operator == (const AIParameters& a1, const AIParameters& a2)
{
	return true;
}
bool jnr::ai::operator > (const AIParameters& a1, const AIParameters& a2)
{
	return false;
}
bool jnr::ai::operator < (const AIParameters& a1, const AIParameters& a2)
{
	return false;
}