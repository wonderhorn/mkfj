#include"jumpnrun/system/BossFactory.h"
#include"jumpnrun/mover/enemy/Enemy.h"
#include"jumpnrun/mover/enemy/EnemyGroup.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/GRAPHICS.h"
#include"jumpnrun/spell/Spell.h"
using namespace gfw;
using namespace jnr;

Character* BossFactory::create(gfw::Tasklist& tl, const JumpnRunData& data, const gmtr::Vector2D& p)
{
	//$P.ai.initialize();

	switch (data.encount_boss)
	{
	default:
	{
		assert(1);
		AIPlayer* ptr = generate<AIPlayer>(tl);
		$P.ai.initialize();
		ptr->initialize(0, 64 * 3, p, 2, true, &$P.ai);
		return ptr;
	}
	case 0:
	{
		AIPlayer* ptr = generate<AIPlayer>(tl);
		$P.ai.initialize("rika");
		ptr->initialize(REFPOINT_BOSS2_X, REFPOINT_BOSS2_Y, p, 2, true, &$P.ai, 80);
		ptr->CircleRef(REFPOINT_CIRCLE_FRAME_PINK_X, REFPOINT_CIRCLE_FRAME_PINK_Y);
		ptr->SpeedWalk(0.9);
		ptr->SpeedDash(1.8);
		using namespace spl;
		/*
		int manacost;
		int anim_type, anim_num;
		int stuntime, frame_of_one_phase, actiontime, num_iteration;*/
		std::vector<spl::Spell> zs =
		{
			{ 8, 8, 2, 90, 30, 32, 1, 30, spl_bubblestream, 32 * 9, 0, 'c', -1, 1, "bubblestream" },
			{ 8, 8, 2, 70, 20, 22, 1, 0, spl_bubblestream, 32 * 9, 0, 'c', -1, 1, "bubblestream" },
			{ 8, 8, 2, 70, 20, 22, 1, 0, spl_bubblestream, 32 * 9, 0, 'c', -1, 1, "bubblestream" },
			{ 8, 8, 2, 70, 20, 22, 1, 0, spl_bubblestream, 32 * 9, 0, 'c', -1, 1, "bubblestream" },
			{ 8, 8, 2, 70, 20, 22, 1, 0, spl_bubblestream, 32 * 9, 0, 'c', -1, 1, "bubblestream" },
		};
		ptr->setZSpellSet(zs);
		std::vector<spl::Spell> xs =
		{
			{ 15, 6, 2, 60, 15, 40, 2, 0, spl_circle_gas, 32 * 12, 32 * 2, 'c', -1, 1, "gas" },
			{ 15, 6, 2, 60, 15, 40, 2, 0, spl_circle_gas, 32 * 12, 32 * 2, 'c', -1, 1, "gas" },
			{ 15, 6, 2, 60, 15, 40, 2, 0, spl_circle_gas, 32 * 12, 32 * 2, 'c', -1, 1, "gas" },
			{ 15, 6, 2, 60, 15, 40, 2, 0, spl_circle_gas, 32 * 12, 32 * 2, 'c', -1, 1, "gas" },
			{ 15, 6, 2, 60, 15, 40, 2, 0, spl_circle_gas, 32 * 12, 32 * 2, 'c', -1, 1, "gas" },
		};
		ptr->setXSpellSet(xs);
		return ptr;
	}
	case 1:
	{
		AIPlayer* ptr = generate<AIPlayer>(tl);
		$P.ai.initialize("kyo1");
		ptr->initialize(REFPOINT_BOSS1_X, REFPOINT_BOSS1_Y, p, 2, true, &$P.ai, 100);
		ptr->CircleRef(REFPOINT_CIRCLE_FRAME_RED_X, REFPOINT_CIRCLE_FRAME_RED_Y);
		using namespace spl;
		/*
		int manacost;
		int anim_type, anim_num;
		int stuntime, frame_of_one_phase, actiontime, num_iteration;*/
		std::vector<spl::Spell> zs =
		{
			{ 15, 8, 2, 60, 25, 28, 1, 0, spl_homingfire, 32 * 9, 0, 'c', -1, 8, "homingfire" },
			{ 15, 8, 2, 60, 25, 28, 1, 0, spl_homingfire, 32 * 9, 0, 'c', -1, 8, "homingfire" },
			{ 15, 8, 2, 60, 25, 28, 1, 0, spl_homingfire, 32 * 9, 0, 'c', -1, 8, "homingfire" },
			{ 15, 8, 2, 60, 25, 28, 1, 0, spl_homingfire, 32 * 9, 0, 'c', -1, 8, "homingfire" },
			{ 15, 8, 2, 40, 16, 18, 1, 0, spl_bomb, 32 * 9, 0, 'c', -1, 10, "bomb" },
		};
		ptr->setZSpellSet(zs);
		std::vector<spl::Spell> xs =
		{
			{ 20, 6, 2, 60, 15, 40, 2, 0, spl_circle_summon_candle, 32 * 12, 32 * 2, 'c', -1, 0, "candle" },
			{ 20, 6, 2, 60, 15, 40, 2, 0, spl_circle_summon_candle, 32 * 12, 32 * 2, 'c', -1, 0, "candle" },
			{ 20, 6, 2, 60, 15, 40, 2, 0, spl_circle_summon_candle, 32 * 12, 32 * 2, 'c', -1, 0, "candle" },
			{ 20, 6, 2, 60, 15, 40, 2, 0, spl_circle_summon_candle, 32 * 12, 32 * 2, 'c', -1, 0, "candle" },
			{ 20, 6, 2, 60, 15, 40, 2, 0, spl_circle_summon_candle, 32 * 12, 32 * 2, 'c', -1, 0, "candle" },
		};
		ptr->setXSpellSet(xs);
		ptr->SpeedDash(1.5);
		ptr->SpeedWalk(1.0);
		return ptr;
	}
	case 2:
	{
		using namespace enm;
		EnemyGroup::EnemyType t = EnemyGroup::Bee;
		EnemyGroup* ptr = gfw::generate<EnemyGroup>(tl);
		ptr->initialize(p, 1, t, "mbf");
		return ptr;
	}
	case 3:
	{
		AIPlayer* ptr = generate<AIPlayer>(tl);
		$P.ai.initialize("tamao");
		//ptr->initialize(0, 64 * 3, p, 2, true, &$P.ai, 150);
		ptr->initialize(REFPOINT_BOSS3_X, REFPOINT_BOSS3_Y, p, 2, true, &$P.ai, 130);
		ptr->CircleRef(REFPOINT_CIRCLE_FRAME_YELLOW_X, REFPOINT_CIRCLE_FRAME_YELLOW_Y);
		using namespace spl;
		std::vector<spl::Spell> zs =
		{
			{ 10, 8, 2, 60, 25, 35, 1, 0, spl::spl_laserslash, 32 * 11, 32, 'u', -1, 10, "laserslash" },
			{ 15, 8, 2, 60, 25, 35, 1, 0, spl::spl_laserslash, 32 * 11, 32, 'u', -1, 10, "laserslash" },
			{ 15, 8, 2, 60, 25, 35, 1, 0, spl::spl_laserslash, 32 * 11, 32, 'u', -1, 10, "laserslash" },
			{ 15, 8, 2, 60, 25, 35, 1, 0, spl::spl_laserslash, 32 * 11, 32, 'u', -1, 10, "laserslash" },
			{ 2, 10, 2, 90, 18, 20, 1, 45, spl_starstream, 0, 32, 'c', -1, 8, "starstream" },

			//{ 5, 12, 1, 55, 70, 15, 1, spl_dashattack, 32 * 5, 0, 'c', -1, "dashattack" },

		};
		ptr->setZSpellSet(zs);
		std::vector<spl::Spell> xs =
		{
			{ 10, 17, 1, 40, 40, 25, 1, 0, spl::spl_barrier, 32 * 6, 0, 'u', -1, 0, "barrier" },
			{ 10, 17, 1, 40, 40, 25, 1, 0, spl::spl_barrier, 32 * 6, 0, 'u', -1, 0, "barrier" },
			{ 10, 17, 1, 40, 40, 25, 1, 0, spl::spl_barrier, 32 * 6, 0, 'u', -1, 0, "barrier" },
			{ 10, 17, 1, 40, 40, 25, 1, 0, spl::spl_barrier, 32 * 6, 0, 'u', -1, 0, "barrier" },
			{ 10, 17, 1, 40, 40, 25, 1, 0, spl::spl_barrier, 32 * 6, 0, 'u', -1, 0, "barrier" },
		};
		ptr->setXSpellSet(xs);
		return ptr;
	}
	case 4:
	{
		using namespace enm;
		BigEye* ptr = gfw::generate<BigEye>(tl);
		ptr->initialize(p, 1);
		return ptr;
	}
	case 5:
	{
		AIPlayer* ptr = generate<AIPlayer>(tl);
		$P.ai.initialize("mari");
		ptr->initialize(REFPOINT_BOSS4_X, REFPOINT_BOSS4_Y, p, 2, true, &$P.ai, 150);
		ptr->CircleRef(REFPOINT_CIRCLE_FRAME_GRAY_X, REFPOINT_CIRCLE_FRAME_GRAY_Y);
		ptr->SpeedWalk(1.5);
		ptr->SpeedDash(3.0);
		ptr->MPInterval(30);
		using namespace spl;
		/*
		int manacost;
		int anim_type, anim_num;
		int stuntime, frame_of_one_phase, actiontime, num_iteration;*/
		std::vector<spl::Spell> zs =
		{
			{ 8, 8, 2, 30, 15, 18, 1, 0, spl_shotgun, 32 * 9, 0, 'c', -1, 8, "shotgun" },
			{ 0, 10, 2, 70, 30, 36, 1, 0, spl_homingmissile, 32 * 9, 0, 'c', -1, 15, "missile" },
			{ 5, 8, 2, 30, 15, 18, 1, 0, spl_punch, 32 * 9, 0, 'c', -1, 5, "punch" },
			{ 5, 8, 2, 30, 15, 18, 1, 0, spl_punch, 32 * 9, 0, 'c', -1, 5, "punch" },
			{ 5, 8, 2, 40, 16, 18, 1, 0, spl_bomb, 32 * 9, 0, 'c', -1, 15, "bomb" },
		};
		ptr->setZSpellSet(zs);
		std::vector<spl::Spell> xs =
		{
			{ 20, 6, 2, 60, 15, 40, 2, 0, spl_circle_summon_lamp, 32 * 12, 32 * 2, 'c', -1, 0, "lamp" },
			{ 10, 6, 2, 60, 15, 40, 2, 0, spl_healbarrier, 32 * 12, 32 * 2, 'c', -1, 0, "healbarrier" },
			{ 15, 6, 2, 60, 15, 40, 2, 0, spl_circle_summon_ghost2, 32 * 12, 32 * 2, 'c', -1, 0, "ghost2" },
			{ 15, 6, 2, 60, 15, 40, 2, 0, spl_circle_summon_ghost2, 32 * 12, 32 * 2, 'c', -1, 0, "ghost2" },
			{ 15, 6, 2, 60, 15, 40, 2, 0, spl_circle_summon_ghost2, 32 * 12, 32 * 2, 'c', -1, 0, "ghost2" },
		};
		ptr->setXSpellSet(xs);
		return ptr;
	}
	case 6:
	{
		using namespace enm;
		EnemyGroup::EnemyType t = EnemyGroup::Soldier;
		EnemyGroup* ptr = gfw::generate<EnemyGroup>(tl);
		ptr->initialize(p, 1, t, "mbg");
		return ptr;
	}
	case 7:
	{
		AIPlayer* ptr = generate<AIPlayer>(tl);
		$P.ai.initialize("kyo2");
		ptr->initialize(REFPOINT_BOSS1_X, REFPOINT_BOSS1_Y, p, 1, true, &$P.ai, 220);
		ptr->CircleRef(REFPOINT_CIRCLE_FRAME_RED_X, REFPOINT_CIRCLE_FRAME_RED_Y);
		ptr->MaxNumJump(5);
		ptr->SpeedJump(2.5);
		ptr->Phys().mass = 0.25;
		using namespace spl;
		/*
		int manacost;
		int anim_type, anim_num;
		int stuntime, frame_of_one_phase, actiontime, num_iteration;*/
		std::vector<spl::Spell> zs =
		{
			{ 15, 8, 2, 60, 25, 28, 1, 0, spl_homingfire3, 32 * 9, 0, 'c', -1, 8, "homingfire3" },
			{ 15, 8, 2, 60, 25, 28, 1, 0, spl_sword3, 32 * 9, 0, 'c', -1, 12, "sword3" },
			{ 15, 8, 2, 60, 25, 28, 1, 0, spl_homingfire, 32 * 9, 0, 'c', -1, 8, "homingfire3" },
			{ 15, 8, 2, 60, 25, 28, 1, 0, spl_homingfire, 32 * 9, 0, 'c', -1, 8, "homingfire" },
			{ 0, 6, 2, 60, 15, 40, 2, 0, spl_willowispperp3, 32 * 12, 32 * 2, 'c', -1, 8, "willowispperp3" },
		};
		ptr->setZSpellSet(zs);
		std::vector<spl::Spell> xs =
		{
			{ 35, 6, 2, 60, 15, 40, 2, 0, spl_circle_summon_candle, 32 * 12, 32 * 2, 'c', -1, 0, "pumpkin" },
			{ 20, 6, 2, 60, 15, 40, 2, 0, spl_circle_summon_candle, 32 * 12, 32 * 2, 'c', -1, 0, "candle" },
			{ 35, 6, 2, 60, 15, 40, 2, 0, spl_circle_flame_nearest, 32 * 12, 32 * 2, 'c', -1, 15, "circle_flame_nearest" },
			{ 20, 6, 2, 60, 15, 40, 2, 0, spl_circle_summon_candle, 32 * 12, 32 * 2, 'c', -1, 0, "candle" },
			{ 0, 6, 2, 60, 15, 40, 2, 0, spl_willowispperp3, 32 * 12, 32 * 2, 'c', -1, 8, "willowispperp3" },
		};
		ptr->setXSpellSet(xs);
		ptr->SpeedDash(1.5);
		ptr->SpeedWalk(1.0);

		using namespace enm;
		EnemyGroup::EnemyType t = EnemyGroup::Hand2;
		EnemyGroup* ptr2 = gfw::generate<EnemyGroup>(tl);
		ptr2->initialize(p, 1, t, "mbg");
		return ptr;
	}
	case 8:
	{
		AIPlayer* ptr = generate<AIPlayer>(tl);
		$P.ai.initialize("wakaba");
		//ptr->initialize(0, 64 * 3, p, 2, true, &$P.ai, 150);
		ptr->initialize(REFPOINT_BOSS5_X, REFPOINT_BOSS5_Y, p, 2, true, &$P.ai, 140);
		ptr->CircleRef(REFPOINT_CIRCLE_FRAME_LIGHTBLUE_X, REFPOINT_CIRCLE_FRAME_LIGHTBLUE_Y);
		using namespace spl;
		std::vector<spl::Spell> zs =
		{
			{ 5, 8, 2, 60, 25, 35, 1, 0, spl::spl_arrow3, 32 * 11, 32, 'c', -1, 10, "arrow3" },
			{ 15, 8, 2, 60, 25, 35, 1, 0, spl::spl_laserslash, 32 * 11, 32, 'u', -1, 1, "laserslash" },
			{ 15, 8, 2, 60, 25, 35, 1, 0, spl::spl_laserslash, 32 * 11, 32, 'u', -1, 1, "laserslash" },
			{ 15, 8, 2, 60, 25, 35, 1, 0, spl::spl_laserslash, 32 * 11, 32, 'u', -1, 1, "laserslash" },
			{ 1, 8, 2, 60, 25, 35, 1, 0, spl::spl_punch, 32 * 11, 32, 'c', -1, 8, "punch" },


			//{ 5, 12, 1, 55, 70, 15, 1, spl_dashattack, 32 * 5, 0, 'c', -1, "dashattack" },

		};
		ptr->setZSpellSet(zs);
		std::vector<spl::Spell> xs =
		{
			{ 15, 17, 1, 30, 20, 15, 1, 0, spl::spl_circle_summon_fairy_blue, 32 * 6, 0, 'u', -1, 0, "circle_summon_fairy_blue" },
			{ 10, 17, 1, 30, 20, 15, 1, 0, spl::spl_barrier, 32 * 6, 0, 'u', -1, 0, "barrier" },
			{ 10, 17, 1, 30, 20, 15, 1, 0, spl::spl_barrier, 32 * 6, 0, 'u', -1, 0, "barrier" },
			{ 10, 17, 1, 30, 20, 15, 1, 0, spl::spl_barrier, 32 * 6, 0, 'u', -1, 0, "barrier" },
			{ 10, 17, 1, 30, 20, 15, 1, 0, spl::spl_barrier, 32 * 6, 0, 'u', -1, 0, "barrier" },
		};
		ptr->setXSpellSet(xs);
		return ptr;
	}
	case 9:
	{
		using namespace enm;
		BigFish* ptr = gfw::generate<BigFish>(tl);
		ptr->initialize(p, 1);
		return ptr;
	}
	
	}
}