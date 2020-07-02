#pragma once
#define _USE_MATH_DEFINES
#include"jumpnrun/mover/enemy/Enemy.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/mover/bullet/Bullet.h"
#include"jumpnrun/mover/Barrier.h"
#include"jumpnrun/mover/block/block.h"
#include"jumpnrun/GRAPHICS.h"
#include"jumpnrun/stage/ItemDropper.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/MoverUtilities.h"
#include"utils/MyCppUtil.h"
#include<cmath>
#include<functional>
#include<algorithm>
using namespace jnr;
using namespace enm;
using namespace gfw;
using namespace gmtr;
#define $V Vector2D

void Hand::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, bool turned)
{
	Enemy::initialize(refx, refy, p, owner);
	power = 10;
	hp = hp_max = 5;
	ai_timer = 0;
	//decel = 0.25;
	height = 32;
	//where_to_go = true;
	margin_bottom = margin_side = margin_top = 0;
	phys.mass = 0;
	direction = 3;
	this->interacting_with_stages = false;
	this->interacting_with_blocks = false;
	this->turned = turned;
	go_out_of_camera = false;
	is_weak_to_water = true;
	writeName("character_enemy_hand", name);
}

void Hand::die(jnr::JumpnRunData& data)
{
	if (this->Owner() != OWNERHANDLE(0) && data.encount_boss == 7)
	{
		data.num_enemies_beaten++;
		ItemDropper().drop(*tl, Center(), Name());
	}
	$P.sound_stack.push_back($P.snd_punch);
	Character::die(data);
}

void Hand::drive(phsc::Physics& phys, jnr::JumpnRunData& data)
{
	if (!timer_stun.moving())
	{
		//walk
		bool rd = Controller().Right().isDown();
		if (rd)
		{
			turned = false;
			if (v_target.x < speed_walk)
			{
				status = eStatus::Walking;
				v_target.x = speed_walk;
			}
		}
		else if (Controller().Left().isDown())
		{
			turned = true;
			//phys.v.x = -1.5;

			if (v_target.x > -speed_walk)
			{
				status = eStatus::Walking;
				v_target.x = -speed_walk;
			}
		}
		else
		{
			v_target.x = 0.0;
		}

		if (Controller().Down().isDown())
		{
			if (v_target.y < speed_walk)
			{
				//status = eStatus::Walking;
				v_target.y = speed_walk;
			}
		}
		else if (Controller().Up().isDown())
		{
			//phys.v.x = -1.5;

			if (v_target.y > -speed_walk)
			{
				//status = eStatus::Walking;
				v_target.y = -speed_walk;
			}
		}
		else
		{
			v_target.y = 0.0;
		}

		//attack
		if (Controller().Z().pushed())
		{
			actz(phys, data);
		}
		if (Controller().X().pushed())
		{
			actx(phys, data);
		}
	}

	//drive and stop
	if (v_target.x > 0)
	{
		phys.v.x += accel;
		if (phys.v.x >= v_target.x)
			phys.v.x = v_target.x;
	}
	else if (v_target.x < 0)
	{
		phys.v.x -= accel;
		if (phys.v.x <= v_target.x)
			phys.v.x = v_target.x;
	}
	else
	{
		if (phys.v.x > 0)
		{
			phys.v.x -= decel;
			if (phys.v.x < 0)
				phys.v.x = 0;
		}
		else if (phys.v.x < 0)
		{
			phys.v.x += decel;
			if (phys.v.x > 0)
				phys.v.x = 0;
		}
	}

	if (v_target.y > 0)
	{
		phys.v.y += accel;
		if (phys.v.y >= v_target.y)
			phys.v.y = v_target.y;
	}
	else if (v_target.y < 0)
	{
		phys.v.y -= accel;
		if (phys.v.y <= v_target.y)
			phys.v.y = v_target.y;
	}
	else
	{
		if (phys.v.y > 0)
		{
			phys.v.y -= decel;
			if (phys.v.y < 0)
				phys.v.y = 0;
		}
		else if (phys.v.y < 0)
		{
			phys.v.y += decel;
			if (phys.v.y > 0)
				phys.v.y = 0;
		}
	}

}

void Hand::run(jnr::JumpnRunData& data)
{
	Enemy::run(data);
	status = eStatus::Flying;

}

void Hand::actx(phsc::Physics& phys, jnr::JumpnRunData& data)
{
	switch ($P.rand.get() % 2)
	{
	case 0:
		handitem = eHandItem::Bomb;
		break;
	case 1:
		handitem = eHandItem::Block;
		break;
	}
	timer_stun.reset(90);
}
void Hand::actz(phsc::Physics& phys, jnr::JumpnRunData& data)
{
	switch (handitem)
	{
	default:
		break;
	case Bomb:
	{
		jnr::Bomb* b = gfw::generate<jnr::Bomb>(*tl);
		b->initialize(REFPOINT_BOMB_X, REFPOINT_BOMB_Y, Center() + $V(-16, -16.0), Owner(), 5, $V(0, 0), 1, false);
		b->V($V(0, -1));
	}
		break;
	case Block:
	{
		jnr::Block* b = gfw::generate<jnr::Block>(*tl);
		b->initialize(REFPOINT_BLOCK_X + 32, REFPOINT_BLOCK_Y, P() + Vector2D(16, -48), -1);
		b->V($V(0, -1));
	}
	break;
	}
	handitem = eHandItem::None;
	timer_stun.reset(90);
}

void Hand::think(jnr::JumpnRunData& data)
{
	if (ai_timer)
	{
		ai_timer--;
	}
	else
	{
		if (handitem != None)
		{
			if (data.camera.Y() > Center().y - 48)
			{
				ai_timer = 40;
				direction = 2;
			}
			else
			{
				using namespace myutil;
				EnemySearcher es;
				es.initialize(this);
				auto v = es.search(64 * 6);
				auto o = this->Owner();
				v = myutil::filter([o](const Object::Pipe & p)
				{
					const Mover* mvr = static_cast<Mover*>(p.Dst(*tl));
					auto myo = mvr->Owner();
					char c = char(myo);
					return mvr->alive && myo != o && mvr->Name().substr(0, 9) == "character";
				}, v);

				if (v.empty() || $P.rand.get() % 6 == 0)
				{
					direction = $P.rand.get() % 8;
					ai_timer = 120;
				}
				else
				{
					auto p = Center();
					std::sort(v.begin(), v.end(), [p](const Object::Pipe & p1, const Object::Pipe & p2)
					{
						return (p - (static_cast<Mover*>(p1.Dst(*tl))->Center() - $V(0, 150))).sqsum()
							< (p - (static_cast<Mover*>(p2.Dst(*tl))->Center() - $V(0, 150))).sqsum();
					});
					auto dp = static_cast<Mover*>(v.begin()->Dst(*tl))->Center() + $V(0, -96) - p;

					//quantize direction into 0--8 integer
					double t = dp.theta() + M_PI / 8.0;
					if (t >= 2 * M_PI)
						t -= 2 * M_PI;
					int qtheta = t / M_PI * 4.0;
					int prev_dir = direction;
					direction = qtheta;
					ai_timer = 45;

					if (dp.l2() < 64)
					{
						direction = -1;
						mc.push_z = true;
					}
				}
			}
		}
		else
		{
			if (data.camera.Y() > Center().y + 48)
			{
				ai_timer = 60;
				mc.push_x = true;
				direction = 2;
			}
			else
			{
				direction = 6;
			}
		}
	}

	switch (direction)
	{
	default:
		break;
	case 0:
		mc.push_right = true;
		break;
	case 1:
		mc.push_right = true;
		mc.push_down = true;
		break;
	case 2:
		mc.push_down = true;
		break;
	case 3:
		mc.push_left = true;
		mc.push_down = true;
		break;
	case 4:
		mc.push_left = true;
		break;
	case 5:
		mc.push_left = true;
		mc.push_up = true;
		break;
	case 6:
		mc.push_up = true;
		break;
	case 7:
		mc.push_right = true;
		mc.push_up = true;
		break;
	}
}

void Hand::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	//int offset_x = age % 6 < 3;
	int offset_x = 0;
	if (handitem == Hand::eHandItem::None)
		offset_x = (age / 60 % 2);
	else
		offset_x = 2;

	int alpha = timer_invinc.moving() ? 0xaaffffff : 0xffffffff;

	switch (handitem)
	{
	case None:
		break;
	case Bomb:
		graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y() + 16, 0
			, REFPOINT_BOMB_X, REFPOINT_BOMB_Y, 32, 32, turned, alpha);
		break;
	case Block:
		graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y() + 16, 0
			, REFPOINT_BLOCK_X + 32, REFPOINT_BLOCK_Y, 32, 32, turned, alpha);
		break;
	}

	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx + width * offset_x, refy, width, height, turned, alpha);
	if (Constants::debugging)
	{
		graphics->drawString(gfw::num2str(this->hp), phys.p.x - data.camera.X(), phys.p.y - data.camera.Y() + height, 48, 20);
	}
}