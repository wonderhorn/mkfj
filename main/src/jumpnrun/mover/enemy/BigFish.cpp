#pragma once
#define _USE_MATH_DEFINES
#include"jumpnrun/mover/enemy/Enemy.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/mover/Barrier.h"
#include"jumpnrun/spell/Spell.h"
#include"jumpnrun/mover/bullet/Bullet.h"
#include"jumpnrun/GRAPHICS.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/MoverUtilities.h"
#include"jumpnrun/mover/StageAccessor.h"
#include"utils/MyCppUtil.h"
#include<cmath>
#include<functional>
#include<algorithm>
using namespace jnr;
using namespace enm;
using namespace gfw;
using namespace gmtr;
#define $V Vector2D

RegisterMoverClass(BigFish);

void BigFish::initialize(gmtr::Vector2D p, OWNERHANDLE owner, bool turned)
{
	Enemy::initialize(refx, refy, p, owner);
	power = 15;
	hp = hp_max = 100;
	ai_timer = 0;
	//decel = 0.25;
	width = 128;
	height = 80;
	//where_to_go = true;
	margin_bottom = margin_side = margin_top = 0;
	phys.mass = 0;
	direction = 3;
	this->turned = turned;

	for (int i = 0; i < length_body; i++)
	{
		//int dx = 0;
		int dx = (turned * 2 - 1) * (16 * (i - 1) - 46) + width * turned;
		double theta = sin((double(age % 30) / 30 * 2 * M_PI));
		double dy = 4 * i * i * sin(theta);
		points_body[i] = P() + $V(dx, dy);
		//points_body[i] = points_body[i] + (P() + $V(dx, dy) - points_body[i]) * 0.1;
	}

	writeName("character_enemy_bigfish", name);
}

void BigFish::drive(phsc::Physics& phys, jnr::JumpnRunData& data)
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
				v_target.y = speed_walk / 2;
			}
		}
		else if (Controller().Up().isDown())
		{
			//phys.v.x = -1.5;
			if (location == 'w')
			{
				if (v_target.y > -speed_walk)
				{
					//status = eStatus::Walking;
					v_target.y = -speed_walk / 2;
				}
			}
			else
			{
				v_target.y = 0.0;
			}
		}
		else
		{
			v_target.y = 0.0;
		}
	}

	//attack
	if (Controller().Z().pushed())
	{
		actz(phys, data);
	}
	if (Controller().X().pushed())
	{
		//actx(phys, data);
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

void BigFish::run(jnr::JumpnRunData& data)
{
	Enemy::run(data);
	status = eStatus::Flying;

	//body points
	for (int i = 0; i < length_body; i++)
	{
		//int dx = 0;
		//int dx = (turned * 2 - 1) * (16 * (i - 1) - 46) + width * turned;
		//int dx = (turned * 2 - 1) * (16 * (i + !turned));
		int dx = (turned * 2 - 1) * (16 * (i + turned) -48) + (width -32) * turned;
		double theta = sin((double((age + 5 * i) % 45) / 45 * 2 * M_PI));
		//double dy = i * i * sin(theta);
		double dy = (i / 2 + 12) * sin(theta);
		//points_body[i] = P() + $V(dx, dy);
		$V dp = (P() + $V(dx, dy) - points_body[i]) * 0.05;
		if (dp.l2() > 8)
			dp = dp / dp.l2() * 8.0;
		points_body[i] = points_body[i] + dp;
	}
}

void BigFish::actz(phsc::Physics& phys, jnr::JumpnRunData& data)
{
	//spl::scatterurchin(*this, data);
	for (int i = 0; i < 4; i++)
	{
		double theta = M_PI * ((double)(i + 1) / 5) + M_PI;
		double dx = 4 * cos(theta);
		double dy = 2 * sin(theta);
		enm::Urchin* e = gfw::generate<enm::Urchin>(*this->tl);
		e->initialize(REFPOINT_URCHIN_X, REFPOINT_URCHIN_Y, this->Center() - $V(16, 16)
			, this->Owner(), false, 1.0, 45 * 60, 12);
		e->V($V(dx, dy));

	}
}

void BigFish::think(jnr::JumpnRunData& data)
{
	if (ai_timer)
	{
		ai_timer--;
	}
	else
	{
		ai_timer = 30;
		StageAccessor sa(*stg);
		sa.water_enterable = true;
		double dfw = sa.distanceToWall(Center(), turned, data);
		if (dfw < 96)
		{
			if (turned)
				direction = 0;
			else
				direction = 4;
		}

		double dff_uppper = sa.distanceToFloor(Center() + $V(0, 48), data);
		double dff_lower = sa.distanceToFloor(Center() + $V(0, -32), data);
		double dff = min(dff_uppper, dff_lower);

		if (dff > 200)
		{
			switch (direction)
			{
			default:
				break;
			case 0:
				direction = 3;
				break;
			case 6:
				direction = 3;
				break;
			case 7:
				direction = 0;
				break;
			case 5:
				direction = 4;
				break;
			case 4:
				direction = 3;
				break;

			}
		}

		if (dff < 80)
		{
			switch (direction)
			{
			default:
				break;
			case 1:
				direction = 7;
				break;
			case 2:
				direction = 6;
				break;
			case 3:
				direction = 4;
				break;
			case 0:
				direction = 7;
				break;
			case 4:
				direction = 5;
				break;

			}
		}

		if ($P.rand.get() % 15 == 0)
			mc.push_z = true;

		/*
		if(direction == 4)
			direction = 0;
		else
			direction = 4;
		*/
		/*
		using namespace myutil;
		EnemySearcher es;
		es.initialize(this);
		auto v = es.search(256);
		auto o = this->Owner();
		v = myutil::filter([o](const Object::Pipe & p)
		{
			const Mover* mvr = static_cast<Mover*>(p.Dst(*tl));
			auto myo = mvr->Owner();
			char c = char(myo);
			return myo != o && mvr->Name().substr(0, 9) == "character";
		}, v);

		if (v.empty() || $P.rand.get() % 4 == 0)
		{
			direction = $P.rand.get() % 8;

			StageAccessor sa(*stg);
			double dff = sa.distanceToFloor(Center(), data);
			if (dff < 64)
			{
				switch (direction)
				{
				default:
					break;
				case 3:
					direction = 2;
					break;
				case 4:
					direction = 0;
					break;
				case 5:
					direction = 6;
					break;
				}
			}

			ai_timer = 120;
		}
		else
		{
			auto p = Center();
			std::sort(v.begin(), v.end(), [p](const Object::Pipe & p1, const Object::Pipe & p2)
			{return (p - static_cast<Mover*>(p1.Dst(*tl))->Center()).sqsum()
				< (p - static_cast<Mover*>(p2.Dst(*tl))->Center()).sqsum(); });
			auto dp = static_cast<Mover*>(v.begin()->Dst(*tl))->Center() + $V(0, 12) - p;

			double t = dp.theta() + M_PI / 8.0;
			if (t >= 2 * M_PI)
				t -= 2 * M_PI;
			int qtheta = t / M_PI * 4.0;
			int prev_dir = direction;
			direction = qtheta;
			ai_timer = 6;

			if ($P.rand.get() % 3 != 0)
			{
				mc.push_z = true;
			}
		}
		*/
	}



	/*double dff = sa.distanceToWall(Center(), turned, data);
	if (dff < 24)
	{
		switch (direction)
		{
		default:
			break;
		case 6:
			direction = 3;
			break;
		case 7:
			direction = 0;
			break;
		case 5:
			direction = 4;
			break;
		}
	}*/

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

void BigFish::die(jnr::JumpnRunData& data)
{
	/*if (this->Owner() != OWNERHANDLE(0))
	{
	data.num_enemies_beaten++;
	ItemDropper().drop(*tl, Center(), Name());
	}*/
	$P.sound_stack.push_back($P.snd_punch);

	alive = false;
	Vector2D center = phys.p + Vector2D(width / 2 - 16, height / 2 - 16);
	generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_X, REFPOINT_EXPLOSION_Y, 32, 32
		, center, 10, Vector2D(0, 0), 0, false, 0.25, 0.25, 8);
	generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32
		, center, 4, Vector2D(0, 0), 0);
	//generate<Effect>(*tl)->initialize(refx, refy, width, height
	//	, phys.p, 4, Vector2D(0, 0), 0, Turned(), 1.0, 0, 16);
	//Character::die(data);
}


void BigFish::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int offset_y = 1;
	if (!onSomething())
		offset_y = age / 20 % 2;
	int offset_x = age % 6 < 3;
	int alpha = timer_invinc.moving() ? 0xaaffffff : 0xffffffff;
	int dir = turned * 2 - 1;

	//head
	graphics->draw($P.img_bigfish
		, phys.p.x + (width - 48) * !turned - data.camera.X(), phys.p.y - data.camera.Y() + ((age / 45) % 2), 0
		//, 0, 144 * 1, 48, height, !turned, alpha);
		, 0, 144 * ((age / 90) % 2), 48, height, !turned, alpha);

	graphics->draw($P.img_bigfish
		, phys.p.x + 48 * turned - data.camera.X(), phys.p.y - data.camera.Y() + ((age / 45) % 2), 0
		, 48, 0, width - 48, height, !turned, alpha);

	//tail
	for (int i = 0; i < length_body; i++)
	{
		graphics->draw($P.img_bigfish, points_body[i].x - data.camera.X(), points_body[i].y - data.camera.Y(), 0
			, 128 + 19 * (i / 2), 0, 19, 80, !turned, alpha);
	}
	//fin
	graphics->draw($P.img_bigfish,
		points_body[length_body - 1].x + 16 * dir - 16 * !turned - data.camera.X(),
		points_body[length_body - 1].y + 12 + (age / 30 % 2) * 3 - data.camera.Y(), 0
		, 32, 96, 32, 48, !turned, alpha);
	graphics->draw($P.img_bigfish,
		phys.p.x + (width - 32) * !turned + dir * 48 - data.camera.X()
		, phys.p.y - data.camera.Y() + height - 32 + ((age / 45) % 2), 0
		, 0, 96, 32, 32, !turned, alpha);

	//graphics->draw($P.img_bigfish, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
	//	, 0, 0, width, height, turned, alpha);
	//$V eyep(-turned * 8 + 4 , 0);

	if (Constants::debugging)
	{
		graphics->drawString(gfw::num2str(this->hp), phys.p.x - data.camera.X(), phys.p.y - data.camera.Y() + height, 48, 20);
	}
}
