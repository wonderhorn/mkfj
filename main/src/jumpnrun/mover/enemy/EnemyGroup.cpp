#include"jumpnrun/mover/enemy/EnemyGroup.h"
#include"jumpnrun/mover/enemy/Enemy.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/GRAPHICS.h"
using namespace gfw;
using namespace jnr;
using namespace enm;
#define $V gmtr::Vector2D

RegisterMoverClass(EnemyGroup);

static void cpyString(char buf[8], const std::string& str)
{
	assert(str.size() < 8);
	for (int i = 0; i < str.size(); i++)
	{
		buf[i] = str[i];
	}
	buf[7] = '\0';
}

void EnemyGroup::initialize(gmtr::Vector2D p, OWNERHANDLE owner, EnemyType type, const std::string& fid)
{
	Character::initialize(-1, -1, p, owner);
	cpyString(this->flad_id, fid);
	this->type = type;
	switch (type)
	{
	case Bee:
		num_enemy_total = 8;
		num_enemy_once = 4;
		break;
	case Soldier:
		num_enemy_total = 10;
		num_enemy_once = 5;
		break;
	case Hand:
		num_enemy_total = 20;
		num_enemy_once = 4;
		break;
	case Hand2:
		num_enemy_total = 20;
		num_enemy_once = 2;
		break;

	default:
		assert(0);
		break;
	}
	hp = hp_max = num_enemy_total * 10;
	num_beaten = 0;
	num_generated = 0;
	for (int i = 0; i < 7; i++)
	{
		timers[i] = 0;
	}

	writeName("enemygroup", name);
}
void EnemyGroup::run(jnr::JumpnRunData& data)
{
	for (int i = 0; i < num_enemy_once; i++)
	{
		if (timers[i] > 0)
			timers[i]--;
		Pipe& p = pipes[i];
		if (auto* ptr = p.Dst(*tl))
		{
			Enemy* e = static_cast<Enemy*>(ptr);
			if (!e->alive)
			{
				num_beaten++;
				p = Pipe(nullptr, *tl);
				timers[i] = 180;
			}
		}
		else
		{
			if (num_generated < num_enemy_total && timers[i] == 0)
			{
				p = Pipe(genEnemy(), *tl);
				timers[i] = -1;
				num_generated++;

				//delay of other genration
				for (int i = 0; i < 7; i++)
				{
					if(timers[i] >= 0)
						timers[i] += 30;
				}

			}
		}
	}

	hp = (num_enemy_total - num_beaten) * 10;
	if (num_enemy_total - num_beaten == 0)
	{
		$P.flags[flad_id] = 1;
		die(data);
	}
}

enm::Enemy* EnemyGroup::genEnemy()
{
	switch (type)
	{
	case Bee:
	{
		enm::Bee* ptr = generate<enm::Bee>(*tl);
		ptr->initialize(REFPOINT_BEE_X, REFPOINT_BEE_Y, P(), owner);
		generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32
			, P() + $V($P.rand.get() % 12, 0)
			, 12, $V(0, 0), 0, 8, 5.0);
		return (Enemy*)ptr;
	}
		break;
	case Hand:
	case Hand2:
	case Soldier:
	{
		enm::Hand* ptr = generate<enm::Hand>(*tl);
		ptr->initialize(REFPOINT_HAND_X, REFPOINT_HAND_Y, P(), owner);
		generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32
			, P() + $V($P.rand.get() % 12, 0)
			, 12, $V(0, 0), 0, 8, 5.0);
		return (Enemy*)ptr;
	}
		break;
	}
}

void EnemyGroup::die(jnr::JumpnRunData& data)
{
	alive = false;
}