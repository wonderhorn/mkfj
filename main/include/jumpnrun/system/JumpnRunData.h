#pragma once
#include"framework/MediaAlternative.h"
#include"framework/VirtualController.h"
#include"jumpnrun/system/Camera.h"
#include"jumpnrun/system/PlayerData.h"

namespace jnr
{
	class JumpnRunData
	{
	public:
		JumpnRunData() : turn_boss_dummy(false), open_gacha(false){}
		void pullFromParmanent();
		void pullDeckFromParmanent();
		void pushToParmanent()const;

		gfw::Camera camera;
		std::vector<PlayerData> players;
		bool turned;
		int change_stage;
		int encount_boss;
		bool turn_boss_dummy;
		std::string next_stage_name;
		gmtr::Vector2D next_start_p;
		std::string message;
		int message_time;
		bool in_boss_battle;
		bool open_gacha;

		int num_player_beaten, num_boss1_beaten, num_boss2_beaten
			, num_boss3_beaten, num_boss32_beaten, num_boss4_beaten, num_boss5_beaten
			, num_boss6_beaten, num_boss7_beaten, num_boss8_beaten, num_boss9_beaten
			, num_enemies_beaten;
		int cleared_stage;
	};
};