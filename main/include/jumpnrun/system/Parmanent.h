#pragma once
#include"framework/VirtualController.h"

#ifdef GFW_WIN32
#include"framework/UDPController.h"
#include"utils/MultiThread.h"
#endif
#ifdef GFW_ADR
#include"android/AndroidMedia.h"
#endif
#ifdef GFW_MAC
#include"framework/mac/MacMedia.h"
#endif

//#include"JumpnRunScreen.h"
//#include"AI_Handmade.h"
//#include"AI_Random.h"
#include"jumpnrun/ai/AI_Planned.h"
//#include"AI_Bandit.h"
#include"framework/MediaAlternative.h"
#include"framework/MediaToFile.h"
#include"jumpnrun/system/Camera.h"
//#include"AI.h"
#include"jumpnrun/ai/AI_Supervised.h"
//#include"AITrainer.h"
#include"jumpnrun/ai/AISuperviser.h"
#include"jumpnrun/spell/Deck.h"
#include"jumpnrun/system/PlayerData.h"
#include"jumpnrun/system/Saver.h"
#include"framework/GameUtil.h"
#include<map>
#include<mutex>
//#ifndef $P
#define $P jnr::Parmanent::Instance()
//#endif

namespace jnr
{
	class JumpnRunScreen;
	class Parmanent
	{
	public:
		static Parmanent& Instance(){ return instance; }

#ifdef GFW_WIN32
		myutil::IOThread iothread;
		gfw::UDPController udpcont;
		gfw::Media media;
#endif
#ifdef GFW_ADR
		gfw::Media media;
		struct android_app* app;
#endif
#ifdef GFW_MAC
    gfw::MediaToFile media;
#endif

		std::string start_time;
		std::string save_file_name;
		bool to_save;
		int rand_seed;
		int window_w, window_h, current_window_w, current_window_h;
		int window_x, window_y;
		int mouse_x, mouse_y;
		gfw::MediaToFile media_to_file;
		gfw::KeyboardController keycont;
		gfw::JoyPadController joycont;
		gfw::AndController andcont;
		gfw::ReplayController replaycont;
		gfw::ControllerRecorder recorder;
		gfw::Camera camera;

		std::map<std::string, ImageHandler> name2img;
		ImageHandler img_obj, img_bigeye, img_bigfish
			, img_map, img_clouds
			, img_building, img_trees, img_rocks
			, img_sky, img_sky_t, img_sky_n
			, img_school, img_school_t, img_school_n
			, img_dojo
			, img_ground, img_ground_t, img_ground_n
			, img_river, img_river_t, img_river_n
			, img_room
			, img_cave
			, img_forest, img_forest_t, img_forest_n
			, img_downtown, img_downtown_t, img_downtown_n
			, img_garden
			, img_mansion, img_cloud
			, img_statusboard, img_cursor, img_menu
			, img_spellboard, img_cards
			, img_effect
			, img_stand, img_stand_s, img_stand_h, img_stand_a, img_stand_e, img_stand_o, img_stand_d, img_stand_b
			, img_stand2, img_stand2_s, img_stand2_h, img_stand2_o
			, img_stand3, img_stand3_s, img_stand3_h, img_stand3_a, img_stand3_o
			, img_stand4, img_stand4_s, img_stand4_h, img_stand4_o, img_stand4_a
			, img_stand5, img_stand5_s, img_stand5_h, img_stand5_o, img_stand5_a
			, img_stand6, img_stand6_a, img_stand6_a2, img_stand6_s
			, img_stand6_o, img_stand6_o2, img_stand6_c, img_stand6_h
			, img_stand6_s1, img_stand6_s2, img_stand6_m, img_stand6_mo
			, img_stand7, img_stand7_s, img_stand7_h, img_stand7_o, img_stand7_a, img_stand7_d
			, img_stand8, img_stand8_s, img_stand8_h, img_stand8_o, img_stand8_a
			, img_stand9, img_stand9_s, img_stand9_a,img_stand9_o, img_stand9_c, img_stand9_h
			, img_paper, img_endrollobj, img_endrollpaper, img_deckedit
			, img_screen, img_logo, img_fujitsubo
			, img_stageselectmap, img_stageselectpanel;
		SoundHandler snd_exp, snd_hit, snd_damaged, snd_mydamaged
			, snd_laser
			, snd_swing, snd_ignition, snd_magic, snd_magicshot, snd_elec, snd_fire
			, snd_kira, snd_door, snd_select, snd_enter, snd_jumping, snd_die
			, snd_cancel, snd_punch, snd_repell
			, snd_nyu, snd_nyu2, snd_suck, snd_blink
			, snd_coin, snd_card, snd_spring, snd_chime
			, snd_powerup, snd_kick;

		std::string bgm_name;
		bool reset_bgm, start_bgm;
		double bgm_segno, bgm_start, bgm_end;

		gfw::MyRand rand;
		jnr::ai::AI_Planned ai;
		//jnr::ai::AITrainer aitrainer;
		jnr::ai::AISuperviser aisuperviser;
		std::vector<PlayerData> players;

		int change_stage;
		int encount_boss;
		bool turn_boss_dummy, appear_boss_dummy;
		bool skip_boss;
		int turn_me;
		std::string next_stage_name;
		gmtr::Vector2D next_start_p;
		std::string message;
		int message_time;
		int deck_cursor;
		int menu_cursor;
		bool turned;

		int stageselect_cursor;

		int volume_se;
		int volume_bgm;

		int num_player_beaten, num_enemies_beaten;
		int total_damage;
		int cleared_stage;
		bool in_stage;

		//gfw::Saver saver;
		std::map<std::string, int> flags;
		std::map<int, int> mylibrary;
		int card_from_gacha;

		bool sound_on;
		std::deque<SoundHandler> sound_stack;

		std::mutex mutex_graphics, mutex_musics, mutex_rand, mutex_tasklist;
		JumpnRunScreen* jnr_scr;

		int getFlag(const std::string& id){ return flags.count(id) ? flags[id] : 0; }
		bool save(const std::string& file_name)const;
		//bool save_config();
		bool load(const std::string& file_name);
		bool renameSaveData(const std::string& file_name = "savedata\\latest.txt");
		void reset();
		void resetData();
		void resetConfig();
		bool loadAllResourses();
		bool loadAllImages();

	private:
		Parmanent();
		static Parmanent instance;
	};

	void iothread_callback(void* param, std::deque<std::string>& q);
};
