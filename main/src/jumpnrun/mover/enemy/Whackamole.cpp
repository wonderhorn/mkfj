#pragma once
#include"jumpnrun/mover/enemy/Enemy.h"
#include"jumpnrun/mover/enemy/Whackamole.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/mover/Barrier.h"
#include"jumpnrun/mover/Item.h"
#include"jumpnrun/GRAPHICS.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/stage/ItemDropper.h"
using namespace jnr;
using namespace enm;
using namespace gfw;
using namespace gmtr;
#define $V Vector2D

RegisterMoverClass(Whackamole);


static void cpyString(char buf[8], const std::string& str)
{
	assert(str.size() < 8);
	for (int i = 0; i < str.size(); i++)
	{
		buf[i] = str[i];
	}
	buf[7] = '\0';
}

void Whackamole::initialize(gmtr::Vector2D p, OWNERHANDLE owner, bool turned, 
	const std::string& flag_id)
{
	Character::initialize(refx, refy, p, owner);
	refx = REFPOINT_WHACKAMOLE_X;
	refy = REFPOINT_WHACKAMOLE_Y;
	height = 32;
	this->interacting_with_items = false;
	is_damaging = false;
	smashable = false;
	timer_deactivate_damage = 0;
	this->lifetime = 999999;
	hp = hp_max = 999999;
	meter = 0;
	meter_max = 150;
	is_started = false;
	timer_game = 0;
	cpyString(this->flag_id, flag_id.c_str());
}

enm::Enemy* Whackamole::genEnemy(gmtr::Vector2D p)
{
	enm::Mole* ptr = generate<enm::Mole>(*tl);
	ptr->initialize(REFPOINT_WHACKAMOLE_X + 32, REFPOINT_WHACKAMOLE_Y, p, owner, true);
	//generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32
	//	, P() + $V($P.rand.get() % 12, 0)
	//	, 12, $V(0, 0), 0, 8, 5.0);
	return (Enemy*)ptr;
}

void Whackamole::run(jnr::JumpnRunData& data)
{
	if (age % 32 == 0 && meter > 0)
		meter -= 1;
	think(data);
	mc.poll();
	Character::run(data);
	if (lifetime < age)
		alive = false; //not die
	if (timer_deactivate_damage > 0)
		timer_deactivate_damage--;
	if (timer_deactivate_damage == 1)
		is_damaging = true;

	if (timer_game > 0)
		timer_game--;
	else 
	{
		if (is_started)
		{
			$P.sound_stack.push_back($P.snd_cancel);
			is_started = false;
			for (int i = 0; i < NUM_MOLE; i++)
			{
				Object::Pipe& p = pipes[i];
				if (auto* ptr = p.Dst(*tl))
				{
					Enemy* e = static_cast<Enemy*>(ptr);
					if (e->alive)
					{
						e->die(data);
					}
				}
			}
		}
	}

	if (flag_id[0] != 0 && $P.getFlag(flag_id)) 
	{
		if (!is_started)
		{
			is_started = true;
			timer_game = 60 * 20;

			for (int i = 0; i < NUM_MOLE; i++)
			{
				Object::Pipe& p = pipes[i];
				p = Pipe(genEnemy(P() + $V(1, 0) * 64 * i), *tl);
			}
		}
	}

	bool all_dead = true;
	for (int i = 0; i < NUM_MOLE; i++)
	{
		Object::Pipe& p = pipes[i]; 
		Enemy* ptr = static_cast<Enemy*>(p.Dst(*tl));
		if (ptr)
		{
			Enemy* e = static_cast<Enemy*>(ptr);
			if (!e->alive)
			{
				meter++;
				p = Pipe(nullptr, *tl);
				//timers[i] = 180;
			}
		}
		
		if (ptr && ptr->alive)
			all_dead = false;
	}

	if (all_dead && timer_game) 
	{
		$P.sound_stack.push_back($P.snd_powerup);
		timer_game = 0;

		generate<GetCard>(*tl)->initialize(
			REFPOINT_CARD_X, REFPOINT_CARD_Y,
			this->P() + $V(0, -96),
			0, 1.0,
			51);
		gfw::generate<Particles>(*tl)->initialize(
			REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32,
			this->P() + $V(0, -96) + $V(0, 0), 8, $V(0, 0), 0);
		alive = false;
	}
}

void Whackamole::think(jnr::JumpnRunData& data)
{}

static void renderNumber(BaseGraphics* graphics, jnr::JumpnRunData& data, const $V& p, int value)
{
	int x = p.x - data.camera.X() - 48;
	int y = p.y - data.camera.Y();

	char digit = (value / 10) % 10;
	graphics->draw($P.img_obj
		, x
		, y, 0
		, REFPOINT_COUNTER_X + 16 * digit, 
		REFPOINT_COUNTER_Y,
		16, 32, false);

	digit = (value) % 10;
	graphics->draw($P.img_obj
		, x + 16
		, y, 0
		, REFPOINT_COUNTER_X + 16 * digit,
		REFPOINT_COUNTER_Y,
		16, 32, false);
}

void Whackamole::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int alpha = timer_invinc.moving() ? 0xaaffffff : 0xffffffff;
	for(int i = 0; i < NUM_MOLE; i++)
	{
		graphics->draw($P.img_obj
			, phys.p.x - data.camera.X() + 64 * i
			, phys.p.y - data.camera.Y() + 8, 0
			, refx, refy, width, height, turned, alpha);

		Object::Pipe& p = pipes[i];
		bool mole_alive = false;
		Enemy* ptr = static_cast<Enemy*>(p.Dst(*tl));
		if (ptr)
		{
			Enemy* e = static_cast<Enemy*>(ptr);
			if (e->alive)
			{
				mole_alive = true;
			}
		}

		int ofset_refx = 32 * (!mole_alive);
		graphics->draw($P.img_obj
			, phys.p.x - data.camera.X() + 64 * i
			, phys.p.y - data.camera.Y() + 48, 0
			, REFPOINT_WHACKAMOLE_X + 64 + ofset_refx, REFPOINT_WHACKAMOLE_Y
			, width, height, turned, alpha);

	}
	renderNumber(graphics, data, P(), timer_game / 60);
}


int Whackamole::damage(const Damage& d)
{
	
	return eDamage::Through;
}