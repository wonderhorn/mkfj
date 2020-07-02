#define _USE_MATH_DEFINES
#include<cmath>
#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/mover/Item.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/GRAPHICS.h"
#include"jumpnrun/spell/Spell.h"
#include"jumpnrun/spell/SpellName2Text.h"
#include"utils/string_util.h"
using namespace jnr;
using namespace gfw;
using namespace gmtr;
using std::max;
using std::min;
#define $V gmtr::Vector2D

RegisterMoverClass(Item);
RegisterMoverClass(EncountBoss);
RegisterMoverClass(EventCaller);
RegisterMoverClass(GetCard);
RegisterMoverClass(HPCan);
RegisterMoverClass(MPCan);
RegisterMoverClass(NextStage);
RegisterMoverClass(DeckReset);

static void cpyString(char buf[JNR_FLAG_SIZE], const std::string& str)
{
	assert(str.size() < JNR_FLAG_SIZE);
	for (int i = 0; i < str.size(); i++)
	{
		buf[i] = str[i];
	}
	buf[JNR_FLAG_SIZE - 1] = '\0';
}

void Item::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v
	, double mass, const std::string& flag_id)
{
	Bullet::initialize(refx, refy, p, owner, power, v, mass);
	num_bounding = -1;
	touch_and_dissapear = false;
	lifetime = 60 * 10;
	rotate_to_direction = false;
	hp_gain = 0;
	mp_gain = 10;
	score_gain = 10;
	is_silent = false;
	timer_show_down_button = 0;
	cpyString(this->flag_id, flag_id.c_str());
	writeName("item", name);
}

void Item::run(jnr::JumpnRunData& data)
{
	if (timer_show_down_button > 0)
		timer_show_down_button--;
	if (phys.v.x > 0)
	{
		phys.v.x -= 0.05;
		if (phys.v.x < 0)
			phys.v.x = 0;
	}
	else if (phys.v.x < 0)
	{
		phys.v.x += 0.05;
		if (phys.v.x > 0)
			phys.v.x = 0;
	}
	Bullet::run(data);
}

void Item::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	if (refx < 0)
		return;
	if (refx == REFPOINT_HEART_X && refy == REFPOINT_HEART_Y)
	{
		int blink = (age / 6 % 10) == 1;
		graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
			, refx, refy + blink * height, width, height, turned);
	}
	else if (refx == REFPOINT_MAGICSTONE_X && refy == REFPOINT_MAGICSTONE_Y + 64)
	{
		int blink = (age / 16 % 2);
		graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
			, refx + width * blink, refy, width, height, turned);

		for (int i = 0; i < 2; i++)
		{
			double theta = (double)(age + 30 * i % 90) / 90 * 2 * M_PI;
			double star_x = 32 * cos(theta);
			double star_y = 16 * sin(theta);
			graphics->draw($P.img_obj,
				phys.p.x + star_x + 8 - data.camera.X(),
				phys.p.y + star_y + 8 - data.camera.Y(), 0
				, REFPOINT_MINISTAR_X + 16, REFPOINT_MINISTAR_Y, 16, 16, turned);
		}

	}
 	else if (refx == REFPOINT_JEWEL_X && refy == REFPOINT_JEWEL_Y + 32)
	{
		int blink = (age / 6 % 10) == 1;
		if (!blink)
			graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
			, refx, refy, width, height, turned);
		else
			graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
			, refx, refy - 32, width, height, turned);
	}
	else
		graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
			, refx, refy, width, height, turned);

	if (timer_show_down_button > 0)
	{
		if((age % 60) < 40)
			graphics->drawString("▼", 
				phys.p.x + 8 - data.camera.X(), phys.p.y + height - data.camera.Y(), 
				24, 24, 0, 1, 0, 1);
	}
}


void Item::interact(jnr::Character& chara , jnr::JumpnRunData& data)
{
	$V center = phys.p + $V(width / 2, height / 2);
	if (chara.Shape().includes(center) && chara.interactingWithItems())
	{
		if (!is_silent)
		{
			generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, phys.p, 2, $V(0.0, 0.0), 0);
			generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, phys.p, 4, $V(0.0, 0.0), 0);
			$P.sound_stack.push_back($P.snd_coin);
			chara.HP() += hp_gain;
			chara.Score() += score_gain;
			chara.MP() += mp_gain;
			if (chara.HP() > chara.HPMax())
				chara.HP() = chara.HPMax();
			if (chara.MP() > chara.MPMax())
				chara.MP() = chara.MPMax();
		}
		if (this->flag_id[0] > 0)
			$P.flags[flag_id] = 1;

		finalize();
	}
}


void HPCan::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, double mass, const std::string& flag_id)
{
	Bullet::initialize(refx, refy, p, owner, power, $V(0, 0), mass);
	num_bounding = -1;
	touch_and_dissapear = false;
	lifetime = 60 * 10;
	rotate_to_direction = false;
	cpyString(this->flag_id, "");
	hp_gain = 10;
	mp_gain = 0;
	score_gain = 10;
	writeName("item_hpcan", name);
}

void HPCan::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	$V center = phys.p + $V(width / 2, height / 2);
	if (chara.Shape().includes(center) && chara.interactingWithItems())
	{
		generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, phys.p, 2, $V(0.0, 0.0), 0);
		generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, phys.p, 4, $V(0.0, 0.0), 0);
		$P.sound_stack.push_back($P.snd_powerup);
		chara.HP() += hp_gain;
		chara.HPMax() += hp_gain;
		if (chara.HP() > chara.HPMax())
			chara.HP() = chara.HPMax();

		if(this->flag_id[0] > 0)
			$P.flags[flag_id] = 1;

		finalize();
	}
}

void MPCan::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, double mass, const std::string& flag_id)
{
	Bullet::initialize(refx, refy, p, owner, power, $V(0, 0), mass);
	num_bounding = -1;
	touch_and_dissapear = false;
	lifetime = 60 * 10;
	rotate_to_direction = false;
	cpyString(this->flag_id, "");
	hp_gain = 0;
	mp_gain = 10;
	score_gain = 10;
	writeName("item_mpcan", name);
}

void MPCan::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	$V center = phys.p + $V(width / 2, height / 2);
	if (chara.Shape().includes(center) && chara.interactingWithItems())
	{
		generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, phys.p, 2, $V(0.0, 0.0), 0);
		generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, phys.p, 4, $V(0.0, 0.0), 0);
		$P.sound_stack.push_back($P.snd_powerup);
		chara.MP() += mp_gain;
		chara.MPMax() += mp_gain;
		if (chara.MP() > chara.MPMax())
			chara.MP() = chara.MPMax();

		if (this->flag_id[0] > 0)
			$P.flags[flag_id] = 1;

		finalize();
	}
}


void DeckReset::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v, double mass)
{
	Item::initialize(refx, refy, p, owner, power, v, mass);
	touch_and_dissapear = false;
	lifetime = INT_MAX;
	this->rotate_to_direction = false;
	writeName("item_deckreset", name);
}

void DeckReset::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	$V center = phys.p + $V(width / 2, height / 2);
	if (chara.Shape().includes(center))
	{
		if (chara.Name() == "character_player")
			timer_show_down_button = 12;
		chara.onItem();
		if (chara.pickingItem())
		{
			$P.sound_stack.push_back($P.snd_card);
			Effect* e = gfw::generate<Effect>(*chara.tl);
			e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, Center() + $V(-16, -16.0)
				, 10, $V(0, 0), 0, false, 2.0, -0.1, 1, 16);
			chara.resetDeck();
		}
	}
}

void DeckReset::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int offset_y = min(age % 90, 90 - (age % 90)) / 9 + 4;
	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - offset_y - data.camera.Y(), 0
		, refx, refy, width, height, turned);

	if (timer_show_down_button > 0)
	{
		if ((age % 60) < 40)
			graphics->drawString("▼",
				phys.p.x + 8 - data.camera.X(), phys.p.y + height - data.camera.Y(),
				24, 24, 0, 1, 0, 1);
	}
}


void NextStage::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, double mass
	, const std::string& stage_name, int start_x, int start_y)
{
	Item::initialize(refx, refy, p, owner, power, $V(0, 0), mass);
	touch_and_dissapear = false;
	lifetime = INT_MAX;
	this->rotate_to_direction = false;
	this->start_x = start_x;
	this->start_y = start_y;
	writeName(stage_name.c_str(), this->stage_name);
	writeName("item_nextstage", name);
}

void NextStage::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	$V center = phys.p + $V(width / 2, height / 2);
	if (chara.Shape().includes(center))
	{
		if (chara.Name() == "character_player")
			timer_show_down_button = 12;
		chara.onItem();
		if (chara.pickingItem())
		{
			if (this->flag_id[0] > 0)
				$P.flags[flag_id] = 1;
			data.change_stage = 0;
			data.next_stage_name = stage_name;
			data.next_start_p = $V(start_x, start_y);
			chara.pickingItem(false);
			chara.enterDoor();
			$P.sound_stack.push_back($P.snd_door);
		}
	}
}


void EncountBoss::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, double mass
	, int boss_id, const std::string& flag_id, const std::string& flag_id_activate)
{
	Item::initialize(refx, refy, p, owner, power, $V(0, 0), mass);
	touch_and_dissapear = false;
	lifetime = INT_MAX;
	this->rotate_to_direction = false;
	this->boss_id = boss_id;
	cpyString(this->flag_id, flag_id);
	cpyString (this->flag_id_activate, flag_id_activate);
	writeName("item_encountboss", name);
	if ($P.getFlag("lastrun") && this->boss_id != 10)
		alive = false;
}

void EncountBoss::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	if (this->flag_id_activate[0] != 0 && !$P.getFlag(this->flag_id_activate))
	{
		return;
	}
	if (data.encount_boss == this->boss_id)
	{
		alive = false;
		return;
	}


	$V center = phys.p + $V(width / 2, height / 2);
	if (chara.Shape().includes(center))
	{
		/*chara.onItem();
		if (chara.pickingItem())
		{
			$P.change_stage = 0;
		}*/
		if (chara.Name() == "character_player")
		{
			this->alive = false;
			data.encount_boss = this->boss_id;
		}
	}
}


void EventCaller::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, double mass
	, const std::string& flag_id)
{
	Item::initialize(refx, refy, p, owner, power, $V(0, 0), mass);
	touch_and_dissapear = false;
	cpyString(this->flag_id, flag_id);

	lifetime = INT_MAX;
	if ($P.getFlag("flag_id"))
		alive = false;

	phys.p.x -= 12;
	phys.p.y -= 12;
	width = 56;
	height = 56;
	
	writeName("item_gameclear", name);
}

void EventCaller::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	
	$V center = phys.p + $V(width / 2, height / 2);
	if (chara.Shape().includes(center))
	{
		/*chara.onItem();
		if (chara.pickingItem())
		{
		$P.change_stage = 0;
		}*/
		if (chara.Name() == "character_player")
		{
			this->alive = false;
			if (this->flag_id[0] > 0)
				$P.flags[flag_id] = 1;
			//data.players[0].vc->force_to_release = true;
		}
	}
}


void GetCard::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, double mass, int card_id, const std::string& flag_id)
{
	Item::initialize(refx, refy, p, owner, power, $V(0, 0), mass);
	touch_and_dissapear = false;
	lifetime = INT_MAX;
	this->rotate_to_direction = false;
	this->card_id = card_id;
	cpyString(this->flag_id, flag_id);
	writeName("item_getcard", name);
}
void GetCard::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	$V center = phys.p + $V(width / 2, height / 2);
	if (chara.Shape().includes(center))
	{
		if (chara.Name() == "character_player")
		{
			$P.sound_stack.push_back($P.snd_card);
			if (this->flag_id[0] > 0)
				$P.flags[flag_id] = 1;
			this->alive = false;
			//$P.encount_boss = this->boss_id;
			if (card_id >= 0 && card_id < spl::Spell::spells.size())
			{
				data.message = "スペル「" 
					+ spl::SpellName2Text::Name(spl::Spell::spells[this->card_id].name) 
					+ "」をひろった！";
				if ($P.mylibrary[this->card_id])
					$P.mylibrary[this->card_id]++;
				else
					$P.mylibrary[this->card_id] = 1;
			}
			else
			{
				data.message = "スペル「？？？」をひろった！";
			}
			data.message_time = 60 * 3;
			chara.guts();
			chara.changeCurrentCard(this->card_id);
			if ($P.to_save)
				$P.save($P.save_file_name);
		}
	}
}
void GetCard::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	{
		BaseGraphics* graphics = media.getGraphics();
		auto v = this->V();
		bool t = (v.x < 0);
		//v = $V(fabs(v.x), v.y);
		double theta = 0;

		int phase = age % 120;
		double rate = 1.0;
		if (phase < 30)
		{
			double phi = M_PI / 2 * phase / 30;
			rate = fabs(cos(phi));
			graphics->drawSpinResize($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
				, refx + 1, refy, width - 1, height, t, 0xffffffff, rate, 1.0, theta);

		}
		else if (phase < 90)
		{
			double phi = M_PI / 2 * phase / 30;
			rate = fabs(cos(phi));
			graphics->drawSpinResize($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
				, refx + 1, refy + 32, width - 1, height, t, 0xffffffff, rate, 1.0, theta);
		}
		else
		{
			double phi = M_PI / 2 * phase / 30;
			rate = fabs(cos(phi));
			graphics->drawSpinResize($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
				, refx + 1, refy, width - 1, height, t, 0xffffffff, rate, 1.0, theta);
		}
	}
}
