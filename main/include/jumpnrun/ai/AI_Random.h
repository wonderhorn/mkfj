#pragma once
#include"jumpnrun/ai/AI.h"
#include"jumpnrun/ai/FeatureExtractor.h"
#include<string>
typedef std::vector<double> Feature;

namespace jnr
{
	namespace ai
	{
		//enum eAction{ Walk, Dash, Turn, Jump, JumpWalk, Attack, Cast, Keep, Wait };
		//class AI;
		class AI_Random : public AI
		{
		public:
			AI_Random();

			~AI_Random();
			void initialize();
			Feature processMyStatus(CharacterStatus cs);
			void think(const CharacterStatus& cs);
		private:
			Feature f_prev;
			AICommand command;
		};
	};
};