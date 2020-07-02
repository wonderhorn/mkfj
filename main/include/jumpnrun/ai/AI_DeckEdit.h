#pragma once
#include"jumpnrun/ai/AI.h"
#include<string>

namespace jnr
{
	namespace ai
	{
		class AI_DeckEdit : public AI
		{
		public:
			AI_DeckEdit();

			~AI_DeckEdit();
			void initialize();
			Feature processMyStatus(CharacterStatus cs);
			void think(const CharacterStatus& cs);
			int target_x;
		private:
			Feature f_prev;
			AICommand command;
		};
	};
};