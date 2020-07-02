#pragma once
#include"framework/Screen.h"
#include"jumpnrun/ai/AITrainingScreen.h"
#include"jumpnrun/ai/AIEvaluateScreen.h"
#include"jumpnrun/ai/AI_Bandit.h"
#include<string>
#include<vector>
namespace jnr
{
	namespace ai
	{
		class AITrainingScreen : public gfw::Screen
		{
		public:
			AITrainingScreen(gfw::Game* game);
			~AITrainingScreen();
			bool initialize();
			bool initialize(AI* ai);
			void finalize();
			gfw::Screen::eScreenState run(gfw::BaseMedia& media);
			void render(gfw::BaseMedia& media);

			void setAI(AI* ai){ this->ai = ai; }
			double Score(){ return score; }
			void StageName(const std::string& str){ this->stage_name = str; }

		private:
			AIEvaluateScreen* training_scr;
			AI* ai;
			double score;
			std::string stage_name;
		};
	};
};