#pragma once
#include"framework/Screen.h"
#include"framework/Control.h"
#include"jumpnrun/system/JumpnRunData.h"
#include"framework/GameUtil.h"
#include<string>
#include<deque>

namespace jnr
{
	class Achievement
	{
	public:
		void calc();

		int num_cards, num_card_species, num_max_card_species
			, stage_clear, enemies_beaten, bosses_beaten, you_beaten
			, used_mana, max_score, fujitsubo_power, total_damage;
		bool scenario_cleared;
	};

	class AchievementScreen : public gfw::Screen
	{
	public:
		AchievementScreen(gfw::Game* game);
		~AchievementScreen();
		bool initialize();
		void finalize();
		gfw::Screen::eScreenState run(gfw::BaseMedia& media);
		void render(gfw::BaseMedia& media);

	private:
		int age;
		Achievement ac;
		std::vector<std::string> values, names;
		std::vector<int> stars;
	};
};