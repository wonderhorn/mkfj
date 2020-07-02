#pragma once
#include"jumpnrun/ai/AI.h"
#include"jumpnrun/mover/character/Character.h"
#include<vector>
namespace jnr
{
	namespace ai
	{
		class AITrainer
		{
		public:
			AITrainer();
			double predict(const Feature& f, char command, int delay);
			void push(const Feature& f, char command, double score);
			double kernel(const Feature& f1, const Feature& f2);

		private:
			std::vector<Feature> features;
			std::vector<char> commands;
			std::vector<double> scores;
		};

	};
};