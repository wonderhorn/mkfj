#pragma once
#include<vector>
#include<string>
#include"utils/Geometry.h"
//#include"JumpnRunData.h"
namespace jnr
{
	class Character;
	class JumpnRunData;
	namespace spl
	{
		enum eSpell
		{
			spl_arrow,
			spl_arrow3,
			spl_barrier,
			spl_bigeyetentacle,
			spl_bomb,
			spl_bubblebarrier,
			spl_bubblestream,
			spl_bullet,
			spl_bullet2,
			spl_circle,
			spl_circle_barrier,
			spl_circle_bubble,
			spl_circle_explosion,
			spl_circle_flame,
			spl_circle_flame_nearest,
			spl_circle_gas,
			spl_circle_summon_angel,
			spl_circle_summon_bigslime,
			spl_circle_summon_bombbox,
			spl_circle_summon_candle,
			spl_circle_summon_cannon,
			spl_circle_summon_dirtcube,
			spl_circle_summon_elf,
			spl_circle_summon_fairy_blue,
			spl_circle_summon_frog,
			spl_circle_summon_ghost,
			spl_circle_summon_ghost2,
			spl_circle_summon_knight,
			spl_circle_summon_lamp,
			spl_circle_summon_manaflower,
			spl_circle_summon_mushroom,
			spl_circle_summon_pumpkin,
			spl_circle_trap_explosion,
			spl_circlecancel,
			spl_concentrate,
			spl_darkwave,
			spl_dashattack,
			spl_fire,
			spl_flame,
			spl_flower,
			spl_flush,
			spl_gas,
			spl_gasbomb,
			spl_hardpunch,
			spl_healbarrier,
			spl_homingfire,
			spl_homingfire3,
			spl_homingmissile,
			spl_jumpattack,
			spl_laser,
			spl_laser2,
			spl_laserslash,
			spl_laserslash2,
			spl_leafshoot,
			spl_life2mana20,
			spl_lifegain20,
			spl_lifegain30,
			spl_mindcontrol,
			spl_miniflame,
			spl_missile,
			spl_mphprate,
			spl_muscle,
			spl_punch,
			spl_punch2,
			spl_quake,
			spl_recycle,
			spl_scatterurchin,
			spl_shockwave,
			spl_shotgun,
			spl_shuffle,
			spl_slimeball,
			spl_sonicboom,
			spl_speedup,
			spl_starshoot,
			spl_starstream,
			spl_sword,
			spl_sword3,
			spl_thunder,
			spl_thunderbolt,
			spl_thunderdir,
			spl_volleyball,
			spl_whip,
			spl_willowisp2,
			spl_willowispperp3,
			spl_wind,
			spl_wing,
			spl_nospell
		};

		int spellname2id(const std::string& name);

		class Spell
		{
		public:
			int manacost;
			int anim_type, anim_num;
			int stuntime, frame_of_one_phase, actiontime, num_iteration, final_frame_time;
			eSpell func_id;
			int thumnil_refx, thumnil_refy;
			char rarity;
			int id, power;
			const char* name;

			static const int NUM_SPELL = 61;
			static std::vector<Spell> spells;
		};

		class SpellMessage
		{
		public:
			SpellMessage() : active(false){}
			bool active;
			gmtr::Vector2D direction;
		};

		class SpellAction
		{
		public:
		};

		void(*SpellFunc(eSpell s))(jnr::Character& chara, jnr::JumpnRunData& data);
		void arrow(jnr::Character& chara, jnr::JumpnRunData& data);
		void arrow3(jnr::Character& chara, jnr::JumpnRunData& data);
		void barrier(jnr::Character& chara, jnr::JumpnRunData& data);
		void bigeyetentacle(jnr::Character& chara, jnr::JumpnRunData& data);
		void bomb(jnr::Character& chara, jnr::JumpnRunData& data);
		void bubblebarrier(jnr::Character& chara, jnr::JumpnRunData& data);
		void bubblestream(jnr::Character& chara, jnr::JumpnRunData& data);
		void bullet(jnr::Character& chara, jnr::JumpnRunData& data);
		void bullet2(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle_barrier(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle_bubble(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle_cancel(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle_explosion(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle_flame(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle_flame_nearest(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle_gas(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle_summon_angel(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle_summon_bigslime(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle_summon_bombbox(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle_summon_candle(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle_summon_cannon(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle_summon_dirtcube(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle_summon_elf(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle_summon_fairy_blue(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle_summon_frog(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle_summon_ghost(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle_summon_ghost2(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle_summon_knight(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle_summon_lamp(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle_summon_manaflower(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle_summon_mushroom(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle_summon_pumpkin(jnr::Character& chara, jnr::JumpnRunData& data);
		void circle_trap_explosion(jnr::Character& chara, jnr::JumpnRunData& data);
		void concentrate(jnr::Character& chara, jnr::JumpnRunData& data);
		void darkwave(jnr::Character& chara, jnr::JumpnRunData& data);
		void dashattack(jnr::Character& chara, jnr::JumpnRunData& data);
		void fire(jnr::Character& chara, jnr::JumpnRunData& data);
		void flame(jnr::Character& chara, jnr::JumpnRunData& data);
		void flower(jnr::Character& chara, jnr::JumpnRunData& data);
		void flush(jnr::Character& chara, jnr::JumpnRunData& data);
		void gas(jnr::Character& chara, jnr::JumpnRunData& data);
		void gasbomb(jnr::Character& chara, jnr::JumpnRunData& data);
		void hardpunch(jnr::Character& chara, jnr::JumpnRunData& data);
		void healbarrier(jnr::Character& chara, jnr::JumpnRunData& data);
		void homingfire(jnr::Character& chara, jnr::JumpnRunData& data);
		void homingfire3(jnr::Character& chara, jnr::JumpnRunData& data);
		void homingmissile(jnr::Character& chara, jnr::JumpnRunData& data);
		void jumpattack(jnr::Character& chara, jnr::JumpnRunData& data);
		void laser(jnr::Character& chara, jnr::JumpnRunData& data);
		void laser2(jnr::Character& chara, jnr::JumpnRunData& data);
		void laserslash(jnr::Character& chara, jnr::JumpnRunData& data);
		void laserslash2(jnr::Character& chara, jnr::JumpnRunData& data);
		void leafshoot(jnr::Character& chara, jnr::JumpnRunData& data);
		void life2mana20(jnr::Character& chara, jnr::JumpnRunData& data);
		void lifegain20(jnr::Character& chara, jnr::JumpnRunData& data);
		void lifegain30(jnr::Character& chara, jnr::JumpnRunData& data);
		void miniflame(jnr::Character& chara, jnr::JumpnRunData& data);
		void missile(jnr::Character& chara, jnr::JumpnRunData& data);
		void mindcontrol(jnr::Character& chara, jnr::JumpnRunData& data);
		void mphprate(jnr::Character& chara, jnr::JumpnRunData& data);
		void muscle(jnr::Character& chara, jnr::JumpnRunData& data);
		void punch(jnr::Character& chara, jnr::JumpnRunData& data);
		void punch2(jnr::Character& chara, jnr::JumpnRunData& data);
		void quake(jnr::Character& chara, jnr::JumpnRunData& data);
		void recycle(jnr::Character& chara, jnr::JumpnRunData& data);
		void scatterurchin(jnr::Character& chara, jnr::JumpnRunData& data);
		void shockwave(jnr::Character& chara, jnr::JumpnRunData& data);
		void shotgun(jnr::Character& chara, jnr::JumpnRunData& data);
		void shotgun2(jnr::Character& chara, jnr::JumpnRunData& data);
		void shuffle(jnr::Character& chara, jnr::JumpnRunData& data);
		void slimeball(jnr::Character& chara, jnr::JumpnRunData& data);
		void sonicboom(jnr::Character& chara, jnr::JumpnRunData& data);
		void speedup(jnr::Character& chara, jnr::JumpnRunData& data);
		void starshoot(jnr::Character& chara, jnr::JumpnRunData& data);
		void starstream(jnr::Character& chara, jnr::JumpnRunData& data);
		void sword(jnr::Character& chara, jnr::JumpnRunData& data);
		void sword3(jnr::Character& chara, jnr::JumpnRunData& data);
		void thunder(jnr::Character& chara, jnr::JumpnRunData& data);
		void thunderbolt(jnr::Character& chara, jnr::JumpnRunData& data);
		void thunderdir(jnr::Character& chara, jnr::JumpnRunData& data);
		void volleyball(jnr::Character& chara, jnr::JumpnRunData& data);
		void whip(jnr::Character& chara, jnr::JumpnRunData& data);
		void willowisp2(jnr::Character& chara, jnr::JumpnRunData& data);
		void willowispperp3(jnr::Character& chara, jnr::JumpnRunData& data);
		void wind(jnr::Character& chara, jnr::JumpnRunData& data);
		void wing(jnr::Character& chara, jnr::JumpnRunData& data);
	};
};