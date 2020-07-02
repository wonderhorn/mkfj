#include"jumpnrun/system/Parmanent.h"
#include"utils/string_util.h"

#include"jumpnrun/system/JumpnRunScreen.h"

#include<iostream>
using namespace jnr;
Parmanent Parmanent::instance;

void jnr::iothread_callback(void* param, std::deque<std::string>& q)
{
	while (!q.empty())
	{
		std::string w = q.front();
		w = myutil::chomp(w);

		if (w.size() <= 0)
		{
			q.pop_front();
			continue;
		}
		else if (w == "dump_task_list")
		{
			if ($P.jnr_scr)
			{
				q.pop_front();
				if (!q.empty())
				{
					std::string file_name = q.front();
					if ($P.jnr_scr->dumpTaskListToFile(file_name))
					{
						std::cout << "saved to " << file_name << std::endl;
					}
					else
					{
						std::cout << "failed" << std::endl;
					}
					q.pop_front();
					std::cout << ">>";
				}
				else
				{
					q.push_front(w);
					break;
				}
			}
		}
		else if (w == "load_task_list")
		{
			if ($P.jnr_scr)
			{
				q.pop_front();
				if (!q.empty())
				{
					std::string file_name = q.front();
					if ($P.jnr_scr->readTaskListFromFile(file_name))
					{
						std::cout << "load from " << file_name << std::endl;
					}
					else
					{
						std::cout << "failed" << std::endl;
					}
					q.pop_front();
					std::cout << ">>";
				}
				else
				{
					q.push_front(w);
					break;
				}
			}
		}
		else
		{
			std::cout << "unrecognized verb: " << w << std::endl;
			std::cout << ">>";
			q.clear();
		}
	}
}


Parmanent::Parmanent() :keycont(media.input), joycont(media.input), andcont(&keycont, &joycont)
, save_file_name("savedata/latest.txt"), to_save(true)
, rand_seed(0), window_x(0), window_y(0), mouse_x(0), mouse_y(0)
, rand(123456789), change_stage(-1)
, encount_boss(-1), message(""), message_time(-1), next_stage_name(""), next_start_p(-1, -1)
, turn_boss_dummy(false), appear_boss_dummy(true)
, deck_cursor(0), menu_cursor(0), turned(false), turn_me(-1)
, sound_on(true)
, volume_se(8), volume_bgm(8), bgm_segno(0.0)
, start_bgm(true), reset_bgm(true)
, num_player_beaten(0), cleared_stage(0)
, in_stage(false), stageselect_cursor(0)
, jnr_scr(nullptr)
{
	mylibrary[0] = 3;
	mylibrary[2] = 3;
	mylibrary[4] = 3;
	mylibrary[7] = 3;
	mylibrary[16] = 3;

#ifdef GFW_WIN32
	iothread.setEventHander(iothread_callback);
#endif
	//$P.iothread.begin();
}

bool Parmanent::save(const std::string& file_name)const
{
	gfw::Saver saver;
	saver.set("start_time", start_time);
	saver.set("window_x", myutil::int2str(window_x));
	saver.set("window_y", myutil::int2str(window_y));
	saver.set("window_w", myutil::int2str(window_w));
	saver.set("window_h", myutil::int2str(window_h));

	saver.set("btnidx_z", myutil::int2str(joycont.btnidx_z));
	saver.set("btnidx_x", myutil::int2str(joycont.btnidx_x));
	saver.set("btnidx_a", myutil::int2str(joycont.btnidx_a));
	saver.set("btnidx_s", myutil::int2str(joycont.btnidx_s));
	saver.set("btnidx_space", myutil::int2str(joycont.btnidx_space));

	for (auto i : flags)
	{
		saver.set(std::string("flag") + i.first, myutil::int2str(i.second));
	}
	saver.set("nextstage", next_stage_name);
	saver.set("startx", myutil::int2str(next_start_p.x));
	saver.set("starty", myutil::int2str(next_start_p.y));

	for (int i = 0; i < players.size(); i++)
	{
		std::string player_id = "player" + myutil::int2str(i) + "_";
		saver.set(player_id + "name", players[i].name);
		saver.set(player_id + "current_hp", myutil::int2str(players[i].hp_current));
		saver.set(player_id + "max_hp", myutil::int2str(players[i].hp_max));
		saver.set(player_id + "current_mp", myutil::int2str(players[i].mp_current));
		saver.set(player_id + "max_mp", myutil::int2str(players[i].mp_max));
		saver.set(player_id + "score", myutil::int2str(players[i].score));
		saver.set(player_id + "max_score", myutil::int2str(players[i].max_score));
		saver.set(player_id + "active", myutil::int2str(players[i].isActive));
		std::string deck;
 		for (int j = 0; j < players[i].deck.numCards(); j++)
		{
			int cid = players[i].deck.getCardConst(j).value;
			deck += myutil::int2str(cid);
			if (players[i].deck.numCards() != j + 1)
				deck += ",";
		}
		saver.set(player_id + "deck", deck);
		//if(i == 0)
		//	std::cout << "p0 deck saveed: " << deck << std::endl;

		deck = "";
		for (int j = 0; j < players[i].deck_tmp.numCards(); j++)
		{
			if (players[i].deck_tmp.getCardConst(j).used)
				continue;
			if (deck.length() > 0)
				deck += ",";
			int cid = players[i].deck_tmp.getCardConst(j).value;
			deck += myutil::int2str(cid);
			
		}
		saver.set(player_id + "deck_tmp", deck);
	}

	std::string lib;
	for (auto i : this->mylibrary)
	{
		lib += myutil::int2str(i.first) + "_" + myutil::int2str(i.second) + ",";
	}
	saver.set("library", lib);

	saver.set("num_enemies_beaten", myutil::int2str(num_enemies_beaten));
	saver.set("num_player_beaten", myutil::int2str(num_player_beaten));
	saver.set("total_damage", myutil::int2str(total_damage));
	saver.set("cleared_stage", myutil::int2str(cleared_stage));

	saver.set("stageselect_cursor", myutil::int2str(stageselect_cursor));
	saver.set("in_stage", myutil::int2str(in_stage));

	saver.set("volume_bgm", myutil::int2str(volume_bgm));
	saver.set("volume_se", myutil::int2str(volume_se));

	return saver.save(file_name);
}

bool Parmanent::load(const std::string& file_name)
{
	gfw::Saver saver;
	if (saver.load(file_name))
	{
		this->window_x = myutil::str2int(saver.get("window_x"));
		this->window_y = myutil::str2int(saver.get("window_y"));
		this->window_w = myutil::str2int(saver.get("window_w"));
		this->window_h = myutil::str2int(saver.get("window_h"));

		this->joycont.btnidx_z = myutil::str2int(saver.get("btnidx_z"));
		this->joycont.btnidx_x = myutil::str2int(saver.get("btnidx_x"));
		this->joycont.btnidx_a = myutil::str2int(saver.get("btnidx_a"));
		this->joycont.btnidx_s = myutil::str2int(saver.get("btnidx_s"));
		this->joycont.btnidx_space = myutil::str2int(saver.get("btnidx_space"));

		/*for (int i = 0; i < 10; i++)
		{
			std::string key = std::string("flag") + myutil::int2str(i);
			this->flags[myutil::int2str(i)] = myutil::str2int(saver.get(key));
		}*/

		auto flag_keys = saver.getKeys();
		for (auto key : flag_keys)
		{
			if (key.substr(0, 4) == "flag")
			{
				this->flags[key.substr(4, key.size() - 4)] = myutil::str2int(saver.get(key));
			}
		}

		this->next_stage_name = saver.get("nextstage");
		this->next_start_p.x = myutil::str2int(saver.get("startx"));
		this->next_start_p.y = myutil::str2int(saver.get("starty"));

		for (int i = 0; 1; i++)
		//for (int i = 0; i < players.size(); i++)
		{
			std::string player_id = "player" + myutil::int2str(i) + "_";
			auto name = saver.get(player_id + "name");
			if (name.size() <= 0)
				break;
			if (i >= players.size())
				this->players.push_back(PlayerData());
			this->players[i].name = saver.get(player_id + "name");
			this->players[i].isActive = myutil::str2int(saver.get(player_id + "active"));
			this->players[i].hp_current = myutil::str2int(saver.get(player_id + "current_hp"));
			this->players[i].hp_max = myutil::str2int(saver.get(player_id + "max_hp"));
			this->players[i].mp_current = myutil::str2int(saver.get(player_id + "current_mp"));
			this->players[i].mp_max = myutil::str2int(saver.get(player_id + "max_mp"));
			this->players[i].score = myutil::str2int(saver.get(player_id + "score"));
			this->players[i].max_score = myutil::str2int(saver.get(player_id + "max_score"));

			std::string deck = saver.get(player_id + "deck");
			auto tokens = myutil::split(deck, ",");
			this->players[i].deck.initialize(tokens);

			std::string deck_tmp = saver.get(player_id + "deck_tmp");
			auto tokens_tmp = myutil::split(deck_tmp, ",");
			this->players[i].deck_tmp.initialize(tokens_tmp);

			//if (i == 0)
			//{
			//	std::cout << "p0 deck loaded: " << deck << std::endl;
			//}
		}

		mylibrary.clear();
		std::string lib = saver.get("library");
		auto tokens = myutil::split(lib , ",");
		for (auto t : tokens)
		{
			if (t.size() <= 0)
				continue;
			auto tokens2 = myutil::split(t, "_");
			mylibrary[myutil::str2int(tokens2[0])] = myutil::str2int(tokens2[1]);
		}

		/*saver.set("num_enemies_beaten", myutil::int2str(num_enemies_beaten));
		saver.set("num_player_beaten", myutil::int2str(num_player_beaten));
		saver.set("num_boss1_beaten", myutil::int2str(num_boss1_beaten));
		saver.set("num_boss2_beaten", myutil::int2str(num_boss2_beaten));*/

		this->num_enemies_beaten = myutil::str2int(saver.get("num_enemies_beaten"));
		this->num_player_beaten = myutil::str2int(saver.get("num_player_beaten"));
		this->total_damage = myutil::str2int(saver.get("total_damage"));
		this->cleared_stage = myutil::str2int(saver.get("cleared_stage"));

		this->stageselect_cursor = myutil::str2int(saver.get("stageselect_cursor"));
		this->in_stage = myutil::str2int(saver.get("in_stage"));

		this->volume_bgm = myutil::str2int(saver.get("volume_bgm"));
		this->volume_se = myutil::str2int(saver.get("volume_se"));

		//this->start_time = myutil::str2int(saver.get("start_time"));

		return true;
	}
	else return false;
}

bool Parmanent::renameSaveData(const std::string& file_name)
{
	unsigned int t = time(nullptr);
	std::string str_t = myutil::int2str(t);

#ifdef GFW_WIN32
	CopyFile(
		file_name.c_str(),
		(std::string("savedata/") + str_t + ".txt").c_str(),
		TRUE
	);
#endif
	return true;
}

void Parmanent::reset()
{
	change_stage = -1;
	encount_boss = -1;
	message = "";
	message_time = -1;
	next_stage_name = "";
	next_start_p = gmtr::Vector2D(-1, -1);
	deck_cursor = 0;
	menu_cursor = 0;
	flags.clear();
	for (auto& p : players)
	{
		p.hp_current = p.hp_max = 50;
		p.mp_current = p.mp_max = 50;
		p.score = 0;
		p.max_score = 0;
		p.deck.initialize();
		p.deck.shuffle();
		p.deck_tmp = p.deck;
	}

	mylibrary.clear();
	mylibrary[0] = 3;
	mylibrary[2] = 3;
	mylibrary[4] = 3;
	mylibrary[7] = 3;
	mylibrary[16] = 3;

	card_from_gacha = -1;
	turned = false;

	num_enemies_beaten = 0;
	num_player_beaten = 0;
	total_damage = 0;
	cleared_stage = 0;
	in_stage = false;
	//start_time =
	stageselect_cursor = 0;
	in_stage = true;

	skip_boss = false;
}

void Parmanent::resetData()
{

}
void Parmanent::resetConfig(){}

bool Parmanent::loadAllResourses()
{
	loadAllImages();

	$P.snd_exp = media.sounds.loadSound("sounds/se_maoudamashii_explosion03.wav", 1.1);
	$P.snd_hit = media.sounds.loadSound("sounds/se_maoudamashii_se_sound16.wav");
	$P.snd_damaged = media.sounds.loadSound("sounds/se_maoudamashii_se_sound16.wav", 0.8);
	$P.snd_mydamaged = media.sounds.loadSound("sounds/se_maoudamashii_battle18.wav", 1.2);
	$P.snd_laser = media.sounds.loadSound("sounds/se_maoudamashii_battle_gun05.wav");
	$P.snd_swing = media.sounds.loadSound("sounds/se_maoudamashii_se_sound17.wav", 1.2);
	$P.snd_ignition = media.sounds.loadSound("sounds/se_maoudamashii_se_ignition01.wav", 0.8);
	$P.snd_magic = media.sounds.loadSound("sounds/se_maoudamashii_magical27_editted.wav");
	$P.snd_magicshot = media.sounds.loadSound("sounds/strange_beam.wav");
	$P.snd_elec = media.sounds.loadSound("sounds/elec.wav", 0.85);
	$P.snd_fire = media.sounds.loadSound("sounds/fire4rev.wav", 1.0);
	$P.snd_punch = media.sounds.loadSound("sounds/punch-middle2.wav", 0.85);
	$P.snd_repell = media.sounds.loadSound("sounds/cursor8.wav", 0.85);
	$P.snd_kira = media.sounds.loadSound("sounds/kira2.wav", 0.8);
	$P.snd_select = media.sounds.loadSound("sounds/select05.wav", 0.8);
	$P.snd_enter = media.sounds.loadSound("sounds/decision9.wav", 0.9);
	$P.snd_cancel = media.sounds.loadSound("sounds/cancel2.wav", 1);
	$P.snd_door = media.sounds.loadSound("sounds/door-open1.wav");
	$P.snd_jumping = media.sounds.loadSound("sounds/jump06.wav", 0.9);
	$P.snd_die = media.sounds.loadSound("sounds/final_attack.wav", 0.7);
	$P.snd_nyu = media.sounds.loadSound("sounds/nyu1.wav");
	$P.snd_nyu2 = media.sounds.loadSound("sounds/nyu3.wav", 0.9);
	$P.snd_suck = media.sounds.loadSound("sounds/suck1.wav", 0.9);
	$P.snd_blink = media.sounds.loadSound("sounds/blink.wav", 0.9);
	//$P.snd_laser = media.sounds.loadSound("sounds/attack1.wav", 0.9);
	$P.snd_coin = media.sounds.loadSound("sounds/coin04.wav", 0.9);
	$P.snd_card = media.sounds.loadSound("sounds/se_maoudamashii_onepoint12.wav");
	$P.snd_spring = media.sounds.loadSound("sounds/boyon1.wav");
	$P.snd_chime = media.sounds.loadSound("sounds/school-chime1rev.wav");
	$P.snd_powerup = media.sounds.loadSound("sounds/powerup04.wav");
	$P.snd_kick = media.sounds.loadSound("sounds/kick1.wav");
	return true;
}

bool Parmanent::loadAllImages()
{
	$P.img_sky = media.graphics.loadImage("images/background/sky.png", 640, 480);
	$P.img_sky_t = media.graphics.loadImage("images/background/sky_twilight.png", 640, 480);
	$P.img_sky_n = media.graphics.loadImage("images/background/sky_night.png", 640, 480);
	$P.img_school = media.graphics.loadImage("images/background/school.png", 640, 480);
	$P.img_school_t = media.graphics.loadImage("images/background/school_twilight.png", 640, 480);
	$P.img_school_n = media.graphics.loadImage("images/background/school_night.png", 640, 480);
	$P.img_dojo = media.graphics.loadImage("images/background/dojo.png", 640, 480);
	$P.img_ground = media.graphics.loadImage("images/background/ground.png", 640, 480);
	$P.img_ground_t = media.graphics.loadImage("images/background/ground_twilight.png", 640, 480);
	$P.img_ground_n = media.graphics.loadImage("images/background/ground_night.png", 640, 480);
	$P.img_river = media.graphics.loadImage("images/background/river.png", 640, 480);
	$P.img_river_t = media.graphics.loadImage("images/background/river_twilight.png", 640, 480);
	$P.img_river_n = media.graphics.loadImage("images/background/river_night.png", 640, 480);
	$P.img_room = media.graphics.loadImage("images/background/room.png", 640, 480);
	$P.img_cave = media.graphics.loadImage("images/background/cave.png", 640, 480);
	$P.img_forest = media.graphics.loadImage("images/background/forest2.png", 640, 480);
	$P.img_forest_t = media.graphics.loadImage("images/background/forest_twilight.png", 640, 480);
	$P.img_forest_n = media.graphics.loadImage("images/background/forest_night.png", 640, 480);
	$P.img_downtown = media.graphics.loadImage("images/background/twilight.png", 640, 480);
	$P.img_downtown_n = media.graphics.loadImage("images/background/twilight_night.png", 640, 480);
	$P.img_garden = media.graphics.loadImage("images/background/garden.png", 640, 480);
	$P.img_mansion = media.graphics.loadImage("images/background/mansion.png", 640, 480);

	$P.img_obj = media.graphics.loadImage("images/gazou6.bmp", 1280, 1280, 0xffffffff);
	$P.img_bigeye = media.graphics.loadImage("images/bigeye.bmp", 256, 256, 0xffffffff);
	$P.img_bigfish = media.graphics.loadImage("images/bigfish.bmp", 256, 256, 0xffffffff);
	$P.img_map = media.graphics.loadImage("images/mapchips4.bmp", 960, 960, 0xffffffff);
	$P.img_clouds = media.graphics.loadImage("images/clouds.bmp", 512, 256, 0xff000000);
	$P.img_building = media.graphics.loadImage("images/buildings.bmp", 512, 512, 0xffffffff);
	$P.img_trees = media.graphics.loadImage("images/trees.bmp", 512, 256, 0xffffffff);
	$P.img_rocks = media.graphics.loadImage("images/cave.bmp", 256, 256, 0xffffffff);
	$P.img_statusboard = media.graphics.loadImage("images/statusboard.bmp", 640, 302, 0xffffffff);

	//$P.img_menu = media.graphics.loadImage("images/menu.png", 320, 200, 0);
	$P.img_cursor = media.graphics.loadImage("images/cursor.png", 40, 40, 0);
	$P.img_spellboard = media.graphics.loadImage("images/spellboard.bmp", 64, 480, 0xff000000);
	$P.img_cards = media.graphics.loadImage("images/card.bmp", 512, 512, 0xffffffff);

	$P.img_stand = media.graphics.loadImage("images/stand/9_23_2n.png", 320, 480, 0);
	$P.img_stand_s = media.graphics.loadImage("images/stand/9_23_2s.png", 320, 480, 0);
	$P.img_stand_h = media.graphics.loadImage("images/stand/9_23_2h.png", 320, 480, 0);
	$P.img_stand_a = media.graphics.loadImage("images/stand/9_23_2a.png", 320, 480, 0);
	$P.img_stand_e = media.graphics.loadImage("images/stand/9_23_2e.png", 320, 480, 0);
	$P.img_stand_o = media.graphics.loadImage("images/stand/9_23_2o.png", 320, 480, 0);
	$P.img_stand_d = media.graphics.loadImage("images/stand/9_23_2d.png", 320, 480, 0);
	$P.img_stand_b = media.graphics.loadImage("images/stand/9_23_2b.png", 320, 480, 0);
	$P.img_stand2 = media.graphics.loadImage("images/stand/11_03_n.png", 320, 480, 0);
	$P.img_stand2_s = media.graphics.loadImage("images/stand/11_03_s.png", 320, 480, 0);
	$P.img_stand2_h = media.graphics.loadImage("images/stand/11_03_h.png", 320, 480, 0);
	$P.img_stand2_o = media.graphics.loadImage("images/stand/11_03_o.png", 320, 480, 0);
	$P.img_stand3 = media.graphics.loadImage("images/stand/10_11_2n.png", 320, 480, 0);
	$P.img_stand3_s = media.graphics.loadImage("images/stand/10_11_2s.png", 320, 480, 0);
	$P.img_stand3_h = media.graphics.loadImage("images/stand/10_11_2h.png", 320, 480, 0);
	$P.img_stand3_a = media.graphics.loadImage("images/stand/10_11_2a.png", 320, 480, 0);
	$P.img_stand3_o = media.graphics.loadImage("images/stand/10_11_2o.png", 320, 480, 0);
	$P.img_stand4 = media.graphics.loadImage("images/stand/04_05_n.png", 320, 480, 0);
	$P.img_stand4_s = media.graphics.loadImage("images/stand/04_05_s.png", 320, 480, 0);
	$P.img_stand4_h = media.graphics.loadImage("images/stand/04_05_h.png", 320, 480, 0);
	$P.img_stand4_o = media.graphics.loadImage("images/stand/04_05_c.png", 320, 480, 0);
	$P.img_stand4_a = media.graphics.loadImage("images/stand/04_05_a.png", 320, 480, 0);
	$P.img_stand5 = media.graphics.loadImage("images/stand/10_18_n.png", 320, 480, 0);
	$P.img_stand5_s = media.graphics.loadImage("images/stand/10_18_s.png", 320, 480, 0);
	$P.img_stand5_h = media.graphics.loadImage("images/stand/10_18_h.png", 320, 480, 0);
	$P.img_stand5_o = media.graphics.loadImage("images/stand/10_18_o.png", 320, 480, 0);
	$P.img_stand5_a = media.graphics.loadImage("images/stand/10_18_a.png", 320, 480, 0);
	$P.img_stand6 = media.graphics.loadImage("images/stand/02_08_n.png", 320, 480, 0);
	$P.img_stand6_a = media.graphics.loadImage("images/stand/02_08_a.png", 320, 480, 0);
	$P.img_stand6_a2 = media.graphics.loadImage("images/stand/02_08_ho.png", 320, 480, 0);
	$P.img_stand6_o = media.graphics.loadImage("images/stand/02_08_h2.png", 320, 480, 0);
	$P.img_stand6_o2 = media.graphics.loadImage("images/stand/02_08_o.png", 320, 480, 0);
	$P.img_stand6_h = media.graphics.loadImage("images/stand/02_08_c2.png", 320, 480, 0);
	$P.img_stand6_s1 = media.graphics.loadImage("images/stand/02_08_s2.png", 320, 480, 0);
	$P.img_stand6_s2 = media.graphics.loadImage("images/stand/02_08_sc.png", 320, 480, 0);
	$P.img_stand6_m = media.graphics.loadImage("images/stand/02_08_m.png", 320, 480, 0);
	$P.img_stand6_mo = media.graphics.loadImage("images/stand/02_08_mo.png", 320, 480, 0);
	$P.img_stand7 = media.graphics.loadImage("images/stand/06_25_n.png", 320, 480, 0);
	$P.img_stand7_s = media.graphics.loadImage("images/stand/06_25_s.png", 320, 480, 0);
	$P.img_stand7_h = media.graphics.loadImage("images/stand/06_25_h.png", 320, 480, 0);
	$P.img_stand7_o = media.graphics.loadImage("images/stand/06_25_o.png", 320, 480, 0);
	$P.img_stand7_a = media.graphics.loadImage("images/stand/06_25_a.png", 320, 480, 0);
	$P.img_stand7_d = media.graphics.loadImage("images/stand/06_25_d.png", 320, 480, 0);
	$P.img_stand8 = media.graphics.loadImage("images/stand/11_14_n.png", 320, 480, 0);
	$P.img_stand8_s = media.graphics.loadImage("images/stand/11_14_s.png", 320, 480, 0);
	$P.img_stand8_h = media.graphics.loadImage("images/stand/11_14_h.png", 320, 480, 0);
	$P.img_stand8_o = media.graphics.loadImage("images/stand/11_14_o.png", 320, 480, 0);
	$P.img_stand8_a = media.graphics.loadImage("images/stand/11_14_a.png", 320, 480, 0);
	$P.img_stand9 = media.graphics.loadImage("images/stand/02_12_mc.png", 320, 480, 0);
	$P.img_stand9_o = media.graphics.loadImage("images/stand/02_12_o.png", 320, 480, 0);
	$P.img_stand9_c = media.graphics.loadImage("images/stand/02_12_c.png", 320, 480, 0);
	$P.img_stand9_a = media.graphics.loadImage("images/stand/02_12_a.png", 320, 480, 0);
	$P.img_stand9_s = media.graphics.loadImage("images/stand/02_12_ht.png", 320, 480, 0);
	$P.img_stand9_h = media.graphics.loadImage("images/stand/02_12_h.png", 320, 480, 0);
	//$P.img_stand8_d = media.graphics.loadImage("images/stand/06_25_d.png", 320, 480, 0);

	$P.img_effect = media.graphics.loadImage("images/stand/effects5.png", 1024, 128, 0);
	$P.img_paper = media.graphics.loadImage("images/6_16.png", 640, 480, 0);
	$P.img_endrollpaper = media.graphics.loadImage("images/endroll.png", 640, 480, 0);
	$P.img_endrollobj = media.graphics.loadImage("images/endroll.bmp", 768, 512, 0xffffffff);
	$P.img_deckedit = media.graphics.loadImage("images/deckedit_chip3.bmp", 640, 640, 0xffffffff);
	$P.img_logo = media.graphics.loadImage("images/logo.png", 320, 141, 0);
	//$P.img_fujitsubo = media.graphics.loadImage("images/10_18_resize.png", 400, 400, 0);
	$P.img_fujitsubo = media.graphics.loadImage("images/01_12_trimmed.png", 164, 400, 0);
	$P.img_stageselectmap = media.graphics.loadImage("images/map.png", 640, 480, 0);
	$P.img_stageselectpanel = media.graphics.loadImage("images/panel.png", 98, 26, 0);
	$P.img_screen = media.graphics.createRenderingTarget(640, 480);

	$P.name2img.clear();
	$P.name2img["buildings.bmp"] = $P.img_building;
	$P.name2img["trees.bmp"] = $P.img_trees;
	$P.name2img["cave.bmp"] = $P.img_rocks;

	return true;
}