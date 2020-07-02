#pragma once
#include"jumpnrun/ai/AISuperviser.h"
#include<string>
#include<vector>
#include<map>
namespace jnr
{
	namespace ai
	{
		class AIScorePredictor
		{
		public:
			AIScorePredictor(){}

			std::string name;
			virtual std::vector<double> calcScores() = 0;
			virtual void train(const std::vector<std::vector<AISuperviser::DataElement>>) = 0;
		};

		class AveragePredictor
		{
		public:
			std::vector<double> calcScores();
			void train(const std::vector<std::vector<AISuperviser::DataElement>>){}
		};
	};
};