#pragma once
#include"jumpnrun/system/Widget.h"
#include"jumpnrun/spell/SpellSelector.h"
#include"jumpnrun/mover/Mover.h"
#include"jumpnrun/stage/Background.h"
#include"jumpnrun/stage/Stage.h"
#include"jumpnrun/system/ObjectManager.h"
#include"framework/Screen.h"
#include"framework/Control.h"
#include"jumpnrun/system/JumpnRunData.h"
#include<string>
#include<deque>
namespace jnr
{
	class JumpnRunScreen : public gfw::Screen
	{
	public:
		JumpnRunScreen(gfw::Game* game);
		~JumpnRunScreen();
		bool initialize();
		void finalize();
		gfw::Screen::eScreenState run(gfw::BaseMedia& media);
		void updateObjects();
		void render(gfw::BaseMedia& media);

		bool dumpTaskListToFile(const std::string& file_naem);
		bool readTaskListFromFile(const std::string& file_naem);

	private:
		jnr::JumpnRunData data;
		gfw::Tasklist tl, tl2;
		jnr::Background bg;
		jnr::Stage stg;
		//StatusBoard sb;
		gfw::ObjectManager objmng;
		gfw::Object::Pipe player, boss;
		jnr::StatusBoard sb;
		std::deque<jnr::CONTROL *> controls;
		jnr::SpellSelector ss;
		std::string bg_name, stg_name;
		//FPSChecker fps;
		int counter;
		bool paused;
		int timer_return_from_boss, timer_start_boss, timer_restart, timer_lastbattle;
		bool boss_encounted, boss_beaten;
	};
};