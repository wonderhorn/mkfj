#pragma once
#include"framework/Screen.h"
#include"framework/Control.h"
#include"jumpnrun/system/JumpnRunData.h"
#include"framework/GameUtil.h"
#include<string>
#include<deque>
namespace jnr
{
	class ConfigScreen : public gfw::Screen
	{
	public:
		ConfigScreen(gfw::Game* game);
		~ConfigScreen();
		bool initialize();
		void finalize();
		gfw::Screen::eScreenState run(gfw::BaseMedia& media);
		void render(gfw::BaseMedia& media);

	private:
		int cursor, age, counter_cursor;
		std::vector<std::string> values, names;
		gfw::Timer timer_button;
	};
};