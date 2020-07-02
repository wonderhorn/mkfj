#include"jumpnrun/mover/SystemObject.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/mover/Mover.h"
#include"jumpnrun/mover/Item.h"
#include"jumpnrun/mover/enemy/Enemy.h"
#include"jumpnrun/GRAPHICS.h"
#define $V gmtr::Vector2D
using namespace jnr;
using namespace gfw;

void ItemPutter::initialize()
{
	Object::initialized();
	isMover = false;
	age = 0;
	writeName("itemputter", name);
}

void ItemPutter::finalize(){}
void ItemPutter::run(jnr::JumpnRunData& data)
{
	if (age % (60 * 5) == 0)
	{
		int x = $P.rand.get() % Mover::stg->Width();
		int y = $P.rand.get() % Mover::stg->Height();
		generate<Item>(*tl)->initialize(REFPOINT_JEWEL_X, REFPOINT_JEWEL_Y + 32, $V(x, y), -1, 10, $V(0, 0), 1);
	}
	age++;
}


void EnemyPutter::initialize()
{
	Object::initialized();
	isMover = false;
	age = 0;
	writeName("itemputter", name);
}

void EnemyPutter::finalize(){}
void EnemyPutter::run(jnr::JumpnRunData& data)
{
	if (age % (60 * 8) == 0)
	{
		int x = $P.rand.get() % Mover::stg->Width();
		int y = $P.rand.get() % Mover::stg->Height();
		generate<enm::Boy>(*tl)->initialize(REFPOINT_BOY_X, REFPOINT_BOY_Y, $V(x, y), 2);
	}
	age++;
}