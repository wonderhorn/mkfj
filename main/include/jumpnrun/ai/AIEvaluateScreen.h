#pragma once
#include"jumpnrun/ai/AI.h"
#include"jumpnrun/mover/Mover.h"
#include"jumpnrun/stage/Background.h"
#include"jumpnrun/stage/Stage.h"
#include"framework/Screen.h"
#include"jumpnrun/system/ObjectManager.h"
#include"jumpnrun/system/JumpnRunData.h"
#include<string>
#include<vector>
namespace jnr
{
	class AIEvaluateScreen : public gfw::Screen
	{
	public:
		AIEvaluateScreen(gfw::Game* game);
		~AIEvaluateScreen();
		bool initialize();
		bool initialize(ai::AI* ai);
		void finalize();
		gfw::Screen::eScreenState run(gfw::BaseMedia& media);
		void render(gfw::BaseMedia& media);

		double ai_fitness;
		bool isRendered;
		void StageName(const std::string& str){ stg_name = str; }

	private:
		jnr::JumpnRunData data;
		gfw::Tasklist tl;
		//jnr::Background bg;
		jnr::Stage stg;
		gfw::ObjectManager objmng;
		gfw::Object::Pipe player;
		//jnr::JumpnRunData data;

		std::string bg_name, stg_name;
		int counter;
		bool paused;

		ai::AI* ai;
		int time_limit;
		std::string name;

	};
};