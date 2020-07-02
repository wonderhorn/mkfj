#include"jumpnrun/mover/block/block.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/mover/effect/Circle.h"
#include"jumpnrun/mover/Barrier.h"
#include"jumpnrun/mover/bullet/Bullet.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/spell/Spell.h"
#include"jumpnrun/GRAPHICS.h"
using namespace jnr;
using namespace gfw;
using namespace gmtr;
#define $V Vector2D

RegisterMoverClass(BombBox);

void BombBox::initialize(int refx, int refy, gmtr::Vector2D p, OWNERHANDLE owner)
{
	Block::initialize(refx, refy, p, owner);
	hp = hp_max = 3;
	writeName("block_bombbox", name);
}

void BombBox::run(jnr::JumpnRunData& data)
{
	Character::run(data);
	if (flag_id[0] > 0 && $P.flags[flag_id])
	{
		int f = $P.flags[flag_id];
		die(data);
	}
}

void BombBox::die(jnr::JumpnRunData& data)
{
	alive = false;
	Vector2D center = phys.p + Vector2D(width / 2 - 16, height / 2 - 16);
	generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, center, 2, $V(0.0, 0.0), 0);
	generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, center, 4, $V(0.0, 0.0), 0);
	generate<Explosion>(*tl)->initialize(REFPOINT_EXPLOSION_X, REFPOINT_EXPLOSION_Y, P(), -1, 15);

	//Character::die();
}
