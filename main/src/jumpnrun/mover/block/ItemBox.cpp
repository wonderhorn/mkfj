#include"jumpnrun/mover/block/block.h"
#include"jumpnrun/mover/Item.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/mover/effect/Circle.h"
#include"jumpnrun/mover/Barrier.h"
#include"jumpnrun/mover/block/Switch.h"
#include"jumpnrun/mover/bullet/Bullet.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/spell/Spell.h"
#include"jumpnrun/GRAPHICS.h"
using namespace jnr;
using namespace gfw;
using namespace gmtr;
#define $V Vector2D

//RegisterMoverClass(ItemBox);

static void cpyString(char buf[JNR_FLAG_SIZE], const std::string& str)
{
	assert(str.size() < JNR_FLAG_SIZE);
	for (int i = 0; i < str.size(); i++)
	{
		buf[i] = str[i];
	}
	buf[JNR_FLAG_SIZE - 1] = '\0';
}

void ItemBlock::initialize(int refx, int refy, gmtr::Vector2D p, OWNERHANDLE owner, int top_margin)
{
	Block::initialize(refx, refy, p, owner, 1, 5, top_margin);
	hp = hp_max = 3;
	writeName("block_itembox", name);
}

void ItemBlock::die(jnr::JumpnRunData& data)
{
	switch (item_type)
	{
	case Coin:
		generate<Item>(*tl)->initialize(REFPOINT_JEWEL_X, REFPOINT_JEWEL_Y + 32, P()
			, -1, 20, gmtr::Vector2D(0, 0), 0);

		break;
	case CommonCard:
		generate<GetCard>(*tl)->initialize(REFPOINT_CARD_X, REFPOINT_CARD_Y, P()
			, -1, 1, item_value);
		break;
	case HPCan:
		if ($P.getFlag(std::string(flag_id)) == 0)
		{
			auto* ptr = generate<jnr::HPCan>(*tl);
			ptr->initialize(REFPOINT_HPCAN_X, REFPOINT_HPCAN_Y, P()
				, -1, 1, flag_id);
			cpyString(ptr->flag_id, flag_id);
			ptr->LifeTime(99999);

		}
		break;
	case MPCan:
		if ($P.getFlag(std::string(flag_id)) == 0)

		{
			auto* ptr = generate<jnr::MPCan>(*tl);
			ptr->initialize(REFPOINT_MPCAN_X, REFPOINT_MPCAN_Y, P()
				, -1, 1, flag_id);
			cpyString(ptr->flag_id, flag_id);
			ptr->LifeTime(99999);
		}
		break;
	case Lever:
		//if ($P.getFlag(std::string(flag_id)) == 0)

		{
			Switch* ptr = gfw::generate<Switch>(*tl);
			ptr->initialize(REFPOINT_TOGGLESWITCH_X, REFPOINT_TOGGLESWITCH_Y, P(), -1);
			ptr->setSwitchType(true, -1);
			
			cpyString(ptr->flag_id, flag_id);
			ptr->setOn($P.getFlag(flag_id));
		}
		break;
	default:
		break;
	}

	alive = false;
	Vector2D center = phys.p + Vector2D(width / 2 - 16, height / 2 - 16);
	generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, center, 2, $V(0.0, 0.0), 0);
	generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, center, 4, $V(0.0, 0.0), 0);
	//Character::die();
}