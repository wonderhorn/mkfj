#pragma once
#include"jumpnrun/ai/AI.h"
#include<vector>
#include<string>
#include<map>
#include"jumpnrun/ai/TemplateHistogram.h"
namespace jnr
{
	namespace ai
	{
		class AI_Supervised: public AI
		{
		public:
			AI_Supervised();

			~AI_Supervised();
			void initialize();
			Feature processMyStatus(CharacterStatus cs);
			void think(const CharacterStatus& cs);
			void decodeButtons(int i);
		private:
			std::map<Feature, myutil::Histogram<int, int>> f2act;
			Feature f_prev;
			int command;
		};
	};
};