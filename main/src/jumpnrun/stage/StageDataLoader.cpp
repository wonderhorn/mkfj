#include"jumpnrun/mover/Mover.h"
//#include"Object.h"
#include"jumpnrun/mover/SystemObject.h"
#include"jumpnrun/mover/Item.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/mover/block/Switch.h"
#include"jumpnrun/mover/block/block.h"
#include"jumpnrun/mover/enemy/Enemy.h"
#include"jumpnrun/mover/enemy/EnemyGroup.h"
#include"jumpnrun/mover/enemy/PunchingMachine.h"
#include"jumpnrun/mover/enemy/Whackamole.h"
#include"jumpnrun/stage/StageDataLoader.h"
#include"jumpnrun/stage/Stage.h"
#include"jumpnrun/system/ObjectManager.h"
#include"jumpnrun/stage/Background.h"
#include"jumpnrun/mapchip.h"
#include"jumpnrun/GRAPHICS.h"
#include"utils/string_util.h"
#include<fstream>

#ifndef GFW_WIN32
#include<android/asset_manager.h>

//http://www.50ply.com/blog/2013/01/19/loading-compressed-android-assets-with-file-pointer/
static int android_read(void* cookie, char* buf, int size) {
	return AAsset_read((AAsset*)cookie, buf, size);
}

static int android_write(void* cookie, const char* buf, int size) {
	return EACCES; // can't provide write access to the apk
}

static fpos_t android_seek(void* cookie, fpos_t offset, int whence) {
	return AAsset_seek((AAsset*)cookie, offset, whence);
}

static int android_close(void* cookie) {
	AAsset_close((AAsset*)cookie);
	return 0;
}

#endif

using namespace jnr;
using namespace myutil;
#define $V gmtr::Vector2D

static void cpyString(char buf[JNR_FLAG_SIZE], const std::string& str)
{
	assert(str.size() < JNR_FLAG_SIZE);
	for (int i = 0; i < str.size(); i++)
	{
		buf[i] = str[i];
	}
	buf[JNR_FLAG_SIZE - 1] = '\0';
}

bool StageDataLoader::load(const std::string file_name, gfw::Tasklist* tl
	, Stage& stg, gfw::ObjectManager& om, Background* bg)
{
	stg.initialize();

#ifdef GFW_WIN32
	std::ifstream ifs(file_name);
	std::cout << "loading " << file_name << std::endl;
	if (!ifs)
	{
		return false;
	}
#else
	AAssetManager* mgr = $P.app->activity->assetManager;
	AAssetDir* assetdir = AAssetManager_openDir(mgr, "");
	while (const char* ptr = AAssetDir_getNextFileName(assetdir))
	{
		std::cout << ptr << std::endl;
	}
	std::string fn = file_name;
	for (int i = 0; i < fn.size(); i++)
	{
		if (fn[i] == '\\')
			fn[i] = '/';
	}
	AAsset* asset = AAssetManager_open(mgr, fn.c_str(), AASSET_MODE_UNKNOWN);
	//AAsset* asset = AAssetManager_open(mgr, "test.png", AASSET_MODE_UNKNOWN);
	FILE* fp = funopen(asset, android_read, android_write, android_seek, android_close);
	if (!fp)
		return -1;
	//std::ifstream ifs(fp);

	//while (char c = fgetc(fp)) {
	//	std::cout << c;
	//}

#endif

	std::string line;
	std::string background;
	double bg_translate_rate_x = 1.0, bg_translate_rate_y = 1.0;
	int bg_offset_x = 64, bg_offset_y = 64
		, bg_image_width = 640, bg_image_height = 480
		, bg_x_min = -99999, bg_y_min = -99999
		, bg_x_max = 99999, bg_y_max = 99999;
	std::string bgm = "";

#ifdef GFW_WIN32
	while (std::getline(ifs, line))
	{
#else
	char buf[10240] = "";
	while(fgets(buf, 10239, fp))
	{
		std::string line(buf);
#endif
		line = myutil::chomp(line);
		auto tokens = split(line, " ");
		std::string& verb = tokens[0];
		if (verb == "name"){}
		else if (verb == "width")
		{
			stg.width = str2int(tokens[1]);
		}
		else if (verb == "height")
		{
			stg.height = str2int(tokens[1]);
			if (stg.chips)
				delete[] stg.chips;
			stg.chips = new Mapchip[(stg.width + 1) * (stg.height + 1)];
			for (int x = 0; x < stg.width; x++)
			{
				for (int y = 0; y < stg.height; y++)
				{
					Mapchip& c = stg.chips[x / Mapchip::WIDTH + y / Mapchip::HEIGHT * stg.width];
					c.x = x;
					c.y = y;
					c.refx = -1;
					c.refy = -1;
					c.type = 'n';
					c.w = Mapchip::WIDTH;
					c.h = Mapchip::HEIGHT;
				}
			}
			if (bg)
				bg_offset_y = stg.height * Mapchip::HEIGHT - bg->img_height;
		}
		else if (verb == "curtain")
		{
			stg.curtainr = str2int(tokens[1]);
			stg.curtaing = str2int(tokens[2]);
			stg.curtainb = str2int(tokens[3]);
			stg.curtaina = str2int(tokens[4]);
		}
		else if (verb == "mapchip")
		{
			int x = str2int(tokens[1]);
			int y = str2int(tokens[2]);
			int refx = str2int(tokens[3]);
			int refy = str2int(tokens[4]);
			int index_x = x / Mapchip::WIDTH;
			int index_y = y / Mapchip::HEIGHT;
			if (index_x >= stg.width )
				continue;
			int index = index_x + index_y * stg.width;
			//assert(index < stg.width * stg.height);
			Mapchip& c = stg.chips[index];
			c.x = x;
			c.y = y;
			c.refx = refx;
			c.refy = refy;
			auto chip = Mapchip::Ref2Chip(refx, refy);
			c.type = chip.type;
			c.alpha = chip.alpha;
			c.foreground = chip.foreground;
			c.waterproof = chip.waterproof;
			c.w = Mapchip::WIDTH;
			c.h = Mapchip::HEIGHT;
			if (tokens.size() >= 8)
				c.data = myutil::replace(tokens[7], "\\n", "\n");
			//else
			//	c.data = "";
		}
		else if (verb == "billboard")
		{
			Billboard b;
			b.x = str2int(tokens[1]);
			b.y = str2int(tokens[2]);
			b.refx = str2int(tokens[3]);
			b.refy = str2int(tokens[4]);
			b.w = str2int(tokens[5]);
			b.h = str2int(tokens[6]);
			b.translate_rate_x = str2double(tokens[7]);
			b.translate_rate_y = str2double(tokens[8]);
			//b.file_name = tokens[7];
			b.file_name = "";
			for (int i = 9; i < tokens.size(); i++)
			{
				b.file_name += tokens[i];
				if (i < tokens.size() - 1)
				{
					b.file_name += " ";
				}
			}
			//b.translate_rate_x = 0.2;
			//b.translate_rate_y = 1.0;
			stg.billboards.push_back(b);
		}
		else if (verb == "object")
		{
			int x = str2int(tokens[1]);
			int y = str2int(tokens[2]) - 1;
			int refx = str2int(tokens[3]);
			int refy = str2int(tokens[4]);

			if (refx == REFPOINT_ME_X && refy == REFPOINT_ME_Y)
			{
				stg.startp.x = x;
				stg.startp.y = y;
			}
			else if (refx == REFPOINT_BOY_X && refy == REFPOINT_BOY_Y)
			{
				om.pushObject(x, y, "boy");
			}
			else if (refx == REFPOINT_SALARYMAN_X && refy == REFPOINT_SALARYMAN_Y)
			{
				om.pushObject(x, y, "salaryman");
			}
			else if (refx == REFPOINT_GIRL1_X && refy == REFPOINT_GIRL1_Y)
			{
				om.pushObject(x, y, "girl1");
			}
			else if (refx == REFPOINT_GIRL2_X && refy == REFPOINT_GIRL2_Y)
			{
				om.pushObject(x, y, "girl2");
			}
			else if (refx == REFPOINT_GIRL3_X && refy == REFPOINT_GIRL3_Y)
			{
				om.pushObject(x, y, "girl3");
			}
			else if (refx == REFPOINT_GIRL4_X && refy == REFPOINT_GIRL4_Y)
			{
				om.pushObject(x, y, "girl4");
			}
			else if (refx == REFPOINT_GIRL5_X && refy == REFPOINT_GIRL5_Y)
			{
				om.pushObject(x, y, "girl5");
			}
			else if (refx == REFPOINT_VOLLEYGIRL_X && refy == REFPOINT_VOLLEYGIRL_Y)
			{
				om.pushObject(x, y, "volley");
			}
			else if (refx == REFPOINT_KENDO_X && refy == REFPOINT_KENDO_Y)
			{
				om.pushObject(x, y, "kendo");
			}
			else if (refx == REFPOINT_BICYCLE_X && refy == REFPOINT_BICYCLE_Y)
			{
				om.pushObject(x, y, "bicycle");
			}
			else if (refx == REFPOINT_CAR_X && refy == REFPOINT_CAR_Y)
			{
				om.pushObject(x, y, "car");
			}
			else if (refx == REFPOINT_CAR_X + 96 && refy == REFPOINT_CAR_Y)
			{
				om.pushObject(x, y, "car2");
			}
			else if (refx == REFPOINT_LONGCAR_X && refy == REFPOINT_LONGCAR_Y)
			{
				om.pushObject(x, y, "longcar");
			}
			else if (refx == REFPOINT_BEE_X && refy == REFPOINT_BEE_Y)
			{
				om.pushObject(x, y, "bee");
			}
			else if (refx == REFPOINT_FROG_X && refy == REFPOINT_FROG_Y)
			{
				om.pushObject(x, y, "frog");
			}
			else if (refx == REFPOINT_MUSHROOM_X && refy == REFPOINT_MUSHROOM_Y)
			{
				om.pushObject(x, y, "mushroom");
			}
			else if (refx == REFPOINT_SNAIL_X && refy == REFPOINT_SNAIL_Y)
			{
				om.pushObject(x, y, "snail");
			}
			else if (refx == REFPOINT_BUG_BLACK_X && refy == REFPOINT_BUG_BLACK_Y)
			{
				om.pushObject(x, y, "bug");
			}
			else if (refx == REFPOINT_PYLONMAN_X && refy == REFPOINT_PYLONMAN_Y)
			{
				om.pushObject(x, y, "pylonman");
			}
			else if (refx == REFPOINT_DUSTBOXMAN_X && refy == REFPOINT_DUSTBOXMAN_Y)
			{
				om.pushObject(x, y, "dustboxman");
			}
			else if (refx == REFPOINT_CANNON_X && refy == REFPOINT_CANNON_Y)
			{
				om.pushObject(x, y, "cannon");
			}
			else if (refx == REFPOINT_MANDRAKE_X && refy == REFPOINT_MANDRAKE_Y)
			{
				om.pushObject(x, y, "mandrake");
			}
			else if (refx == REFPOINT_GOBLIN_X && refy == REFPOINT_GOBLIN_Y)
			{
				om.pushObject(x, y, "goblin");
			}
			else if (refx == REFPOINT_RESEARCHER_X && refy == REFPOINT_RESEARCHER_Y)
			{
				om.pushObject(x, y, "researcher");
			}
			else if (refx == REFPOINT_ELF_X && refy == REFPOINT_ELF_Y)
			{
				om.pushObject(x, y, "elf");
			}
			else if (refx == REFPOINT_GARGOYLE_X && refy == REFPOINT_GARGOYLE_Y)
			{
				om.pushObject(x, y, "gargoyle");
			}
			else if (refx == REFPOINT_PUMPKIN_X && refy == REFPOINT_PUMPKIN_Y)
			{
				om.pushObject(x, y, "pumpkin");
			}
			else if (refx == REFPOINT_CANDLE_X && refy == REFPOINT_CANDLE_Y)
			{
				om.pushObject(x, y, "candle");
			}
			else if (refx == REFPOINT_HAND_X && refy == REFPOINT_HAND_Y)
			{
				om.pushObject(x, y, "hand");
			}
			else if (refx == REFPOINT_FAIRY_BLUE_X && refy == REFPOINT_FAIRY_BLUE_Y)
			{
				om.pushObject(x, y, "fairy_blue");
			}
			else if (refx == REFPOINT_FISH_X && refy == REFPOINT_FISH_Y)
			{
				om.pushObject(x, y, "fish");
			}
			else if (refx == REFPOINT_JELLYFISH_X && refy == REFPOINT_JELLYFISH_Y)
			{
				om.pushObject(x, y, "jellyfish");
			}
			else if (refx == REFPOINT_URCHIN_X && refy == REFPOINT_URCHIN_Y)
			{
				om.pushObject(x, y, "urchin");
			}
			else if (refx == REFPOINT_FIRE_X && refy == REFPOINT_FIRE_Y)
			{
				//om.pushObject(x, y, "jewel");
				//Item* ptr = gfw::generate<Item>(*tl);
				EmitFire* ptr = generateMover(EmitFire, tl);
				ptr->initialize(gmtr::Vector2D(x, y), gmtr::Vector2D(0, -4)
					, 1, 20);
				ptr->LifeTime(999999);
			}
			else if (refx == REFPOINT_JEWEL_X && refy == REFPOINT_JEWEL_Y + 32
				|| refx == REFPOINT_JEWEL_X + 32 && refy == REFPOINT_JEWEL_Y
				|| refx == REFPOINT_JEWEL_X + 32 && refy == REFPOINT_JEWEL_Y + 32)
			{
				//om.pushObject(x, y, "jewel");
				//Item* ptr = gfw::generate<Item>(*tl);
				Item* ptr = generateMover(Item, tl);
				ptr->initialize(REFPOINT_JEWEL_X, REFPOINT_JEWEL_Y + 32, gmtr::Vector2D(x, y)
					, -1, 20, gmtr::Vector2D(0, 0), 0);
				if (tokens.size() >= 8)
				{
					auto etc = myutil::split(tokens[7], ",");
					if (etc.size() >= 1)
					{
						cpyString( ptr->flag_id, etc[0]);
					}
				}
				ptr->LifeTime(999999);
			}
			else if ((refx == REFPOINT_TRAP_X && refy == REFPOINT_TRAP_Y) ||
				(refx == REFPOINT_TRAP_UP_X && refy == REFPOINT_TRAP_UP_Y) ||
				(refx == REFPOINT_TRAP_RIGHT_X && refy == REFPOINT_TRAP_RIGHT_Y) ||
				(refx == REFPOINT_TRAP_DOWN_X && refy == REFPOINT_TRAP_DOWN_Y) ||
				(refx == REFPOINT_TRAP_LEFT_X && refy == REFPOINT_TRAP_LEFT_Y) ||
				(refx == 384 && refy == 512))
			{
				y += 1;
				std::vector<std::string> etc = {};
				if(tokens.size() >= 8)
					etc = myutil::split(tokens[7], ",");
				std::string delay = "0";
				if (etc.size() >= 1)
				{
					using enm::TrapTimered;
					delay = etc[0];
					std::vector<double> args(3);
					args[0] = refx;
					args[1] = refy;
					args[2] = str2double(delay);
					om.pushObject(x, y, std::string("trap"), args);
					//enm::TrapTimered* ptr = gfw::generate<enm::TrapTimered>(*tl);
					//TrapTimered* ptr = generateMover(TrapTimered, tl);
					//ptr->initialize(refx, refy, gmtr::Vector2D(x, y), 1, 25, str2double(delay));
				}
				else
				{
					std::vector<double> args(3);
					args[0] = refx;
					args[1] = refy;
					args[2] =-1;
					using enm::Trap;
					om.pushObject(x, y, std::string("trap"), args);
					//enm::Trap* ptr = gfw::generate<enm::Trap>(*tl);
					//enm::Trap* ptr = generateMover(Trap, tl);
					//ptr->initialize(refx, refy, gmtr::Vector2D(x, y), 1, 10);
				}
			}
			else if ((refx == REFPOINT_TRAP_X + 128 && refy == REFPOINT_TRAP_Y) ||
				(refx == REFPOINT_TRAP_UP_X + 128 && refy == REFPOINT_TRAP_UP_Y) ||
				(refx == REFPOINT_TRAP_RIGHT_X + 128 && refy == REFPOINT_TRAP_RIGHT_Y) ||
				(refx == REFPOINT_TRAP_DOWN_X + 128 && refy == REFPOINT_TRAP_DOWN_Y) ||
				(refx == REFPOINT_TRAP_LEFT_X + 128 && refy == REFPOINT_TRAP_LEFT_Y) ||
				(refx == 384 && refy == 512))
			{
				y += 1;
				std::vector<std::string> etc = {};
				if (tokens.size() >= 8)
					etc = myutil::split(tokens[7], ",");
				std::string delay = "0";
				if (etc.size() >= 1)
				{
					using enm::TrapTimered;
					delay = etc[0];
					std::vector<double> args(3);
					args[0] = refx;
					args[1] = refy;
					args[2] = str2double(delay);
					om.pushObject(x, y, std::string("trap_red"), args);
					//enm::TrapTimered* ptr = gfw::generate<enm::TrapTimered>(*tl);
					TrapTimered* ptr = generateMover(TrapTimered, tl);
					ptr->initialize(refx, refy, gmtr::Vector2D(x, y), 1, 25, str2double(delay));
				}
				else
				{
					using enm::Trap;
					std::vector<double> args(3);
					args[0] = refx;
					args[1] = refy;
					args[2] = -1;
					om.pushObject(x, y, std::string("trap_red"), args);
					//enm::Trap* ptr = gfw::generate<enm::Trap>(*tl);
					//enm::Trap* ptr = generateMover(Trap, tl);
					//ptr->initialize(refx, refy, gmtr::Vector2D(x, y), 1, 25, 1);
				}
			}
			else if (refx == REFPOINT_HEART_X && refy == REFPOINT_HEART_Y)
			{
				y += 1;
				//Item* ptr = gfw::generate<Item>(*tl);
				Item* ptr = generateMover(Item, tl);
				ptr->initialize(REFPOINT_HEART_X, REFPOINT_HEART_Y , gmtr::Vector2D(x, y)
					,  -1, 20, gmtr::Vector2D(0, 0), 0);
				ptr->hp_gain = 10;
				ptr->mp_gain = 0;
				ptr->score_gain = 10;
				ptr->LifeTime(999999);
			}
			else if (refx == REFPOINT_HPCAN_X && refy == REFPOINT_HPCAN_Y)
			{
				y += 1;
				auto etc = myutil::split(tokens[7], ",");
				std::string flag_id = "";
				if (etc.size() >= 1)
				{
					flag_id = etc[0];
				}
				if ($P.getFlag(flag_id) == 0)
				{
					//Item* ptr = gfw::generate<HPCan>(*tl);
					Item* ptr = generateMover(HPCan, tl);
					ptr->initialize(REFPOINT_HPCAN_X, REFPOINT_HPCAN_Y, gmtr::Vector2D(x, y)
						, -1, 20, gmtr::Vector2D(0, 0), 0);
					ptr->hp_gain = 10;
					ptr->mp_gain = 0;
					ptr->score_gain = 0;
					ptr->LifeTime(999999);
					cpyString(ptr->flag_id, flag_id);
				}
			}
			else if (refx == REFPOINT_MPCAN_X && refy == REFPOINT_MPCAN_Y)
			{
				y += 1;
				auto etc = myutil::split(tokens[7], ",");
				std::string flag_id = "";
				if (etc.size() >= 1)
				{
					flag_id = etc[0];
				}
				if ($P.getFlag(flag_id) == 0)
				{
					Item* ptr = generateMover(MPCan, tl);
					//Item* ptr = gfw::generate<MPCan>(*tl);
					ptr->initialize(REFPOINT_MPCAN_X, REFPOINT_MPCAN_Y, gmtr::Vector2D(x, y)
						, -1, 20, gmtr::Vector2D(0, 0), 0);
					ptr->hp_gain = 0;
					ptr->mp_gain = 10;
					ptr->score_gain = 0;
					ptr->LifeTime(999999);
					cpyString(ptr->flag_id, flag_id);
				}
			}
			else if (refx == REFPOINT_CARD_X && refy == REFPOINT_CARD_Y
				|| refx == REFPOINT_CARD_X - 32 && refy == REFPOINT_CARD_Y)
			{
				auto etc = myutil::split(tokens[7], ",");
				std::string flag_id = "";
				int card_id = -1;
				if (etc.size() >= 2)
				{
					card_id = myutil::str2int(etc[0]);
					flag_id = etc[1];
				}
				else if (etc.size() >= 1)
				{
					card_id = myutil::str2int(etc[0]);
				}
				//bool b = flag_id < 0 || $P.getFlag(flag_id);
				int f = $P.getFlag(flag_id);
				if (flag_id.size() == 0)
				{
					GetCard* ptr = generateMover(GetCard, tl);
					ptr->initialize(REFPOINT_CARD_X, REFPOINT_CARD_Y, gmtr::Vector2D(x, y)
						, -1, 20, card_id, flag_id);
				}
				else if ($P.getFlag(flag_id) == 0)
				{
					GetCard* ptr = generateMover(GetCard, tl);
					int refx = REFPOINT_CARD_X - 32;
					if (card_id >= 59)
						refx = REFPOINT_CARD_X + 32;
					ptr->initialize(refx, REFPOINT_CARD_Y, gmtr::Vector2D(x, y)
						, -1, 20, card_id, flag_id);
				}
			}
			else if (refx == REFPOINT_BOOK_X && refy == REFPOINT_BOOK_Y)
			{
				//DeckReset* ptr = gfw::generate<DeckReset>(*tl);
				DeckReset* ptr = generateMover(DeckReset, tl);
				ptr->initialize(REFPOINT_BOOK_X, REFPOINT_BOOK_Y,  gmtr::Vector2D(x, y)
					, -1, 0, gmtr::Vector2D(0, 0), 0);

			}
			else if (refx == REFPOINT_HOLE_X && refy == REFPOINT_HOLE_Y)
			{
				y++;
				auto etc = myutil::split(tokens[7], ",");
				std::string next_stage_name = etc[0];
				int next_x = str2int(etc[1]);
				int next_y = str2int(etc[2]);
				//gfw::generate<NextStage>(*tl)->initialize(REFPOINT_HOLE_X, REFPOINT_HOLE_Y
				//	, gmtr::Vector2D(x, y), 0, 0.0, next_stage_name, next_x, next_y);
				NextStage* ptr = generateMover(NextStage, tl);
				ptr->initialize(REFPOINT_HOLE_X, REFPOINT_HOLE_Y
					, gmtr::Vector2D(x, y), 0, 0.0, next_stage_name, next_x, next_y);
			}
			else if (refx == REFPOINT_GATE_X && refy == REFPOINT_GATE_Y)
			{
				y++;
				std::vector<std::string> etc = {};
				std::string next_stage_name = "";
				int next_x = 0;
				int next_y = 0;
				if (tokens.size() > 7) {
					etc = myutil::split(tokens[7], ",");
					next_stage_name = etc[0];
					next_x = str2int(etc[1]);
					next_y = str2int(etc[2]);
				}
				generateMover(NextStage, tl)->initialize(REFPOINT_GATE_X, REFPOINT_GATE_Y
					, gmtr::Vector2D(x, y), 0, 0.0, next_stage_name, next_x, next_y);
				generateMover(NextStage, tl)->initialize(REFPOINT_GATE_X, REFPOINT_GATE_Y + 32
					, gmtr::Vector2D(x, y + 32), 0, 0.0, next_stage_name, next_x, next_y);
			}
			else if (refx == REFPOINT_PUNCHING_MACHINE_X && refy == REFPOINT_PUNCHING_MACHINE_Y)
			{
				using namespace enm;
				//DeckReset* ptr = gfw::generate<DeckReset>(*tl);
				PunchingMachine* ptr = generateMover(PunchingMachine, tl);
				ptr->initialize(gmtr::Vector2D(x, y), -1);
			}
			else if (refx == REFPOINT_WHACKAMOLE_X && refy == REFPOINT_WHACKAMOLE_Y)
			{
				using namespace enm;
				//DeckReset* ptr = gfw::generate<DeckReset>(*tl);
				Whackamole* ptr = generateMover(Whackamole, tl);
				std::string flag_id = "";
				if (tokens.size() > 7)
				{
					flag_id = tokens[7];
				}
				ptr->initialize(gmtr::Vector2D(x, y), -1, true, flag_id);

			}
			//*************************************************** effects *****************************************************
			else if (refx == REFPOINT_SEAWEED_X && refy == REFPOINT_SEAWEED_Y)
			{
				y += 1;
				//Effect* ptr = gfw::generate<Effect>(*tl);
				Effect* ptr = generateMover(Effect, tl);
				ptr->initialize(REFPOINT_SEAWEED_X, REFPOINT_SEAWEED_Y, 32, 32, gmtr::Vector2D(x, y)
					, 99999, gmtr::Vector2D(0, 0), 1);
				ptr->y_anim_interval = 45;
				ptr->y_anim_num = 2;
			}
			else if (refx >= REFPOINT_NEON_X && refx <= REFPOINT_NEON_X + 128
				&& refy == REFPOINT_NEON_Y)
			{
				y += 1;
				//Effect* ptr = gfw::generate<Effect>(*tl);
				Effect* ptr = generateMover(Effect, tl);
				ptr->initialize(refx, refy, 32, 32, gmtr::Vector2D(x, y)
					, 99999, gmtr::Vector2D(0, 0), 0);
				ptr->y_anim_interval = 45;
				ptr->y_anim_num = 2;
			}
			else if (refx == REFPOINT_SLEEPINGKYO_X && refy == REFPOINT_SLEEPINGKYO_Y)
			{
				y += 1;
				//Effect* ptr = gfw::generate<Effect>(*tl);
				Effect* ptr = generateMover(Effect, tl);
				ptr->initialize(refx, refy, 64, 32, gmtr::Vector2D(x, y)
					, 99999, gmtr::Vector2D(0, 0), 0);
				ptr->y_anim_interval = 120;
				ptr->y_anim_num = 2;
			}
			//*************************************************** boss ********************************************************

			else if (refx == REFPOINT_BOSSPOINT_X && refy == REFPOINT_BOSSPOINT_Y)
			{
				std::string flag_id = "";
				int boss_id = 0;
				if (tokens.size() >= 8)
				{
					auto etc = myutil::split(tokens[7], ",");

					if (etc.size() >= 2)
					{
						boss_id = myutil::str2int(etc[0]);
						flag_id = etc[1];
					}
					else if (etc.size() >= 1)
					{
						boss_id = myutil::str2int(etc[0]);
					}
				}
				generateMover(EncountBoss, tl)->initialize(REFPOINT_BOSSPOINT_X, REFPOINT_BOSSPOINT_Y
					, gmtr::Vector2D(x, y), 0, 0.0, boss_id, flag_id);
			}
			/*else if (refx == REFPOINT_BOSS3_X && refy == REFPOINT_BOSS3_Y)
			{
				if ($P.num_boss1_beaten == 0)
				{
					stg.bossp.x = x;
					stg.bossp.y = y;
					generateMover(BossDummy, tl)->initialize(refx, refy, 32, 64
						, gmtr::Vector2D(x, y), 1.0);
				}
			}*/
			else if (refx == REFPOINT_BOSS1_X && refy == REFPOINT_BOSS1_Y ||
				refx == REFPOINT_BOSS2_X && refy == REFPOINT_BOSS2_Y ||
				refx == REFPOINT_BOSS3_X && refy == REFPOINT_BOSS3_Y ||
				refx == REFPOINT_BOSS4_X && refy == REFPOINT_BOSS4_Y ||
				refx == REFPOINT_BOSS5_X && refy == REFPOINT_BOSS5_Y ||
				refx == REFPOINT_BOSS6_X && refy == REFPOINT_BOSS6_Y)
			{
				stg.bossp.x = x;
				stg.bossp.y = y;
				gfw::generate<BossDummy>(*tl)->initialize(refx, refy, 32, 64
					, gmtr::Vector2D(x, y), 1.0);

				if (tokens.size() >= 8)
				{
					auto etc = myutil::split(tokens[7], ",");
					if (etc.size() >= 1)
					{
						$P.appear_boss_dummy = bool(myutil::str2int(etc[0]));
					}
				}
			}
			else if (refx == REFPOINT_CHANDELIER_X && refy == REFPOINT_CHANDELIER_Y)
			{
				Chandelier* ptr = gfw::generate<Chandelier>(*tl);
				ptr->initialize($V(x, y), 99999, $V(0, 0), 1.5);
				//Effect* ptr = gfw::generate<Effect>(*tl);
				//ptr->initialize(refx, refy, 64, 32, $V(x, y), 99999, $V(0, 0), 0.0);
				ptr->y_anim_interval = 6;
				ptr->y_anim_num = 2;
			}
			else if (refx == REFPOINT_BOSS1_X + 32 * 18 && refy == REFPOINT_BOSS1_Y)
			{
				HangedKyo* ptr = gfw::generate<HangedKyo>(*tl);
				ptr->initialize(refx, refy, $V(x, y));
			}

			else if (refx == REFPOINT_EVENT_X && refy == REFPOINT_EVENT_Y)
			{
				std::string flag_id = "";
				if (tokens.size() >= 8)
				{
					flag_id = tokens[7];
				}
				generateMover(EventCaller, tl)->initialize(REFPOINT_BOSSPOINT_X, REFPOINT_BOSSPOINT_Y
					, gmtr::Vector2D(x, y), 0, 0.0, flag_id);
			}
			//*************************************************** switches ********************************************************
			else if (refx == REFPOINT_TOGGLESWITCH_X && refy == REFPOINT_TOGGLESWITCH_Y)
			{
				y += 1;
				Switch* ptr = gfw::generate<Switch>(*tl);
				ptr->initialize(REFPOINT_TOGGLESWITCH_X, REFPOINT_TOGGLESWITCH_Y, gmtr::Vector2D(x, y), -1);
				ptr->setSwitchType(true, -1);
				if (tokens.size() > 7)
				{
					std::string fid = tokens[7];
					cpyString(ptr->flag_id, fid);
					ptr->setOn($P.getFlag(fid));
				}
			}
			else if (refx == REFPOINT_SWITCH_X && refy == REFPOINT_SWITCH_Y)
			{
				y += 1;
				Switch* ptr = gfw::generate<Switch>(*tl);
				ptr->initialize(REFPOINT_SWITCH_X, REFPOINT_SWITCH_Y, gmtr::Vector2D(x, y), -1, 1.0);
				ptr->setSwitchType(false, 240);
				if (tokens.size() > 7)
				{
					std::string fid = tokens[7]; cpyString(ptr->flag_id, fid);
					cpyString(ptr->flag_id, fid);
					$P.flags[fid] = 0;
				}
			}
			else if (refx == REFPOINT_SWITCH_LEFT_X && refy == REFPOINT_SWITCH_LEFT_Y)
			{
				y += 1;
				Switch* ptr = gfw::generate<Switch>(*tl);
				ptr->initialize(REFPOINT_SWITCH_LEFT_X, REFPOINT_SWITCH_LEFT_Y, gmtr::Vector2D(x, y), -1);
				ptr->setSwitchType(false, 180);
				if (tokens.size() > 7)
				{
					std::string fid = tokens[7];
					cpyString(ptr->flag_id, fid);
					$P.flags[fid] = 0;
				}
			}
			else if (refx == REFPOINT_SWITCH_RIGHT_X && refy == REFPOINT_SWITCH_RIGHT_Y)
			{
				y += 1;
				Switch* ptr = gfw::generate<Switch>(*tl);
				ptr->initialize(REFPOINT_SWITCH_RIGHT_X, REFPOINT_SWITCH_RIGHT_Y, gmtr::Vector2D(x, y), -1);
				ptr->setSwitchType(false, 180);
				if (tokens.size() > 7)
				{
					std::string fid = tokens[7];
					cpyString(ptr->flag_id, fid);
					$P.flags[fid] = 0;
				}
			}
			else if (refx == REFPOINT_DOOR_X && refy == REFPOINT_DOOR_Y
				|| refx == REFPOINT_SHUTTER_X && refy == REFPOINT_SHUTTER_Y
				|| refx == REFPOINT_STARGATE_X && refy == REFPOINT_STARGATE_Y
				|| refx == REFPOINT_STARGATE_X + 64 && refy == REFPOINT_STARGATE_Y
				|| refx == REFPOINT_STARGATE_X + 128 && refy == REFPOINT_STARGATE_Y)
			{
				y += 1;
				Door* ptr = gfw::generate<Door>(*tl);
				ptr->initialize(refx, refy, gmtr::Vector2D(x, y), -1);
				if (tokens.size() > 7)
				{
					std::string fid = tokens[7];
					cpyString(ptr->flag_id, fid);
				}
			}
			else if (refx == REFPOINT_BOMBBOX_X && refy == REFPOINT_BOMBBOX_Y)
			{
				BombBox* ptr = gfw::generate<BombBox>(*tl);
				ptr->initialize(REFPOINT_BOMBBOX_X, REFPOINT_BOMBBOX_Y, gmtr::Vector2D(x, y), -1);
				if (tokens.size() > 7)
				{
					std::string fid = tokens[7];
					cpyString(ptr->flag_id, fid);
				}
			}
			//アイテムボックス　乗れるやつ
			else if ((refx == REFPOINT_BOX_X && refy == REFPOINT_BOX_Y)
				|| (refx == REFPOINT_CONSOLE_X && refy == REFPOINT_CONSOLE_Y))
			{
				ItemBlock* ptr = gfw::generate<ItemBlock>(*tl);
				ptr->initialize(refx, refy, gmtr::Vector2D(x, y), -1, 0);
				ptr->item_type = ItemBlock::ItemBlockType::Empty;
				if (tokens.size() > 7)
				{
					auto etc = myutil::split(tokens[7], ",");
					if (etc[0] == "coin")
					{
						ptr->item_type = ItemBlock::ItemBlockType::Coin;
						cpyString(ptr->flag_id, "");
					}
					else if (etc[0] == "card")
					{
						ptr->item_type = ItemBlock::ItemBlockType::CommonCard;
						int v = myutil::str2int(etc[1]);
						ptr->item_value = v;
						cpyString(ptr->flag_id, "");
					}
					else if (etc[0] == "hpcan")
					{
						ptr->item_type = ItemBlock::ItemBlockType::HPCan;
						std::string str = etc[1];
						cpyString(ptr->flag_id, str);
					}
					else if (etc[0] == "mpcan")
					{
						ptr->item_type = ItemBlock::ItemBlockType::MPCan;
						std::string str = etc[1];
						cpyString(ptr->flag_id, str);
					}
				}

			}
			//アイテムボックス　乗れないやつ
			else if ((refx == REFPOINT_DUSTBOX_X && refy == REFPOINT_DUSTBOX_Y)
				|| (refx == REFPOINT_DUSTBOX_X && refy == REFPOINT_DUSTBOX_Y)
				|| (refx == REFPOINT_POT_X && refy == REFPOINT_POT_Y)
				|| (refx == REFPOINT_POT_X && refy == REFPOINT_POT_Y + 32))
			{
				ItemBlock* ptr = gfw::generate<ItemBlock>(*tl);
				ptr->initialize(refx, refy, gmtr::Vector2D(x, y), -1, 0);
				ptr->item_type = ItemBlock::ItemBlockType::Empty;
				ptr->is_solid = false;
				if (tokens.size() > 7)
				{
					auto etc = myutil::split(tokens[7], ",");
					if (etc[0] == "coin")
					{
						ptr->item_type = ItemBlock::ItemBlockType::Coin;
						cpyString(ptr->flag_id, "");
					}
					else if (etc[0] == "card")
					{
						ptr->item_type = ItemBlock::ItemBlockType::CommonCard;
						int v = myutil::str2int(etc[1]);
						ptr->item_value = v;
						cpyString(ptr->flag_id, "");
					}
					else if (etc[0] == "hpcan")
					{
						ptr->item_type = ItemBlock::ItemBlockType::HPCan;
						std::string str = etc[1];
						cpyString(ptr->flag_id, str);
					}
					else if (etc[0] == "mpcan")
					{
						ptr->item_type = ItemBlock::ItemBlockType::MPCan;
						std::string str = etc[1];
						cpyString(ptr->flag_id, str);
					}
					else if (etc[0] == "lever")
					{
						ptr->item_type = ItemBlock::ItemBlockType::Lever;
						std::string str = etc[1];
						cpyString(ptr->flag_id, str);
					}
				}

			}
			else if (refx == REFPOINT_COVER_X && refy == REFPOINT_COVER_Y)
			{
				y += 1;
				Block* ptr = gfw::generate<Block>(*tl);
				ptr->initialize(REFPOINT_COVER_X, REFPOINT_COVER_Y, gmtr::Vector2D(x, y), -1, 0.0);
				if (tokens.size() > 7)
				{
					auto etc = myutil::split(tokens[7], ",");
				}
			}
			else if (refx == REFPOINT_BLOCK_X && refy == REFPOINT_BLOCK_Y)
			{

				y += 1;
				Block* ptr = gfw::generate<Block>(*tl);
				ptr->initialize(REFPOINT_BLOCK_X, REFPOINT_BLOCK_Y, gmtr::Vector2D(x, y), -1, 1.0, 24);
			}
			else if (refx == REFPOINT_BLOCK_X + 32 && refy == REFPOINT_BLOCK_Y)
			{

				y += 1;
				Block* ptr = gfw::generate<Block>(*tl);
				ptr->initialize(REFPOINT_BLOCK_X + 32, REFPOINT_BLOCK_Y, gmtr::Vector2D(x, y), -1, 1.0, 10);
			}
			else if (refx == REFPOINT_VENDOR_X && refy == REFPOINT_VENDOR_Y 
				|| refx == REFPOINT_VENDOR_X + 32 && refy == REFPOINT_VENDOR_Y)
			{
				//y += 1;
				Block* ptr = gfw::generate<Block>(*tl);
				ptr->initialize(refx, refy, gmtr::Vector2D(x, y), -1, 1.0, 10);
				ptr = gfw::generate<Block>(*tl);
				ptr->initialize(refx, refy + 32, gmtr::Vector2D(x, y + 31), -1, 1.0, 10);
			}
			else if ((refx == REFPOINT_BARRICADE_X && refy == REFPOINT_BARRICADE_Y))
			{

				y += 1;
				Block* ptr = gfw::generate<Block>(*tl);
				ptr->initialize(REFPOINT_BARRICADE_X + 32, REFPOINT_BARRICADE_Y, 
					gmtr::Vector2D(x, y), -1, 1.0, 99999);
			}
			else if (refx == REFPOINT_GACHA_X && refy == REFPOINT_GACHA_Y)
			{

				y += 1;
				Gacha* ptr = gfw::generate<Gacha>(*tl);
				ptr->initialize(refx, refy, gmtr::Vector2D(x, y), -1, 1.0);
			}
			else if (refx == REFPOINT_TOILET_X && refy == REFPOINT_TOILET_Y)
			{

				y += 0;
				x += 4;
				
				ItemBlock* ptr = gfw::generate<ItemBlock>(*tl);
				ptr->initialize(refx, refy, gmtr::Vector2D(x, y), -1, 32);
				ptr->item_type = ItemBlock::ItemBlockType::Empty;
				if (tokens.size() > 7)
				{
					auto etc = myutil::split(tokens[7], ",");
					if (etc[0] == "coin")
					{
						ptr->item_type = ItemBlock::ItemBlockType::Coin;
						cpyString(ptr->flag_id, "");
					}
					else if (etc[0] == "card")
					{
						ptr->item_type = ItemBlock::ItemBlockType::CommonCard;
						int v = myutil::str2int(etc[1]);
						ptr->item_value = v;
						cpyString(ptr->flag_id, "");
					}
					else if (etc[0] == "hpcan")
					{
						ptr->item_type = ItemBlock::ItemBlockType::HPCan;
						std::string str = etc[1];
						cpyString(ptr->flag_id, str);
					}
					else if (etc[0] == "mpcan")
					{
						ptr->item_type = ItemBlock::ItemBlockType::MPCan;
						std::string str = etc[1];
						cpyString(ptr->flag_id, str);
					}
				}
			}
			else if ((refx == REFPOINT_STEP_X && refy == REFPOINT_STEP_Y)
				|| (refx == REFPOINT_LIFT_X && refy == REFPOINT_LIFT_Y))
			{
				y += 1;
				std::string flag_id = "";
				double vx = 0, vy = 0, period = 0;
				if (tokens.size() > 7)
				{
					auto etc = myutil::split(tokens[7], ",");
					if (etc.size() >= 3)
					{
						vx = myutil::str2double(etc[0]);
						vy = myutil::str2double(etc[1]);
						period = myutil::str2int(etc[2]);
					}
					if (etc.size() >= 4)
					{
						flag_id = etc[3];
					}
					else flag_id = "";
				}
				//om.pushObject(x, y, "jewel");
				MovingFloor* ptr = gfw::generate<MovingFloor>(*tl);
				ptr->initialize(refx, refy, gmtr::Vector2D(x, y)
					, gmtr::Vector2D(vx, vy) , period);
				cpyString(ptr->flag_id, flag_id);

			}
			else if (refx == REFPOINT_STEP2_X && refy == REFPOINT_STEP2_Y)
			{
				y += 1;
				int vx = 0, vy = 0, period = 120, delay = 0;
				std::string flag_id;
				if (tokens.size() > 7)
				{
					auto etc = myutil::split(tokens[7], ",");
					if (etc.size() >= 1)
					{
						period = myutil::str2int(etc[0]);
					}
					if (etc.size() >= 2)
					{
						delay = myutil::str2int(etc[1]);
						//flag_id = myutil::str2int(etc[3]);
					}
					if (etc.size() >= 3)
					{
						flag_id = etc[2];
					}
					else flag_id = "";
				}
				//om.pushObject(x, y, "jewel");
				DisappearingFloor* ptr = gfw::generate<DisappearingFloor>(*tl);
				ptr->initialize(REFPOINT_STEP2_X, REFPOINT_STEP2_Y, gmtr::Vector2D(x, y)
					, gmtr::Vector2D(vx, vy), period, delay);
				cpyString(ptr->flag_id, flag_id);

			}
			else if (refx == REFPOINT_CONVEYER_X && refy == REFPOINT_CONVEYER_Y ||
				refx == REFPOINT_CONVEYER_X + 32 && refy == REFPOINT_CONVEYER_Y ||
				refx == REFPOINT_CONVEYER_X + 64 && refy == REFPOINT_CONVEYER_Y)
			{
				y += 1;
				Conveyer* ptr = gfw::generate<Conveyer>(*tl);
				char speed = 1;
				if (tokens.size() > 7)
				{
					auto etc = myutil::split(tokens[7], ",");
					speed = myutil::str2int(etc[0]);

					if (etc.size() > 2) 
					{
						std::string fid = tokens[7];
						cpyString(ptr->flag_id, fid);
						$P.flags[fid] = 0;
					}
				}
				ptr->initialize(refx, refy, gmtr::Vector2D(x, y), speed);
				std::string flag_id = "";
				
			}
			else if (refx == REFPOINT_SPRING_X && refy == REFPOINT_SPRING_Y)
			{

				Trampoline* ptr = gfw::generate<Trampoline>(*tl);
				ptr->initialize(REFPOINT_SPRING_X, REFPOINT_SPRING_Y,  gmtr::Vector2D(x, y), -1);

			}
			else if (refx == REFPOINT_BOAT_X && refy == REFPOINT_BOAT_Y)
			{

				y += 20;
				FlyingCarpet* ptr = gfw::generate<FlyingCarpet>(*tl);
				ptr->initialize(REFPOINT_BOAT_X, REFPOINT_BOAT_Y, gmtr::Vector2D(x, y), gmtr::Vector2D(0, 0)
					, 0.0, FlyingCarpet::eVehicleType::Boat);
			}
			else if (refx == REFPOINT_CARPET_X && refy == REFPOINT_CARPET_Y)
			{
				y -= 64;
				FlyingCarpet* ptr = gfw::generate<FlyingCarpet>(*tl);
				ptr->initialize(REFPOINT_CARPET_X, REFPOINT_CARPET_Y, gmtr::Vector2D(x, y), gmtr::Vector2D(0, 0)
					, 0.0, FlyingCarpet::eVehicleType::Flying);
			}
			else if (refx == REFPOINT_ENEMYGROUP_X && refy == REFPOINT_ENEMYGROUP_Y)
			{
				//if ($P.num_boss3_beaten == 0)
				{
					stg.bossp.x = x;
					stg.bossp.y = y;
				}

				if (file_name == "map\\downtown6.txt"
					&& !$P.getFlag("nbossb4"))
				{
					BigEyeDummy* ptr = gfw::generate<BigEyeDummy>(*tl);
					ptr->initialize(0, 96, 96, 96, gmtr::Vector2D(x - 16, y), 0);
				}

			}
		}
		else if (verb == "background")
		{
			background = tokens[1];
		}
		else if (verb == "bg_translate_rate")
		{
			bg_translate_rate_x = bg_translate_rate_y
				= myutil::str2double(tokens[1]);
		}
		else if (verb == "bg_translate_rate_x")
		{
			bg_translate_rate_x	= myutil::str2double(tokens[1]);
		}
		else if (verb == "bg_translate_rate_y")
		{
			bg_translate_rate_y	= myutil::str2double(tokens[1]);
		}
		else if (verb == "bg_offset_x"){ bg_offset_x = myutil::str2int(tokens[1]); }
		else if (verb == "bg_offset_y"){ bg_offset_y = myutil::str2int(tokens[1]); }
		else if (verb == "bg_image_width"){ bg_image_width = myutil::str2int(tokens[1]); }
		else if (verb == "bg_image_height"){ bg_image_height = myutil::str2int(tokens[1]); }
		else if (verb == "bg_x_min"){ bg_x_min = myutil::str2int(tokens[1]); }
		else if (verb == "bg_x_max"){ bg_x_max = myutil::str2int(tokens[1]); }
		else if (verb == "bg_y_min"){ bg_y_min = myutil::str2int(tokens[1]); }
		else if (verb == "bg_y_max"){ bg_y_max = myutil::str2int(tokens[1]); }
		else if (verb == "bgm")
		{
			bgm = tokens[1];
			if ($P.bgm_name != bgm)
				$P.reset_bgm = true;
			$P.bgm_name = bgm;


			if (tokens.size() >= 3)
			{
				$P.bgm_segno = myutil::str2double(tokens[2]);
			}
			if (tokens.size() >= 4)
			{
				$P.bgm_start = myutil::str2double(tokens[3]);
			}
		}
	}

	if (bgm.size() == 0)
	{
		$P.reset_bgm = true;
		$P.bgm_name = bgm;
	}

	if (bg)
	{
		bg->offset_x = bg_offset_x;
		bg->offset_y = bg_offset_y;
		bg->img_width = bg_image_width;
		bg->img_height= bg_image_height;
		bg->translate_rate_x = bg_translate_rate_x;
		bg->translate_rate_y = bg_translate_rate_y;
		bg->offset_x_min = bg_x_min;
		bg->offset_x_max = bg_x_max;
		bg->offset_y_min = bg_y_min;
		bg->offset_y_max = bg_y_max;
		if (background.size() > 0)
		{
			//stg.generateRainDrops();
			if (background == "school")
			{
				if ($P.getFlag("stg4c"))
					bg->img = $P.img_school_n;
				else if ($P.getFlag("stg3c") || $P.getFlag("stg32c"))
					bg->img = $P.img_school_t;
				else
					bg->img = $P.img_school;
			}
			if (background == "dojo")
				bg->img = $P.img_dojo;
			if (background == "sky")
			{
				stg.generateClouds();
				if ($P.getFlag("stg4c"))
					bg->img = $P.img_sky_n;
				else if($P.getFlag("stg3c") || $P.getFlag("stg32c"))
					bg->img = $P.img_sky_t;
				else
					bg->img = $P.img_sky;
			}
			if (background == "ground")
			{
				if ($P.getFlag("stg4c"))
					bg->img = $P.img_ground_n;
				else if ($P.getFlag("stg3c") || $P.getFlag("stg32c"))
					bg->img = $P.img_ground_t;
				else
					bg->img = $P.img_ground;
				stg.generateClouds();
			}
			if (background == "river")
			{
				if ($P.getFlag("stg4c"))
					bg->img = $P.img_sky_n;
				else if ($P.getFlag("stg3c") || $P.getFlag("stg32c"))
					bg->img = $P.img_sky_t;
				else
					bg->img = $P.img_sky;
				stg.generateClouds();
				stg.generateBirds();
			}
			if (background == "cave")
				bg->img = $P.img_cave;
			if (background == "forest")
			{
				if ($P.getFlag("stg4c"))
					bg->img = $P.img_forest_n;
				else if ($P.getFlag("stg3c") || $P.getFlag("stg32c"))
					bg->img = $P.img_forest_t;
				else
					bg->img = $P.img_forest;
			}
			if (background == "downtown")
			{
				stg.generateClouds();
				bg->img = $P.img_downtown;
				if ($P.getFlag("stg4c"))
					bg->img = $P.img_downtown_n;
				else
					bg->img = $P.img_downtown;
			}
			if (background == "garden")
				bg->img = $P.img_garden;
			if (background == "mansion")
				bg->img = $P.img_mansion;
			if(background == "room")
				bg->img = $P.img_room;

		}
	}

	if (file_name == "map\\mansion6.txt")
		stg.generateRainDrops();

	if (file_name == "map\\boss_room1.txt")
		$P.stageselect_cursor = 0;
	else if (file_name == "map\\ground1.txt")
		$P.stageselect_cursor = 1;
	else if (file_name == "map\\forest2.txt")
		$P.stageselect_cursor = 2;
	else if (file_name == "map\\river1.txt")
		$P.stageselect_cursor = 5;
	else if (file_name == "map\\downtown1.txt")
		$P.stageselect_cursor = 3;
	else if (file_name == "map\\garden1.txt")
		$P.stageselect_cursor = 4;
	else if (file_name == "map\\mansion4.txt")
		$P.stageselect_cursor = 6;
	else if (file_name == "map\\boss_room1.txt")
		$P.stageselect_cursor = 0;
	else if (file_name == "map\\boss_room2.txt")
		$P.stageselect_cursor = 1;
	else if (file_name == "map\\boss_room3.txt")
		$P.stageselect_cursor = 2;
	else if (file_name == "map\\boss_room6.txt")
		$P.stageselect_cursor = 5;
	else if (file_name == "map\\boss_room4.txt")
		$P.stageselect_cursor = 3;
	else if (file_name == "map\\boss_room5.txt")
		$P.stageselect_cursor = 4;

	if ($P.getFlag("lastrun") && file_name == "map\\boss_room5.txt")
	{
		stg.curtainr = 255;
		stg.curtaing = 0;
		stg.curtainb = 0;
		stg.curtaina = 64;
	}
	
	return true;
}

void StageDataLoader::createPlainStage(int width, int height, gfw::Tasklist* tl
	, Stage& stg, gfw::ObjectManager& om, Background* bg)
{
	stg.width = width;
	stg.height = height;
	if (stg.chips)
		delete[] stg.chips;
	stg.chips = new Mapchip[(stg.width + 1) * (stg.height + 1)];
	for (int x = 0; x < stg.width; x++)
	{
		for (int y = 0; y < stg.height; y++)
		{
			Mapchip& c = stg.chips[x / Mapchip::WIDTH + y / Mapchip::HEIGHT * stg.width];
			c.x = x;
			c.y = y;
			c.refx = -1;
			c.refy = -1;
			c.type = 'n';
			c.w = Mapchip::WIDTH;
			c.h = Mapchip::HEIGHT;
		}
	}
	for (int x = 0; x < stg.width; x++)
	{
		int y = stg.height - 1;
		Mapchip& c = stg.chips[x / Mapchip::WIDTH + y / Mapchip::HEIGHT * stg.width];
		c.x = x;
		c.y = y;
		c.refx = -1;
		c.refy = -1;
		c.type = 'f';
		c.w = Mapchip::WIDTH;
		c.h = Mapchip::HEIGHT;
	}
}