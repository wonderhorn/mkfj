#pragma once
#include"jumpnrun/ai/AI.h"
#include<vector>

typedef std::pair<double, jnr::ai::AIParameters> ai_and_fitness;
namespace jnr
{

	namespace ai
	{
		bool operator == (const AIParameters& a1, const AIParameters& a2);
		bool operator > (const AIParameters& a1, const AIParameters& a2);
		bool operator < (const AIParameters& a1, const AIParameters& a2);

		class AIOptimizer
		{
		public:
			AIOptimizer();
			std::vector<ai_and_fitness> test();
			std::vector<AIParameters> generate();

			int group_size;
		private:
			std::vector<AIParameters> group;
			std::vector<ai_and_fitness> fitness, old_group;
		};
	};
};