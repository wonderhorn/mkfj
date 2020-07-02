#define _USE_MATH_DEFINES
#include<cmath>
#include"jumpnrun/spell/Spell.h"
#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/mover/enemy/Enemy.h"
#include"jumpnrun/MoverUtilities.h"
#include"jumpnrun/mover/bullet/Bullet.h"
#include"jumpnrun/mover/Barrier.h"
#include"jumpnrun/mover/bullet/Bubble.h"
#include"jumpnrun/mover/Enchant.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/mover/effect/Circle.h"
#include"jumpnrun/mover/StageAccessor.h"
#include"jumpnrun/GRAPHICS.h"
#include"utils/MyCppUtil.h"
#define $V gmtr::Vector2D
using namespace jnr;
using namespace gmtr;
using namespace spl;

std::vector<Spell> Spell::spells =
{
	/*
	int manacost;
	int anim_type, anim_num;
	int stuntime, frame_of_one_phase, actiontime, num_iteration, final_frame_length;*/
	{ 15, 6, 2, 60, 14, 45, 2, 0, spl_circle_explosion, 0, 0, 'c', 1, 24, "explosion" },//0
	{ 5, 6, 2, 30, 6, 15, 2, 0, spl_circle_bubble, 32 * 2, 32 * 5, 'c', 11, 1, "bubble" },
	{ 10, 8, 2, 44, 20, 24, 1, 0, spl_bullet, 32 * 11, 0, 'c', 3, 5, "bullet" },
	{ 15, 6, 2, 45, 14, 35, 2, 0, spl_circle_gas, 64, 0, 'c', 7, 1, "gas" },
	{ 15, 17, 1, 50, 48, 25, 1, 0, spl_lifegain20, 96, 0, 'c', 5, 0, "lifegain20" },
	{ 25, 17, 1, 18, 18, 5, 1, 0, spl_shuffle, 64, 32, 'r', 37, 0, "shuffle" },
	{ 12, 17, 1, 30, 30, 20, 1, 0, spl_barrier, 32 * 6, 0, 'c', 14, 0, "barrier" },
	{ 10, 6, 2, 60, 15, 55, 2, 0, spl_circle_summon_frog, 32 * 4, 0, 'c', 4, 0, "frog" },
	{ 10, 12, 1, 40, 40, 5, 1, 0, spl_dashattack, 32 * 5, 0, 'c', 6, 15, "dashattack" },
	{ 15, 10, 2, 45, 20, 22, 1, 0, spl_flame, 32 * 8, 0, 'c', 46, 20, "flame" },
	{ 18, 8, 2, 50, 22, 28, 1, 0, spl_bomb, 32 * 9, 0, 'c', 9, 12, "bomb" },//10
{ 15, 10, 2, 20, 10, 14, 1, 0, spl_missile, 32 * 10, 0, 'c', -1, 12, "missile" },
{ 22, 10, 2, 100, 45, 50, 1, 0, spl_homingmissile, 32 * 6, 32, 'u', 39, 12, "homingmissile" },
{ 15, 10, 2, 40, 15, 18, 1, 0, spl_laser, 32, 32, 'u', 25, 18, "laser" },
{ 15, 10, 2, 40, 15, 18, 1, 0, spl_laser2, 32 * 14, 0, 'u', -1, 18, "laser2" },
{ 15, 8, 2, 40, 15, 18, 1, 0, spl_laserslash, 32 * 11, 32, 'r', -1, 12, "laserslash" },
{ 9, 8, 2, 40, 15, 18, 1, 0, spl_starshoot, 0, 32, 'c', 2, 10, "starshoot" },
{ 12, 10, 2, 60, 30, 28, 1, 40, spl_starstream, 0, 32, 'c', -1, 10, "starstream" },
{ 10, 8, 2, 20, 8, 14, 1, 0, spl_wind, 32 * 7, 32, 'c', -1, 0, "wind" },
{ 16, 8, 2, 36, 12, 18, 1, 0, spl_thunder, 32 * 8, 32, 'c', 29, 8, "thunder" },
/*
int manacost;
int anim_type, anim_num;
int stuntime, frame_of_one_phase, actiontime, num_iteration;*/
{ 10, 8, 2, 45, 22, 26, 1, 0, spl_sword, 32 * 9, 32, 'c', 19, 15, "sword" },//20
{ 16, 17, 1, 45, 45, 16, 1, 0, spl_thunderbolt, 32 * 12, 32, 'c', 30, 8, "thunderbolt" },
{ 15, 6, 2, 60, 15, 55, 2, 0, spl_circle_summon_dirtcube, 32 * 12, 0, 'c', 40, 0, "dirtcube" },
{ 15, 6, 2, 60, 15, 55, 2, 0, spl_circle_summon_mushroom, 32 * 12, 32 * 2, 'c', 15, 0, "mushroom" },
{ 15, 6, 2, 60, 15, 55, 2, 0, spl_circle_summon_ghost, 32 * 13, 32 * 0, 'c', -1, 0, "ghost" },
{ 10, 8, 2, 40, 15, 18, 1, 0, spl_slimeball, 32 * 11, 32 * 2, 'c', 12, 5, "slimeball" },
{ 20, 10, 2, 90, 45, 55, 1, 0, spl_laser, 32, 32, 'r', 25, 18, "laser" },
{ 25, 17, 1, 80, 75, 55, 1, 0, spl_lifegain30, 32 * 2, 32 * 2, 'u', 23, 0, "lifegain30" },
{ 2, 17, 1, 65, 60, 30, 1, 0, spl_life2mana20, 32 * 6, 32 * 3, 'c', 38, 0, "life2mana20" },
{ 15, 17, 1, 65, 60, 45, 1, 0, spl_muscle, 32 * 0, 32 * 3, 'u', 21, 0, "muscle" },
{ 10, 8, 2, 28, 12, 12, 1, 0, spl_leafshoot, 32 * 13, 32, 'c', 20, 0, "leafshoot" },//30
{ 5, 6, 2, 60, 15, 55, 2, 0, spl_circle_summon_manaflower, 32 * 11, 32 * 3, 'u', 22, 0, "manaflower" },
{ 20, 17, 1, 18, 18, 15, 1, 0, spl_healbarrier, 32 * 10, 32 * 2, 'r', -1, 0, "healbarrier" },
{ 20, 6, 2, 60, 15, 55, 2, 0, spl_circle_summon_candle, 32 * 14, 32 * 3, 'c', -1, 0, "candle" },
{ 18, 10, 2, 60, 30, 34, 1, 0, spl_homingfire, 32 * 1, 32 * 4, 'u', 16, 12, "homingfire" },
{ 18, 8, 2, 45, 25, 28, 1, 0, spl_bubblestream, 32 * 12, 32 * 3, 'u', 31, 1, "bubblestream" },//35
{ 18, 8, 2, 45, 25, 28, 1, 0, spl_whip, 32 * 8, 32 * 3, 'u', 32, 10, "whip" },
{ 13, 8, 2, 40, 20, 30, 1, 0, spl_sonicboom, 32 * 14, 32 * 2, 'c', 26, 8, "sonicboom" },
{ 22, 8, 2, 60, 30, 35, 1, 0, spl_sword3, 32 * 14, 32, 'u', 45, 8, "sword3" },
{ 5, 8, 2, 40, 20, 24, 1, 0, spl_arrow, 32 * 5, 32 * 4, 'c', 18, 8, "arrow" },
{ 14, 8, 2, 45, 12, 18, 1, 20, spl_whip, 32 * 8, 32 * 3, 'u', 32, 10, "whip" }, //40
/*
 int manacost;
 int anim_type, anim_num;
 int stuntime, frame_of_one_phase, actiontime, num_iteration;*/
{ 5, 6, 2, 30, 6, 15, 2, 0, spl_circle_barrier,  32 * 13, 32 * 2, 'c', 14, 0, "barrier" },
{ 25, 6, 2, 60, 15, 55, 2, 0, spl_circle_summon_elf,  32 * 7, 32 * 4, 'u', 24, 0, "elf" },
{ 20, 6, 2, 60, 14, 45, 2, 0, spl_circle_trap_explosion, 32 * 3, 32 * 5, 'u', 34, 25, "trapexp" },
{ 15, 8, 2, 45, 18, 22, 1, 0, spl_scatterurchin, 32 * 3, 32 * 4, 'u', 27, 5, "scatterurchin" },
{ 20, 17, 1, 65, 60, 45, 1, 0, spl_speedup, 32 * 9, 32 * 4, 'r', 10, 0, "speedup" }, // 5
{ 25, 6, 2, 60, 15, 55, 2, 0, spl_circle_summon_cannon, 32 * 4, 32 * 1, 'u', 43, 0, "cannon" },
{ 19, 6, 2, 60, 14, 45, 2, 0, spl_circle_summon_bombbox, 32 * 1, 32 * 3, 'u', 13, 0, "bombox" },
{ 24, 17, 1, 65, 60, 45, 1, 0, spl_wing, 32 * 8, 32 * 2, 'r', 47, 0, "wing" },
{ 22, 6, 2, 60, 15, 55, 2, 0, spl_circle_summon_knight, 32 * 11, 32 * 4, 'u', 44, 0, "knight" },
{18, 10, 2, 60, 30, 34, 1, 0, spl_flame, 32 * 1, 32 * 4, 'u', 46, 20, "flame" },//50
{ 35, 10, 2, 60, 30, 34, 1, 0, spl_quake, 32 * 10, 32 * 4, 'r', 36, 25, "quake" },
{ 8, 10, 2, 85, 45, 42, 1, 0, spl_hardpunch, 32 * 12, 32 * 4, 'u', 35, 30, "hardpunch" },
{ 14, 17, 1, 60, 55, 35, 1, 0, spl_circlecancel, 32 * 4, 32 * 5, 'u', 28, 0, "circlecancel" },
{ 28, 17, 1, 70, 65, 45, 1, 0, spl_darkwave, 32 * 6, 32 * 2, 'r', 33, 4, "darkwave" },
{ 13, 8, 2, 44, 20, 24, 1, 0, spl_shotgun, 32 * 4, 32 * 4, 'c', 8, 8, "shotgun" },
{ 20, 17, 1, 65, 60, 45, 1, 0, spl_concentrate, 32 * 13, 32 * 4, 'u', 17, 0, "concentrate" },
{ 32, 17, 1, 90, 75, 65, 1, 0, spl_mindcontrol, 32 * 14, 32 * 4, 'r', 42, 0, "mindcontrol" },
{ 18, 8, 2, 50, 22, 28, 1, 0, spl_gasbomb, 32 * 14, 32 * 5, 'c', 41, 2, "gasbomb" },
{ 25, 6, 2, 60, 15, 55, 2, 0, spl_circle_summon_angel, 32 * 13, 32 * 5, 's', 49, 0, "angel" },
{ 30, 17, 1, 80, 75, 55, 1, 0, spl_mphprate, 32 * 12, 32 * 5, 's', 50, 0, "mphprate" }, //60
{ 20, 6, 2, 60, 15, 55, 2, 0, spl_circle_barrier, 32 * 11, 32 * 5, 's', 48, 0, "circlebarrier" },
{ 15, 6, 2, 60, 14, 45, 2, 0, spl_arrow, 0, 32 * 4, 'c', 1, 24, "dummy" },//61

};

int jnr::spl::spellname2id(const std::string& name)
{
	for (int i = 0; i < Spell::spells.size(); i++)
	{
		if (name == Spell::spells[i].name)
			return i;
	}
	assert(false);
	return -1;
}


void(*jnr::spl::SpellFunc(jnr::spl::eSpell s))(jnr::Character& chara, jnr::JumpnRunData& data)
{
	switch (s)
	{
	default:
		assert(1);
		return nullptr;
	case eSpell::spl_flower:
		return flower;
	case eSpell::spl_punch:
		return punch;
	case eSpell::spl_punch2:
		return punch2;
	case eSpell::spl_hardpunch:
		return hardpunch;
	case eSpell::spl_quake:
		return quake;
	case eSpell::spl_arrow:
		return arrow;
	case eSpell::spl_arrow3:
		return arrow3;
	case eSpell::spl_bullet:
		return bullet;
	case eSpell::spl_bullet2:
		return bullet2;
	case eSpell::spl_shotgun:
		return shotgun;
	case eSpell::spl_bomb:
		return bomb;
	case eSpell::spl_dashattack:
		return dashattack;
	case eSpell::spl_flame:
		return flame;
	case eSpell::spl_gasbomb:
		return gasbomb;
	case eSpell::spl_miniflame:
		return miniflame;
	case eSpell::spl_fire:
		return fire;
	case eSpell::spl_homingfire:
		return homingfire;
	case eSpell::spl_homingfire3:
		return homingfire3;
	case eSpell::spl_missile:
		return missile;
	case eSpell::spl_homingmissile:
		return homingmissile;
	case eSpell::spl_laser:
		return laser;
	case eSpell::spl_laser2:
		return laser2;
	case eSpell::spl_laserslash:
		return laserslash;
	case eSpell::spl_laserslash2:
		return laserslash2;
	case eSpell::spl_volleyball:
		return volleyball;
	case eSpell::spl_wind:
		return wind;
	case eSpell::spl_thunder:
		return thunder;
	case eSpell::spl_thunderdir:
		return thunderdir;
	case eSpell::spl_starshoot:
		return starshoot;
	case eSpell::spl_starstream:
		return starstream;
	case eSpell::spl_bubblestream:
		return bubblestream;
	case eSpell::spl_sword:
		return sword;
	case eSpell::spl_sword3:
		return sword3;
	case eSpell::spl_thunderbolt:
		return thunderbolt;
	case eSpell::spl_slimeball:
		return slimeball;
	case eSpell::spl_darkwave:
		return darkwave;
	case eSpell::spl_leafshoot:
		return leafshoot;
	case eSpell::spl_sonicboom:
		return sonicboom;
	case eSpell::spl_shockwave:
		return shockwave;
	case eSpell::spl_scatterurchin:
		return scatterurchin;
	case eSpell::spl_whip:
		return whip;
	case eSpell::spl_bigeyetentacle:
		return bigeyetentacle;
	case eSpell::spl_gas:
		return gas;
	case eSpell::spl_barrier:
		return barrier;
	case eSpell::spl_healbarrier:
		return healbarrier;
	case eSpell::spl_bubblebarrier:
		return bubblebarrier;
	case eSpell::spl_wing:
		return wing;
	case eSpell::spl_willowisp2:
		return willowisp2;
	case eSpell::spl_willowispperp3:
		return willowispperp3;
	case eSpell::spl_lifegain20:
		return lifegain20;
	case eSpell::spl_lifegain30:
		return lifegain30;
	case eSpell::spl_life2mana20:
		return life2mana20;
	case eSpell::spl_mindcontrol:
		return mindcontrol;
	case eSpell::spl_muscle:
		return muscle;
	case eSpell::spl_speedup:
		return speedup;
	case eSpell::spl_mphprate:
		return mphprate;
	case eSpell::spl_shuffle:
		return shuffle;
	case eSpell::spl_recycle:
		return recycle;
	case eSpell::spl_flush:
		return flush;
	case eSpell::spl_circlecancel:
		return circle_cancel;
	case eSpell::spl_circle:
		return circle;
	case eSpell::spl_circle_explosion:
		return circle_explosion;
	case eSpell::spl_circle_bubble:
		return circle_bubble;
	case eSpell::spl_circle_barrier:
		return circle_barrier;
	case eSpell::spl_circle_gas:
		return circle_gas;
	case eSpell::spl_circle_flame:
		return circle_flame;
	case eSpell::spl_circle_flame_nearest:
		return circle_flame_nearest;
	case eSpell::spl_circle_summon_frog:
		return circle_summon_frog;
	case eSpell::spl_circle_summon_dirtcube:
		return circle_summon_dirtcube;
	case eSpell::spl_circle_summon_bombbox:
		return circle_summon_bombbox;
	case eSpell::spl_circle_summon_mushroom:
		return circle_summon_mushroom;
	case eSpell::spl_circle_summon_ghost:
		return circle_summon_ghost;
	case eSpell::spl_circle_summon_ghost2:
		return circle_summon_ghost2;
	case eSpell::spl_circle_summon_knight:
		return circle_summon_knight;
	case eSpell::spl_circle_summon_candle:
		return circle_summon_candle;
	case eSpell::spl_circle_summon_cannon:
		return circle_summon_cannon;
	case eSpell::spl_circle_summon_pumpkin:
		return circle_summon_pumpkin;
	case eSpell::spl_circle_summon_fairy_blue:
		return circle_summon_fairy_blue;
	case eSpell::spl_circle_summon_angel:
		return circle_summon_angel;
	case eSpell::spl_circle_summon_elf:
		return circle_summon_elf;
	case eSpell::spl_circle_summon_manaflower:
		return circle_summon_manaflower;
	case eSpell::spl_circle_summon_lamp:
		return circle_summon_lamp;
	case eSpell::spl_circle_summon_bigslime:
		return circle_summon_bigslime;
	case eSpell::spl_circle_trap_explosion:
		return circle_trap_explosion;
	case eSpell::spl_concentrate:
		return concentrate;
	}
}

void jnr::spl::flower(jnr::Character& chara, jnr::JumpnRunData& data)
{
	chara.fool();
}

void jnr::spl::punch(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -1.0 : 1.0;
	Bullet* b = static_cast<Bullet*>(RegisteredMoverClasses["Bullet"](chara.tl));
	b->initialize(REFPOINT_SLASH_X, REFPOINT_SLASH_Y
		, chara.Center() + $V(vx * 24 - 16, -16.0), chara.Owner(), 5 + chara.Muscle(), $V(vx * 1.5, 0), 0);
	//b->snd_hit = $P.snd_hit;
	b->LifeTime(8);
	b->Radius(8);
	$P.sound_stack.push_back($P.snd_swing);
}

void jnr::spl::punch2(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -1.0 : 1.0;
	Bullet* b = static_cast<Bullet*>(RegisteredMoverClasses["Bullet"](chara.tl));
	b->initialize(REFPOINT_SLASH_X, REFPOINT_SLASH_Y
		, chara.Center() + $V(vx * 24 - 16, -16.0), chara.Owner(), 18 + chara.Muscle(), $V(vx * 1.5, 0), 0);
	//b->snd_hit = $P.snd_hit;
	b->LifeTime(8);
	b->Radius(12);
	$P.sound_stack.push_back($P.snd_swing);
}

void jnr::spl::hardpunch(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -1.0 : 1.0;
	Bullet* b = static_cast<Bullet*>(RegisteredMoverClasses["Bullet"](chara.tl));
	b->initialize(REFPOINT_SLASH_X, REFPOINT_SLASH_Y
		, chara.Center() + $V(vx * 24 - 16, -16.0), chara.Owner(), 30 + chara.Muscle(), $V(vx * 1.5, 0), 0);
	//b->snd_hit = $P.snd_hit;
	b->LifeTime(15);
	b->Radius(16);
	$P.sound_stack.push_back($P.snd_swing);
	$P.sound_stack.push_back($P.snd_exp);

	gfw::generate<Particles>(*chara.tl)->initialize(REFPOINT_MINISTAR_X, REFPOINT_MINISTAR_Y, 16, 16,
		chara.Center() + $V(vx * 24 - 16, -16.0), 5, Vector2D(0, 0), 0, 8, 4.0);
	gfw::generate<Particles>(*chara.tl)->initialize(REFPOINT_MINISTAR_X, REFPOINT_MINISTAR_Y, 16, 16,
		chara.Center() + $V(vx * 24 - 16, -16.0), 5, Vector2D(0, 0), 0, 8, 2.0);
}

void jnr::spl::arrow(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -6.0 : 6.0;
	//Effect* e = gfw::generate<Effect>(*chara.tl);
	//e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(vx - 16, -16.0), 2, $V(0, 0), 0);
	Bullet* b = gfw::generate<Bullet>(*chara.tl);
	b->initialize(REFPOINT_ARROW_X, REFPOINT_ARROW_Y, chara.Center() + $V(vx - 16, -16.0)
		, chara.Owner(), 8, $V(vx, -3), 1);
	$P.sound_stack.push_back($P.snd_ignition);
}
void jnr::spl::arrow3(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -4.0 : 4.0;
	//Effect* e = gfw::generate<Effect>(*chara.tl);
	//e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(vx - 16, -16.0), 2, $V(0, 0), 0);
	Bullet* b = gfw::generate<Bullet>(*chara.tl);
	b->initialize(REFPOINT_ARROW_X, REFPOINT_ARROW_Y, chara.Center() + $V(vx - 16, -16.0)
		, chara.Owner(), 10, $V(vx, -4), 1);
	b = gfw::generate<Bullet>(*chara.tl);
	b->initialize(REFPOINT_ARROW_X, REFPOINT_ARROW_Y, chara.Center() + $V(vx - 16, -16.0)
		, chara.Owner(), 10, $V(vx, -5), 1);
	b = gfw::generate<Bullet>(*chara.tl);
	b->initialize(REFPOINT_ARROW_X, REFPOINT_ARROW_Y, chara.Center() + $V(vx - 16, -16.0)
		, chara.Owner(), 10, $V(vx, -2), 1);
	$P.sound_stack.push_back($P.snd_ignition);
}
void jnr::spl::bomb(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -4.0 : 4.0;
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y,
		32, 32, chara.Center() + $V(vx - 16, -16.0), 2, $V(0, 0), 0);
	Bomb* b = gfw::generate<Bomb>(*chara.tl);
	b->initialize(REFPOINT_BOMB_X, REFPOINT_BOMB_Y, chara.Center() + $V(vx - 16, -16.0)
		, chara.Owner(), 5, $V(vx, -2), 1, false);
	$P.sound_stack.push_back($P.snd_ignition);
}
void jnr::spl::bullet(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -8.0 : 8.0;
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(vx - 16, -16.0), 2, $V(0, 0), 0);
	Bullet* b = gfw::generate<Bullet>(*chara.tl);
	b->initialize(REFPOINT_BULLET_X, REFPOINT_BULLET_Y, chara.Center() + $V(vx - 16, -16.0), chara.Owner(), 5, $V(vx, 0), 0);
	b->Radius(3);
	$P.sound_stack.push_back($P.snd_ignition);
}
void jnr::spl::bullet2(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -8.0 : 8.0;
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y,
		32, 32, chara.Center() + $V(vx - 16, -16.0), 2, $V(0, 0), 0);
	Bullet* b = gfw::generate<Bullet>(*chara.tl);
	b->initialize(REFPOINT_BULLET_X, REFPOINT_BULLET_Y,
		chara.Center() + $V(vx - 16, -16.0), chara.Owner(), 8, $V(vx, 0), 0);
	b->Radius(3);
	$P.sound_stack.push_back($P.snd_ignition);
}
void jnr::spl::shotgun(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -8.0 : 8.0;
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y,
		32, 32, chara.Center() + $V(vx - 16, -16.0), 2, $V(0, 0), 0);
	ShotGun* b = gfw::generate<ShotGun>(*chara.tl);
	b->initialize(REFPOINT_BULLET_X, REFPOINT_BULLET_Y,
		chara.Center() + $V(vx - 16, -16.0), chara.Owner(), 8, $V(vx, 0), 0);
	b->Radius(3);
	$P.sound_stack.push_back($P.snd_ignition);
}
void jnr::spl::dashattack(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -8.0 : 8.0;
	auto t = gfw::generate<Tackle>(*chara.tl);
	t->initialize(chara, 15, $V(vx, 0));
	t->setParticleRef(REFPOINT_MINISTAR_X, REFPOINT_MINISTAR_Y);
	$P.sound_stack.push_back($P.snd_nyu);
}
void jnr::spl::jumpattack(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vy = -12;
	gfw::generate<Tackle>(*chara.tl)->initialize(chara, 14, $V(0, vy));
	$P.sound_stack.push_back($P.snd_nyu);
}
void jnr::spl::flame(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -6.0 : 6.0;
	gfw::generate<Flame>(*chara.tl)->initialize(REFPOINT_FIRE_X, REFPOINT_FIRE_Y, chara.Center() + $V(vx - 16, -16.0)
		, chara.Owner(), 20, $V(vx, 0), 0, 6);
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(vx - 16, -16.0), 4, $V(0, 0), 0);
	$P.sound_stack.push_back($P.snd_fire);
}
void jnr::spl::miniflame(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -6.0 : 6.0;
	gfw::generate<Flame>(*chara.tl)->initialize(REFPOINT_FIRE_X, REFPOINT_FIRE_Y, chara.Center() + $V(vx - 16, -16.0)
		, chara.Owner(), 15, $V(vx, 0), 0, 4);
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(vx - 16, -16.0), 4, $V(0, 0), 0);
	$P.sound_stack.push_back($P.snd_fire);
}
void jnr::spl::fire(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -3.0 : 3.0;
	gfw::generate<Fire>(*chara.tl)->initialize(REFPOINT_FIRE_X, REFPOINT_FIRE_Y, chara.Center() + $V(vx - 16, -16.0)
		, chara.Owner(), 10, $V(vx, 0), 0);
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(vx - 16, -16.0), 8
		, $V(0, 0), 0, false, 0.6, 0.1, 16);
	$P.sound_stack.push_back($P.snd_fire);
}
void jnr::spl::homingfire(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -2.0 : 2.0;
	gfw::generate<Fire>(*chara.tl)->initialize(REFPOINT_FIRE_X + 32, REFPOINT_FIRE_Y, chara.Center() + $V(vx - 16, -16.0)
		, chara.Owner(), 12, $V(vx, 0), 0, true);
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(vx - 16, -16.0), 8
		, $V(0, 0), 0, false, 0.6, 0.1, 16);
	$P.sound_stack.push_back($P.snd_fire);
}
void jnr::spl::homingfire3(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -2.0 : 2.0;
	$V vs[] = { $V(vx * 0.75, -1.5), $V(vx, 0), $V(vx* 0.75, 1.5) };
	for (auto v : vs)
	{
		gfw::generate<Fire>(*chara.tl)->initialize(REFPOINT_FIRE_X + 32, REFPOINT_FIRE_Y, chara.Center() + $V(vx - 16, -16.0)
			, chara.Owner(), 8, v, 0, true);
	}
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(vx - 16, -16.0), 8
		, $V(0, 0), 0, false, 0.6, 0.1, 16);
	$P.sound_stack.push_back($P.snd_fire);
}

void jnr::spl::laser(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -8.0 : 8.0;
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(vx - 16, -16.0), 2, $V(0, 0), 0);
	gfw::generate<Laser>(*chara.tl)
		->initialize(0xffffaaaa, 128, chara.Center() + $V(vx, 0), chara.Owner(), 18, $V(vx, 0.0));
	gfw::generate<Particles>(*chara.tl)->initialize(REFPOINT_MINISTAR_X, REFPOINT_MINISTAR_Y, 16, 16,
		chara.Center() - Vector2D(8, 8), 5, Vector2D(0, 0), 0, 12, 4.0);
	//b->initialize(REFPOINT_MISSILE_X, REFPOINT_MISSILE_Y, chara.Center() + $V(vx - 16, -16.0), chara.Owner(), 5, $V(vx, 0.0), 0, true);
	$P.sound_stack.push_back($P.snd_laser);
}
void jnr::spl::laser2(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -8.0 : 8.0;
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(vx - 16, -16.0), 2, $V(0, 0), 0);
	gfw::generate<Laser>(*chara.tl)
		->initialize(0xffffaaaa, 128, chara.Center() + $V(vx, 0), chara.Owner(), 18, $V(vx, 0.0), 2);
	gfw::generate<Particles>(*chara.tl)->initialize(REFPOINT_MINISTAR_X, REFPOINT_MINISTAR_Y, 16, 16,
		chara.Center() - Vector2D(8, 8), 5, Vector2D(0, 0), 0, 12, 4.0);
	//b->initialize(REFPOINT_MISSILE_X, REFPOINT_MISSILE_Y, chara.Center() + $V(vx - 16, -16.0), chara.Owner(), 5, $V(vx, 0.0), 0, true);
	$P.sound_stack.push_back($P.snd_laser);
}
void jnr::spl::laserslash(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -8.0 : 8.0;
	double theta = chara.Turned() ? M_PI : 0.0;

	gfw::generate<LaserSlash>(*chara.tl)
		->initialize(0xffff6666, 128, theta, &chara, 10);
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32
		, chara.Center() + $V(vx - 16, -16.0), 9, $V(0, 0), 0);
	gfw::generate<Particles>(*chara.tl)->initialize(REFPOINT_MINISTAR_X, REFPOINT_MINISTAR_Y, 16, 16,
		chara.Center() - Vector2D(8, 8), 6, Vector2D(0, 0), 0, 6, 5);
	$P.sound_stack.push_back($P.snd_laser);
}
void jnr::spl::laserslash2(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -8.0 : 8.0;
	double theta = chara.Turned() ? M_PI : 0.0;

	gfw::generate<LaserSlash>(*chara.tl)
		->initialize(0xffff66aa, 128, theta, &chara, 15);
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, 
		chara.Center() + $V(vx - 16, -16.0), 9, $V(0, 0), 0);
	gfw::generate<Particles>(*chara.tl)->initialize(REFPOINT_MINISTAR_X, REFPOINT_MINISTAR_Y, 16, 16,
		chara.Center() - Vector2D(8, 8), 6, Vector2D(0, 0), 0, 6, 5);
	$P.sound_stack.push_back($P.snd_laser);
}
void jnr::spl::missile(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -8.0 : 8.0;
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(vx - 16, -16.0), 2, $V(0, 0), 0);
	Bomb* b = gfw::generate<Bomb>(*chara.tl);
	b->initialize(REFPOINT_MISSILE_X, REFPOINT_MISSILE_Y, chara.Center() + $V(vx - 16, -16.0), chara.Owner(), 5, $V(vx, 0.0), 0
		, true, false);
	$P.sound_stack.push_back($P.snd_ignition);
}
void jnr::spl::mindcontrol(jnr::Character& chara, jnr::JumpnRunData& data)
{
	using namespace myutil;
	using namespace gfw;
	EnemySearcher es;
	es.initialize(&chara);
	auto v = es.search(64 * 6);
	auto o = chara.Owner();
	auto* tl = chara.tl;
	auto p_target = chara.Center();
	v = myutil::filter([o, tl](const Object::Pipe & p)
	{
		const Mover* mvr = static_cast<Mover*>(p.Dst(*tl));
		auto myo = mvr->Owner();
		char c = char(myo);
		return mvr->alive && myo != o && mvr->Name().substr(0, 15) == "character_enemy";
	}, v);

	if (!v.empty())
	{
		auto p = chara.Center();
		std::sort(v.begin(), v.end(), [p, tl](const Object::Pipe & p1, const Object::Pipe & p2)
		{
			return (p - (static_cast<Mover*>(p1.Dst(*tl))->Center() - $V(0, 150))).sqsum()
				< (p - (static_cast<Mover*>(p2.Dst(*tl))->Center() - $V(0, 150))).sqsum();
		});
		auto nearest_enemy = static_cast<Character*>(v.begin()->Dst(*tl));

		auto* e0 = generate<EnchantOwner>(*tl);
		e0->initialize(REFPOINT_CURSE_X, REFPOINT_CURSE_Y, 0, nearest_enemy, 60 * 60, chara.Owner());
		nearest_enemy->setEnchant(e0);

		Effect* e = gfw::generate<Effect>(*chara.tl);
		e->initialize(REFPOINT_EXPLOSION_BLACK_X, REFPOINT_EXPLOSION_BLACK_Y, 32, 32
			, chara.Center() + $V(-16, -16.0)
			, 14, $V(0, 0), 0, false, 0.0, 0.2, 10, 0);
		Effect* e2 = gfw::generate<Effect>(*chara.tl);
		e2->initialize(REFPOINT_EXPLOSION_BLACK_X, REFPOINT_EXPLOSION_BLACK_Y, 32, 32
			, nearest_enemy->Center() + $V(-16, -16.0)
			, 14, $V(0, 0), 0, false, 3.0, -0.2, 0, 10);
		Flow* f = gfw::generate<Flow>(*chara.tl);
		//f->initialize(nearest_enemy, REFPOINT_CURSE_X, REFPOINT_CURSE_Y, 16, 16,
		f->initialize(nearest_enemy, REFPOINT_MINISTAR_X, REFPOINT_MINISTAR_Y + 32, 16, 16,
			chara.Center() - $V(16, 16), 60 * 3, 0, 8);
		$P.sound_stack.push_back($P.snd_blink);
	}

}
void jnr::spl::homingmissile(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -4.0 : 4.0;
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(vx - 16, -16.0), 2, $V(0, 0), 0);
	Bomb* b = gfw::generate<Bomb>(*chara.tl);
	b->initialize(REFPOINT_MISSILE_X, REFPOINT_MISSILE_Y, chara.Center() + $V(vx - 16, -16.0)
		, chara.Owner(), 5, $V(vx, -3.0), 0, true, true);
	$P.sound_stack.push_back($P.snd_ignition);
}
void jnr::spl::volleyball(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -3.0 : 3.0;
	Ball* b = gfw::generate<Ball>(*chara.tl);
	b->initialize(REFPOINT_VOLLEYBALL_X, REFPOINT_VOLLEYBALL_Y, chara.Center() + $V(vx - 16, -16.0), $V(vx, -3.0), 8, chara.Owner());
}
void jnr::spl::starshoot(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -4.0 : 4.0;
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(vx - 16, -16.0), 2, $V(0, 0), 0);
	Bullet* b = gfw::generate<Bullet>(*chara.tl);
	b->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, chara.Center() + $V(vx - 16, -16.0), chara.Owner(), 10, $V(vx, -4), 2.0);
	$P.sound_stack.push_back($P.snd_magicshot);
	gfw::generate<Particles>(*chara.tl)->initialize(REFPOINT_MINISTAR_X, REFPOINT_MINISTAR_Y, 16, 16,
		chara.Center() - Vector2D(8, 8), 5, Vector2D(0, 0), 0, 8, 4.0);
}
void jnr::spl::starstream(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -4.0 : 4.0;
	//Effect* e = gfw::generate<Effect>(*chara.tl);
	StarStream* b = gfw::generate<StarStream>(*chara.tl);
	b->initialize(chara, 10, $V(vx, -4.0), 2);
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, 
		chara.Center() + $V(-16, -16.0)
		, 10, $V(0, 0), 0, false, 2.5, -0.15, 1, 16);
}

void jnr::spl::bubblestream(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -4.0 : 4.0;
	//Effect* e = gfw::generate<Effect>(*chara.tl);
	//e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(vx - 16, -16.0), 2, $V(0, 0), 0);
	BubbleStream* b = gfw::generate<BubbleStream>(*chara.tl);
	b->initialize(chara, 1, $V(vx, -1.0), 1);
}

void jnr::spl::wind(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -8.0 : 8.0;
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(vx - 16, -16.0), 2, $V(0, 0), 0);
	Wind* b = gfw::generate<Wind>(*chara.tl);
	b->initialize(REFPOINT_GAS_WHITE_X, REFPOINT_GAS_WHITE_Y, chara.Center() + $V(vx - 16, -16.0), chara.Owner(), 5, $V(vx, 0), 0);
}
void jnr::spl::thunder(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -10.0 : 10.0;
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(vx - 16, -16.0), 4, $V(0, 0), 0);
	Thunder* b = gfw::generate<Thunder>(*chara.tl);
	b->initialize(REFPOINT_THUNDER_X, REFPOINT_THUNDER_Y, chara.Center() + $V(vx - 16, -16.0)
		, chara.Owner(), 8, $V(vx, 0), 0, true);
	$P.sound_stack.push_back($P.snd_elec);

}
void jnr::spl::thunderdir(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -10.0 : 10.0;
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(vx - 16, -16.0), 4, $V(0, 0), 0);
	Thunder* b = gfw::generate<Thunder>(*chara.tl);
	b->initialize(REFPOINT_THUNDER_X, REFPOINT_THUNDER_Y, chara.Center() + $V(vx - 16, -16.0)
		, chara.Owner(), 8, 10 * chara.DirectionSpell(), 0, true);
	$P.sound_stack.push_back($P.snd_elec);

}

void jnr::spl::sword(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -8.0 : 8.0;
	double dy = 52;
	//Effect* e = gfw::generate<Effect>(*chara.tl);
	//e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(vx - 16, -16.0 - dy), 8, $V(0, 0), 0);
	Sword* b = gfw::generate<Sword>(*chara.tl);
	b->initialize(REFPOINT_SWORD_X, REFPOINT_SWORD_Y, chara.Center() + $V(vx - 16, -16.0 - dy)
		, chara.Owner(), 15, $V(vx, 0), 0, true);
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(vx - 16, -16.0 - dy), 8, $V(0, 0), 0);
}

void jnr::spl::sword3(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -8.0 : 8.0;
	//Effect* e = gfw::generate<Effect>(*chara.tl);
	//e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(vx - 16, -16.0), 2, $V(0, 0), 0);
	SwordStream* b = gfw::generate<SwordStream>(*chara.tl);
	b->initialize(chara, 10, $V(vx, -4.0), 0);
}

void jnr::spl::thunderbolt(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -8.0 : 8.0;
	double dy = 48;
	Thunderbolt* b = gfw::generate<Thunderbolt>(*chara.tl);
	b->initialize(chara, 10, $V(vx, -4.0), 0, true);
}
void jnr::spl::gas(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -8.0 : 8.0;
	double dy = 48;
	EmitGas* b = gfw::generate<EmitGas>(*chara.tl);
	b->initialize(&chara, chara.Owner(), 2, $V(0, 0), $V(vx, -4.0), 0.5);
	b->initialize(&chara, chara.Owner(), 2, $V(0, 0), $V(vx, -4.0), 0.5);
}

void jnr::spl::gasbomb(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -4.0 : 4.0;
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y,
		32, 32, chara.Center() + $V(vx - 16, -16.0), 2, $V(0, 0), 0);
	Bomb* b = gfw::generate<Bomb>(*chara.tl);
	b->initialize(REFPOINT_FRASCO_X, REFPOINT_FRASCO_Y, chara.Center() + $V(vx - 16, -16.0)
		, chara.Owner(), 5, $V(vx, -2), 1, false, false, false, Bomb::TGas);
	$P.sound_stack.push_back($P.snd_ignition);
}

void jnr::spl::slimeball(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -2.0 : 2.0;
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(vx - 16, -16.0), 2, $V(0, 0), 0);
	SlowBall* b = gfw::generate<SlowBall>(*chara.tl);
	b->initialize(REFPOINT_SLIME_X, REFPOINT_SLIME_Y, chara.Center() + $V(vx - 16, -16.0)
		, chara.Owner(), 5, $V(vx, -1.6), 1, false);
	$P.sound_stack.push_back($P.snd_blink);
}

void jnr::spl::darkwave(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -8.0 : 8.0;
	double dy = 48;
	DarkWave* b = gfw::generate<DarkWave>(*chara.tl);
	b->initialize(chara, 4, $V(0, 0), 0);
}

void jnr::spl::leafshoot(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -2.0 : 2.0;
	//Effect* e = gfw::generate<Effect>(*chara.tl);
	//e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(vx - 16, -16.0), 2, $V(0, 0), 0);
	Bullet* b = gfw::generate<Bullet>(*chara.tl);
	b->initialize(REFPOINT_LEAF_X, REFPOINT_LEAF_Y, chara.Center() + $V(vx - 16, -16.0)
		, chara.Owner(), 8, $V(vx, -4), 1, true);
	Bullet* b2 = gfw::generate<Bullet>(*chara.tl);
	b2->initialize(REFPOINT_LEAF_X, REFPOINT_LEAF_Y, chara.Center() + $V(vx - 16, -16.0)
		, chara.Owner(), 8, $V(vx * 2, -4), 1, true);
	$P.sound_stack.push_back($P.snd_ignition);
}

void jnr::spl::sonicboom(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -8.0 : 8.0;
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(vx - 16, -16.0), 2, $V(0, 0), 0);
	Sonicboom* b = gfw::generate<Sonicboom>(*chara.tl);
	b->initialize(REFPOINT_SONIC_X, REFPOINT_SONIC_Y, chara.Center() + $V(vx - 16, -16.0), chara.Owner(), 8, $V(vx, 0), 0);
	$P.sound_stack.push_back($P.snd_ignition);
}

void jnr::spl::shockwave(jnr::Character& chara, jnr::JumpnRunData& data)
{
	if (!chara.onSomething())
	{
		$P.sound_stack.push_back($P.snd_punch);
		return;
	}
	double vx = chara.Turned() ? -8.0 : 8.0;
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(vx - 16, -16.0), 2, $V(0, 0), 0);
	Sonicboom* b = gfw::generate<Sonicboom>(*chara.tl);
	b->initialize(REFPOINT_SONIC_X, REFPOINT_SONIC_Y, chara.Center() + $V(vx - 16, -16.0), chara.Owner(), 8, $V(vx, 0), 0);
	b->initialize(REFPOINT_SONIC_X, REFPOINT_SONIC_Y, chara.Center() + $V(-vx - 16, -16.0), chara.Owner(), 8, $V(-vx, 0), 0);
	$P.sound_stack.push_back($P.snd_swing);
}

void jnr::spl::scatterurchin(jnr::Character& chara, jnr::JumpnRunData& data)
{
	for (int i = 0; i < 4; i++)
	{
		double theta = M_PI * ((double)(i + 1) / 5) + M_PI;
		double dx = 3 * cos(theta);
		double dy = 2 * sin(theta);
		enm::Urchin* e = gfw::generate<enm::Urchin>(*chara.tl);
		e->initialize(REFPOINT_URCHIN_X, REFPOINT_URCHIN_Y, chara.Center() - $V(16, 16)
			, chara.Owner(), false, 1.0, 30 * 60);
		e->V($V(dx, dy));

	}
}


void jnr::spl::whip(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -1.0 : 1.0;
	Whip* b = gfw::generate<Whip>(*chara.tl);
	b->initialize(chara, 10, $V(vx * 8, -1), $V(chara.Width() / 2, chara.Height() / 2), 1);
	//b->snd_hit = $P.snd_hit;
	//b->LifeTime(8);
	//b->Radius(8);
	$P.sound_stack.push_back($P.snd_swing);
}

void jnr::spl::bigeyetentacle(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vx = chara.Turned() ? -1.0 : 1.0;
	BigEyeTentacle* b = gfw::generate<BigEyeTentacle>(*chara.tl);
	b->initialize(chara, 10, $V(vx * 4, -1), $V(chara.Width() / 2, chara.Height() / 2), 1);
	//b->snd_hit = $P.snd_hit;
	//b->LifeTime(8);
	//b->Radius(8);
	$P.sound_stack.push_back($P.snd_swing);
}

void jnr::spl::barrier(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<Barrier>(*chara.tl)->initialize(REFPOINT_CIRCLE_LIGHT_X, REFPOINT_CIRCLE_LIGHT_Y,
		chara.P(), chara.Owner(), &chara);
	$P.sound_stack.push_back($P.snd_kira);
	gfw::generate<Particles>(*chara.tl)->initialize(REFPOINT_MINISTAR_X, REFPOINT_MINISTAR_Y, 16, 16,
		chara.Center() + Vector2D(-8, -8 + 32), 32, Vector2D(0, 0), 0, 4, 2.0, Particles::ParticleSpiral);
}
void jnr::spl::healbarrier(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<HealBarrier>(*chara.tl)->initialize(REFPOINT_CIRCLE_LIGHT_X, REFPOINT_CIRCLE_LIGHT_Y,
		chara.P(), chara.Owner(), &chara);
	$P.sound_stack.push_back($P.snd_kira);
}
void jnr::spl::bubblebarrier(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<BubbleBarrier>(*chara.tl)->initialize(REFPOINT_CIRCLE_LIGHT_X, REFPOINT_CIRCLE_LIGHT_Y,
		chara.P(), chara.Owner(), &chara);
	$P.sound_stack.push_back($P.snd_kira);
}
void jnr::spl::wing(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<Wing>(*chara.tl)->initialize(REFPOINT_WING_X, REFPOINT_WING_Y, chara.P(), chara.Owner(), &chara);
}
void jnr::spl::willowisp2(jnr::Character& chara, jnr::JumpnRunData& data)
{
	for (int i = 0; i < 2; i++)
	{
		double p = M_PI * 2.0 / 2 * i;
		$V v = $V::polar(4, p);
		gfw::generate<Willowisp>(*chara.tl)->initialize(REFPOINT_FIRE_X, REFPOINT_FIRE_Y,
			chara, chara.P(), v, 10, chara.Owner(), p);
		$P.sound_stack.push_back($P.snd_fire);
	}
}
void jnr::spl::willowispperp3(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<WillowispPerpetual>(*chara.tl)->
		initialize(REFPOINT_FIRE_X, REFPOINT_FIRE_Y, chara.P(), chara.Owner(), &chara, 3);
}
void jnr::spl::quake(jnr::Character& chara, jnr::JumpnRunData& data)
{
	using namespace gfw;
	data.camera.setQuake(30, 4);
	$P.sound_stack.push_back($P.snd_exp);

	for (auto it = chara.tl->begin(); it != chara.tl->end();)
	{
		gfw::Object* obj = getObject(it);
		Mover* mvr = static_cast<Mover*>(obj);
		std::string prefix = mvr->Name().substr(0, 9);
		if (mvr->IsOnSomething()
			&& mvr->ID() != chara.ID()
			&& mvr->Name().size() >= 9
			&& prefix.compare("character") == 0)
		{
			Character* c = static_cast<Character*>(obj);
			Damage d;
			d.ignoring_barrier = true;
			d.isPhysical = false;
			d.owner = chara.Owner();
			d.power = 25;
			d.stun_time = Constants::time_stun;
			d.invinc_time = Constants::time_invinc;
			d.smash = $V(0, -6);
			if (c->damage(d) >= 1)
			{
				generate<Effect>(*chara.tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32,
					c->Center() - $V(16, 16), 2, $V(0.0, 0.0), 0);
				generate<Effect>(*chara.tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32,
					c->Center() - $V(16, 16), 4, $V(0.0, 0.0), 0);

			}
		}

		it++;
	}
}
void jnr::spl::lifegain20(jnr::Character& chara, jnr::JumpnRunData& data)
{
	chara.HP() += 15;
	int hpmax = chara.HPMax();
	if (chara.HP() > hpmax)
		chara.HP() = hpmax;
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(-16, -16.0)
		, 10, $V(0, 0), 0, false, 2.0, -0.1, 1, 16);
	$P.sound_stack.push_back($P.snd_kira);
	gfw::generate<Particles>(*chara.tl)->initialize(REFPOINT_MINISTAR_X, REFPOINT_MINISTAR_Y + 48, 16, 16,
		chara.Center() + Vector2D(-8, -8 + 32), 32, Vector2D(0, 0), 0, 4, 2.0, Particles::ParticleSpiral);
}
void jnr::spl::lifegain30(jnr::Character& chara, jnr::JumpnRunData& data)
{
	chara.HP() += 35;
	int hpmax = chara.HPMax();
	if (chara.HP() > hpmax)
		chara.HP() = hpmax;
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(-16, -16.0)
		, 10, $V(0, 0), 0, false, 3.0, -0.2, 1, 16);
	$P.sound_stack.push_back($P.snd_kira);
	gfw::generate<Particles>(*chara.tl)->initialize(REFPOINT_MINISTAR_X, REFPOINT_MINISTAR_Y + 48, 16, 16,
		chara.Center() + Vector2D(-8, -8 + 32), 32, Vector2D(0, 0), 0, 4, 2.0, Particles::ParticleSpiral);
	gfw::generate<Particles>(*chara.tl)->initialize(REFPOINT_MINISTAR_X, REFPOINT_MINISTAR_Y + 48, 16, 16,
		chara.Center() + Vector2D(-8, -8 + 32), 32, Vector2D(0, 0), 0, 4, 2.0, Particles::ParticleSpiral, true);
}
void jnr::spl::life2mana20(jnr::Character& chara, jnr::JumpnRunData& data)
{
	chara.HP() -= 15;
	chara.MP() += 20;
	int hpmax = chara.HPMax();
	if (chara.HP() > hpmax)
		chara.HP() = hpmax;
	if (chara.HP() < 0)
		chara.HP() = 1;
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_RED_X, REFPOINT_EXPLOSION_RED_Y, 32, 32, chara.Center() + $V(-16, -16.0)
		, 16, $V(0, 0), 0, false, 3, -0.15, 1, 12);
	$P.sound_stack.push_back($P.snd_damaged);
}
void jnr::spl::muscle(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<EnchantMuscle>(*chara.tl)->initialize(REFPOINT_SHINE_X, REFPOINT_SHINE_Y
		, chara.Owner(), &chara, 20 * 60, 5);
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(-16, -16.0)
		, 10, $V(0, 0), 0, false, 2.0, -0.1, 1, 16);
	$P.sound_stack.push_back($P.snd_kira);
}
void jnr::spl::concentrate(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<EnchantConcentrate>(*chara.tl)->initialize(REFPOINT_SHINE_X, REFPOINT_SHINE_Y
		, chara.Owner(), &chara, 20 * 60, 8);
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(-16, -16.0)
		, 10, $V(0, 0), 0, false, 2.0, -0.1, 1, 16);
	$P.sound_stack.push_back($P.snd_kira);
}
void jnr::spl::speedup(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<EnchantSlow>(*chara.tl)->initialize(REFPOINT_SHINE_X, REFPOINT_SHINE_Y
		, chara.Owner(), &chara, 10 * 60, 1.5);
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(-16, -16.0)
		, 10, $V(0, 0), 0, false, 2.0, -0.1, 1, 16);
	$P.sound_stack.push_back($P.snd_kira);
}
void jnr::spl::mphprate(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<EnchantMPHPRate>(*chara.tl)->initialize(REFPOINT_CURSE_X + 32, REFPOINT_CURSE_Y + 32
		, chara.Owner(), &chara, 20 * 60, 1);
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_RED_X, REFPOINT_EXPLOSION_RED_Y, 32, 32, chara.Center() + $V(-16, -16.0)
		, 10, $V(0, 0), 0, false, 3.5, -0.17, 1, 12);
	$P.sound_stack.push_back($P.snd_kira);
}
void jnr::spl::shuffle(jnr::Character& chara, jnr::JumpnRunData& data)
{
	chara.resetDeck();
	Effect* e = gfw::generate<Effect>(*chara.tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(-16, -16.0)
		, 10, $V(0, 0), 0, false, 2.0, -0.1, 1, 16);
}
void jnr::spl::recycle(jnr::Character& chara, jnr::JumpnRunData& data)
{
	chara.recycleCard(data);
}
void jnr::spl::flush(jnr::Character& chara, jnr::JumpnRunData& data)
{
	chara.flushHand();
}
void jnr::spl::circle_cancel(jnr::Character& chara, jnr::JumpnRunData& data)
{
	auto& tl = *chara.tl;

	int max_lifetime = -99999;
	Circle* argmax = nullptr;

	for (auto t = tl.begin(); t != tl.end(); t++)
	{
		gfw::Object* obj = getObject(t);
		Mover* mvr = (Mover*)obj;

		if (mvr->Name().substr(0, 6) == "circle")
		{
			Circle* ccl = (Circle*)obj;
			if (ccl->Owner() == chara.Owner())
				continue;
			if (ccl->LeftLifeTime() > max_lifetime)
			{
				argmax = ccl;
				max_lifetime = ccl->LeftLifeTime();
			}

		}
	}

	if (argmax)
	{
		gfw::generate<Effect>(*chara.tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y
			, 32, 32, argmax->P() + Vector2D(16, 16), 20, Vector2D(0, 0), 0, false, 0.2, 0.2, 15);
		argmax->cancel();
		Effect* e = gfw::generate<Effect>(*chara.tl);
		e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, chara.Center() + $V(-16, -16.0)
			, 10, $V(0, 0), 0, false, 3.0, -0.2, 1, 16);
		gfw::generate<Particles>(*chara.tl)->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32,
			chara.Center() + $V(-16, 0), 8, $V(0, 0), 0);
	}
}

void jnr::spl::circle(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<Circle>(*chara.tl)->initialize(REFPOINT_CIRCLE_HEPTAGRAMMA_X, REFPOINT_CIRCLE_HEPTAGRAMMA_Y
		, chara.CircleRefX(), chara.CircleRefY(), chara.Center() + Vector2D(-32, 16), chara.Owner()
		, 180, 160, Vector2D(0, 0), 0.01, 1, 0, 0, 4);

}
void jnr::spl::circle_explosion(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<CircleExplosion>(*chara.tl)->initialize(REFPOINT_CIRCLE_HEPTAGRAMMA_X, REFPOINT_CIRCLE_HEPTAGRAMMA_Y
		, chara.CircleRefX(), chara.CircleRefY(), chara.Center() + Vector2D(-32, 16), chara.Owner()
		, 180, 160, Vector2D(0, 0), 0.01, 1, 0, 0, 4);
	gfw::generate<Effect>(*chara.tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y
		, 32, 32, chara.Center() + Vector2D(-16, 32), 20, Vector2D(0, 0), 0, false, 0.2, 0.2, 15);
}

void jnr::spl::circle_barrier(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<CircleBarrier>(*chara.tl)->initialize(64 * 10, REFPOINT_CIRCLE_FOURCIRCLE_Y
		, chara.CircleRefX(), chara.CircleRefY(), chara.Center() + Vector2D(-32, 16), chara.Owner()
		, 60 * 30, 45, Vector2D(0, 0), 0.01, 1, 0, 0, 4);
	gfw::generate<Effect>(*chara.tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y
		, 32, 32, chara.Center() + Vector2D(-16, 32), 20, Vector2D(0, 0), 0, false, 0.2, 0.2, 15);
}

void jnr::spl::circle_bubble(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<CircleBubble>(*chara.tl)->initialize(REFPOINT_CIRCLE_FOURCIRCLE_X, REFPOINT_CIRCLE_FOURCIRCLE_Y
		, chara.CircleRefX(), chara.CircleRefY(), chara.Center() + Vector2D(-32, 16), chara.Owner()
		, 240, 45, Vector2D(0, 0), 0.01, 1, 0, 0, 4);
	gfw::generate<Effect>(*chara.tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y
		, 32, 32, chara.Center() + Vector2D(-16, 32), 20, Vector2D(0, 0), 0, false, 0.2, 0.2, 15);
}

void jnr::spl::circle_gas(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<CircleGas>(*chara.tl)->initialize(REFPOINT_CIRCLE_FOURCIRCLE_X, REFPOINT_CIRCLE_FOURCIRCLE_Y
		, chara.CircleRefX(), chara.CircleRefY(), chara.Center() + Vector2D(-32, 16), chara.Owner()
		, 450, 150, Vector2D(0, 0), 0.01, 1, 0, 0, 4);
	gfw::generate<Effect>(*chara.tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y
		, 32, 32, chara.Center() + Vector2D(-16, 32), 20, Vector2D(0, 0), 0, false, 0.2, 0.2, 15);
}

void jnr::spl::circle_flame(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<CircleFlame>(*chara.tl)->initialize(REFPOINT_CIRCLE_TRIANGLE_X, REFPOINT_CIRCLE_TRIANGLE_Y
		, chara.CircleRefX(), chara.CircleRefY(), chara.Center() + Vector2D(-32, 16), chara.Owner()
		, 450, 150, Vector2D(0, 0), 0.01, 1, 0, 0, 4);
	gfw::generate<Effect>(*chara.tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y
		, 32, 32, chara.Center() + Vector2D(-16, 32), 20, Vector2D(0, 0), 0, false, 0.2, 0.2, 15);
}


void jnr::spl::circle_flame_nearest(jnr::Character& chara, jnr::JumpnRunData& data)
{
	using namespace myutil;
	using namespace gfw;
	EnemySearcher es;
	es.initialize(&chara);
	auto v = es.search(64 * 6);
	auto o = chara.Owner();
	auto* tl = chara.tl;
	auto p_target = chara.Center();
	v = myutil::filter([o, tl](const Object::Pipe & p)
	{
		const Mover* mvr = static_cast<Mover*>(p.Dst(*tl));
		auto myo = mvr->Owner();
		char c = char(myo);
		return mvr->alive && myo != o && mvr->Name().substr(0, 9) == "character";
	}, v);

	if (!v.empty())
	{
		auto p = chara.Center();
		std::sort(v.begin(), v.end(), [p, tl](const Object::Pipe & p1, const Object::Pipe & p2)
		{
			return (p - (static_cast<Mover*>(p1.Dst(*tl))->Center() - $V(0, 150))).sqsum()
				< (p - (static_cast<Mover*>(p2.Dst(*tl))->Center() - $V(0, 150))).sqsum();
		});
		auto dp = static_cast<Mover*>(v.begin()->Dst(*tl))->Center() + $V(0, -150) - p;
		p_target = static_cast<Mover*>(v.begin()->Dst(*tl))->Center();

		StageAccessor sa(*Mover::stg);
		double d = sa.distanceToFloor(p_target, data);
		p_target.y += d;
		p_target.y = (int)p_target.y / 32 * 32 - 32;
	}

	gfw::generate<CircleFlame>(*chara.tl)->initialize(REFPOINT_CIRCLE_TRIANGLE_X, REFPOINT_CIRCLE_TRIANGLE_Y
		, chara.CircleRefX(), chara.CircleRefY(), p_target + Vector2D(-32, 16), chara.Owner()
		, 360, 150, Vector2D(0, 0), 0.01, 1, 0, 0, 4);
	gfw::generate<Effect>(*chara.tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y
		, 32, 32, p_target + Vector2D(-16, 32), 20, Vector2D(0, 0), 0, false, 0.2, 0.2, 15);
}

void jnr::spl::circle_summon_frog(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<CircleSummonFrog>(*chara.tl)->initialize(REFPOINT_CIRCLE_STAR_X, REFPOINT_CIRCLE_STAR_Y
		, chara.CircleRefX(), chara.CircleRefY(), chara.Center() + Vector2D(-32, 16), chara.Owner()
		, 180, 170, Vector2D(0, 0), chara.Turned(), 0.01, 1, 0, 0, 4);
	gfw::generate<Effect>(*chara.tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y
		, 32, 32, chara.Center() + Vector2D(-16, 32), 20, Vector2D(0, 0), 0, false, 0.2, 0.2, 15);
}

void jnr::spl::circle_summon_dirtcube(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<CircleSummonDirtCube>(*chara.tl)->initialize(REFPOINT_CIRCLE_SINGLESQUARE_X, REFPOINT_CIRCLE_SINGLESQUARE_Y
		, chara.CircleRefX(), chara.CircleRefY(), chara.Center() + Vector2D(-32, 16), -1
		, 180, 170, Vector2D(0, 0), chara.Turned(), 0.01, 1, 0, 0, 4);
	gfw::generate<Effect>(*chara.tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y
		, 32, 32, chara.Center() + Vector2D(-16, 32), 20, Vector2D(0, 0), 0, false, 0.2, 0.2, 15);

}


void jnr::spl::circle_summon_bombbox(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<CircleSummonBombBox>(*chara.tl)->initialize(REFPOINT_CIRCLE_SINGLESQUARE_X, REFPOINT_CIRCLE_SINGLESQUARE_Y
		, chara.CircleRefX(), chara.CircleRefY(), chara.Center() + Vector2D(-32, 16), -1
		, 180, 170, Vector2D(0, 0), chara.Turned(), 0.01, 1, 0, 0, 4);
	gfw::generate<Effect>(*chara.tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y
		, 32, 32, chara.Center() + Vector2D(-16, 32), 20, Vector2D(0, 0), 0, false, 0.2, 0.2, 15);

}


void jnr::spl::circle_summon_mushroom(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<CircleSummonMushroom>(*chara.tl)->initialize(REFPOINT_CIRCLE_STAR_X, REFPOINT_CIRCLE_STAR_Y
		, chara.CircleRefX(), chara.CircleRefY(), chara.Center() + Vector2D(-32, 16), chara.Owner()
		, 180, 170, Vector2D(0, 0), chara.Turned(), 0.01, 1, 0, 0, 4);
	gfw::generate<Effect>(*chara.tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y
		, 32, 32, chara.Center() + Vector2D(-16, 32), 20, Vector2D(0, 0), 0, false, 0.2, 0.2, 15);
}

void jnr::spl::circle_summon_ghost(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<CircleSummonGhost>(*chara.tl)->initialize(REFPOINT_CIRCLE_STAR_X, REFPOINT_CIRCLE_STAR_Y
		, chara.CircleRefX(), chara.CircleRefY(), chara.Center() + Vector2D(-32, 16), chara.Owner()
		, 180, 170, Vector2D(0, 0), chara.Turned(), 0.01, 1, 0, 0, 4);
	gfw::generate<Effect>(*chara.tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y
		, 32, 32, chara.Center() + Vector2D(-16, 32), 20, Vector2D(0, 0), 0, false, 0.2, 0.2, 15);
}

void jnr::spl::circle_summon_ghost2(jnr::Character& chara, jnr::JumpnRunData& data)
{
	for (int i = 0; i < 2; i++)
	{
		gfw::generate<CircleSummonGhost>(*chara.tl)->initialize(REFPOINT_CIRCLE_STAR_X, REFPOINT_CIRCLE_STAR_Y
			, chara.CircleRefX(), chara.CircleRefY(), chara.Center() + Vector2D(-32, 16), chara.Owner()
			, 180, 170 - 15 * i, Vector2D(0, 0), chara.Turned(), 0.01, 1, 0, 0, 4);
	}
	gfw::generate<Effect>(*chara.tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y
		, 32, 32, chara.Center() + Vector2D(-16, 32), 20, Vector2D(0, 0), 0, false, 0.2, 0.2, 15);
}

void jnr::spl::circle_summon_knight(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<CircleSummonKnight>(*chara.tl)->initialize(REFPOINT_CIRCLE_STAR_X, REFPOINT_CIRCLE_STAR_Y
		, chara.CircleRefX(), chara.CircleRefY(), chara.Center() + Vector2D(-32, 16), chara.Owner()
		, 180, 170, Vector2D(0, 0), chara.Turned(), 0.01, 1, 0, 0, 4);
	gfw::generate<Effect>(*chara.tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y
		, 32, 32, chara.Center() + Vector2D(-16, 32), 20, Vector2D(0, 0), 0, false, 0.2, 0.2, 15);
}


void jnr::spl::circle_summon_candle(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<CircleSummonCandle>(*chara.tl)->initialize(REFPOINT_CIRCLE_STAR_X, REFPOINT_CIRCLE_STAR_Y
		, chara.CircleRefX(), chara.CircleRefY(), chara.Center() + Vector2D(-32, 16), chara.Owner()
		, 180, 170, Vector2D(0, 0), chara.Turned(), 0.01, 1, 0, 0, 4);
	gfw::generate<Effect>(*chara.tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y
		, 32, 32, chara.Center() + Vector2D(-16, 32), 20, Vector2D(0, 0), 0, false, 0.2, 0.2, 15);
}


void jnr::spl::circle_summon_cannon(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<CircleSummonCannon>(*chara.tl)->initialize(REFPOINT_CIRCLE_STAR_X, REFPOINT_CIRCLE_STAR_Y
		, chara.CircleRefX(), chara.CircleRefY(), chara.Center() + Vector2D(-32, 16), chara.Owner()
		, 180, 170, Vector2D(0, 0), chara.Turned(), 0.01, 1, 0, 0, 4);
	gfw::generate<Effect>(*chara.tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y
		, 32, 32, chara.Center() + Vector2D(-16, 32), 20, Vector2D(0, 0), 0, false, 0.2, 0.2, 15);
}


void jnr::spl::circle_summon_pumpkin(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<CircleSummonPumpkin>(*chara.tl)->initialize(REFPOINT_CIRCLE_STAR_X, REFPOINT_CIRCLE_STAR_Y
		, chara.CircleRefX(), chara.CircleRefY(), chara.Center() + Vector2D(-32, 16), chara.Owner()
		, 210, 200, Vector2D(0, 0), chara.Turned(), 0.008, 1, 0, 0, 4);
	gfw::generate<Effect>(*chara.tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y
		, 32, 32, chara.Center() + Vector2D(-16, 32), 20, Vector2D(0, 0), 0, false, 0.2, 0.2, 15);
}

void jnr::spl::circle_summon_fairy_blue(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<CircleSummonFairyBlue>(*chara.tl)->initialize(REFPOINT_CIRCLE_FOURCIRCLE_X, REFPOINT_CIRCLE_FOURCIRCLE_Y
		, chara.CircleRefX(), chara.CircleRefY(), chara.Center() + Vector2D(-32, 16), chara.Owner()
		, 180, 170, Vector2D(0, 0), chara.Turned(), 0.01, 1, 0, 0, 4);
	gfw::generate<Effect>(*chara.tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y
		, 32, 32, chara.Center() + Vector2D(-16, 32), 20, Vector2D(0, 0), 0, false, 0.2, 0.2, 15);
}

void jnr::spl::circle_summon_angel(jnr::Character& chara, jnr::JumpnRunData& data)
{
	auto* ptr = gfw::generate<CircleSummonAngel>(*chara.tl);
	ptr->initialize(REFPOINT_CIRCLE_STAR_X, REFPOINT_CIRCLE_STAR_Y
		, chara.CircleRefX(), chara.CircleRefY(), chara.Center() + Vector2D(-32, 16), chara.Owner()
		, 180, 170, Vector2D(0, 0), chara.Turned(), 0.01, 1, 0, 0, 4);
	ptr->pipe = gfw::Object::Pipe(&chara, *chara.tl);
	gfw::generate<Effect>(*chara.tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y
		, 32, 32, chara.Center() + Vector2D(-16, 32), 20, Vector2D(0, 0), 0, false, 0.2, 0.2, 15);
}

void jnr::spl::circle_summon_elf(jnr::Character& chara, jnr::JumpnRunData& data)
{
	auto* ptr = gfw::generate<CircleSummonElf>(*chara.tl);
	ptr->initialize(REFPOINT_CIRCLE_STAR_X, REFPOINT_CIRCLE_STAR_Y
		, chara.CircleRefX(), chara.CircleRefY(), chara.Center() + Vector2D(-32, 16), chara.Owner()
		, 180, 170, Vector2D(0, 0), chara.Turned(), 0.01, 1, 0, 0, 4);
	ptr->pipe = gfw::Object::Pipe(&chara, *chara.tl);
	gfw::generate<Effect>(*chara.tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y
		, 32, 32, chara.Center() + Vector2D(-16, 32), 20, Vector2D(0, 0), 0, false, 0.2, 0.2, 15);
}

void jnr::spl::circle_summon_manaflower(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<CircleSummonManaFlower>(*chara.tl)->initialize(REFPOINT_CIRCLE_THREECIRCLE_X, REFPOINT_CIRCLE_THREECIRCLE_Y
		, chara.CircleRefX(), chara.CircleRefY(), chara.Center() + Vector2D(-32, 16), chara.Owner()
		, 180, 170, Vector2D(0, 0), 0.01, 1, 0, 0, 4);
	gfw::generate<Effect>(*chara.tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y
		, 32, 32, chara.Center() + Vector2D(-16, 32), 20, Vector2D(0, 0), 0, false, 0.2, 0.2, 15);
}

void jnr::spl::circle_summon_lamp(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<CircleSummonLamp>(*chara.tl)->initialize(REFPOINT_CIRCLE_SQUARE_X, REFPOINT_CIRCLE_SQUARE_Y
		, chara.CircleRefX(), chara.CircleRefY(), chara.Center() + Vector2D(-32, 16), chara.Owner()
		, 180, 170, Vector2D(0, 0), chara.Turned(), 0.01, 1, 0, 0, 4);
	gfw::generate<Effect>(*chara.tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y
		, 32, 32, chara.Center() + Vector2D(-16, 32), 20, Vector2D(0, 0), 0, false, 0.2, 0.2, 15);
}

void jnr::spl::circle_summon_bigslime(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<CircleSummonBigSlime>(*chara.tl)->initialize(REFPOINT_CIRCLE_STAR_X, REFPOINT_CIRCLE_STAR_Y
		, chara.CircleRefX(), chara.CircleRefY(), chara.Center() + Vector2D(-32, 16), chara.Owner()
		, 180, 170, Vector2D(0, 0), chara.Turned(), 0.01, 1, 0, 0, 4);
	gfw::generate<Effect>(*chara.tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y
		, 32, 32, chara.Center() + Vector2D(-16, 32), 20, Vector2D(0, 0), 0, false, 0.2, 0.2, 15);
}

void jnr::spl::circle_trap_explosion(jnr::Character& chara, jnr::JumpnRunData& data)
{
	gfw::generate<CircleTrapExplosion>(*chara.tl)->initialize(REFPOINT_CIRCLE_HEPTAGRAMMA_X, REFPOINT_CIRCLE_HEPTAGRAMMA_Y
		, chara.CircleRefX(), chara.CircleRefY(), chara.Center() + Vector2D(-32, 16), chara.Owner()
		, 60 * 45, 75, Vector2D(0, 0), 0.01, 1, 0, 0, 4);
	gfw::generate<Effect>(*chara.tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y
		, 32, 32, chara.Center() + Vector2D(-16, 32), 20, Vector2D(0, 0), 0, false, 0.2, 0.2, 15);
}