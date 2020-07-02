#pragma once
#include"jumpnrun/ai/AI.h"
#include"jumpnrun/ai/FeatureExtractor.h"
#include<string>
typedef std::vector<double> Feature;
namespace jnr
{
	namespace ai
	{
		class AI_Handmade : public AI
		{
		public:
			AI_Handmade();

			~AI_Handmade();
			void initialize();
			Feature processMyStatus(CharacterStatus cs);
			void think(const CharacterStatus& cs);
		private:
			Feature f_prev;
			AICommand command;
		};
	};
};