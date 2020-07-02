#pragma once
#include"jumpnrun/ai/AI_Base.h"
#include"jumpnrun/mover/Mover.h"
#include"jumpnrun/stage/Background.h"
#include"jumpnrun/stage/Stage.h"
#include"framework/Screen.h"
#include<string>
#include<vector>
namespace jnr
{
	class AITestScreen : public gfw::Screen
	{
	public:
		AITestScreen(gfw::Game* game);
		~AITestScreen();
		bool initialize();
		bool initialize(const ai::AIParameters& param);
		void finalize();
		gfw::Screen::eScreenState run(gfw::Media& media);
		void render(gfw::Media& media);

		int ai_fitness;

	private:
		gfw::Tasklist tl;
		//jnr::Background bg;
		jnr::Stage stg;
		gfw::Object::Pipe player;

		std::string bg_name, stg_name;
		int counter;
		bool paused;
		bool isRendered;

		ai::AI_Base ai_test;
		int time_limit;
		std::string name;

	};
};