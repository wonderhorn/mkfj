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

RegisterMoverClass(BigEye);

void BigEye::initialize(gmtr::Vector2D p, OWNERHANDLE owner, bool turned)
{
	Enemy::initialize(refx, refy, p, owner);
	power = 12;
	hp = hp_max = 100;
	ai_timer = 0;
	//decel = 0.25;
	width = 96;
	height = 96;
	//where_to_go = true;
	margin_bottom = margin_side = margin_top = 0;
	phys.mass = 0;
	direction = 3;
	eyev.x = eyev.y = 0;
	this->turned = turned;

	for (int i = 0; i < max_num_tentacles; i++)
	{
		tentacles[i] = Pipe(nullptr, *tl);
	}

	writeName("character_enemy_bigeye", name);
}

void BigEye::drive(phsc::Physics& phys, jnr::JumpnRunData& data)
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
	}


	//attack
	if (Controller().Z().pushed())
	{
		std::vector<int> v(max_num_tentacles);
		for (int i = 0; i < max_num_tentacles; i++)
			v[i] = i;
		for (int i = 0; i < max_num_tentacles; i++)
		{
			int idx = i + $P.rand.get() % (max_num_tentacles - i);
			int tmp = v[i];
			v[i] = v[idx];
			v[idx] = v[i];
		}

		for (int i = 0; i < max_num_tentacles; i++)
		{
			int idx = v[i];
			if (!tentacles[idx].Dst(*tl))
			{
				double theta = 2.0 * M_PI / max_num_tentacles * idx;
				$V direction = $V::polar(1, theta);
				double vx = Turned() ? -1.0 : 1.0;
				BigEyeTentacle* b = gfw::generate<BigEyeTentacle>(*tl);
				b->initialize(*this, 5, direction * 2, direction * 48 + $V(width / 2 , height / 2 ), 0.1);
				tentacles[i] = Pipe(b, *tl);
				//b->snd_hit = $P.snd_hit;
				//b->LifeTime(8);
				//b->Radius(8);
				timer_stun.reset(90);
				v_target = $V(0, 0);
				$P.sound_stack.push_back($P.snd_swing);
				//spl::Whip(*this, data);
				break;
			}
		}
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

void BigEye::run(jnr::JumpnRunData& data)
{
	for (int i = 0; i < max_num_tentacles; i++)
	{
		if (BigEyeTentacle* t = static_cast<BigEyeTentacle*>(tentacles[i].Dst(*tl)))
		{
			if (!t->alive)
			{
				tentacles[i] = Pipe(nullptr, *tl);
			}
		}
	}


	Enemy::run(data);
	status = eStatus::Flying;

}

void BigEye::think(jnr::JumpnRunData& data)
{
	StageAccessor sa(*stg);

	if (ai_timer)
	{
		ai_timer--;
	}
	else
	{
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

			ai_timer = 120;
		}
		else
		{
			auto p = Center();
			std::sort(v.begin(), v.end(), [p](const Object::Pipe & p1, const Object::Pipe & p2)
			{return (p - static_cast<Mover*>(p1.Dst(*tl))->Center()).sqsum()
			< (p - static_cast<Mover*>(p2.Dst(*tl))->Center()).sqsum(); });
			auto dp = static_cast<Mover*>(v.begin()->Dst(*tl))->Center() + $V(0, 12) - p;
			if (dp.l2() != 0)
			{
				eyev = dp / dp.l2() * 2;
			}
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
		if (dff > 220)
		{
			switch (direction)
			{
			default:
				break;
			case 0:
				direction = 4;
				break;
			case 1:
				direction = 3;
				break;
			case 7:
				direction = 5;
				break;
			}
		}
	}

	if (age % 20 == 0 && $P.rand.get() % 3 == 0)
	{
		if (v_reaction.x > 0.0 && direction == 3)
		{
			direction = 7;
			ai_timer = 120;
		}
		if (v_reaction.x > 0.0 && direction == 4)
		{
			direction = 0;
			ai_timer = 120;
		}
		if (v_reaction.x > 0.0 && direction == 5)
		{
			direction = 1;
			ai_timer = 120;
		}
		if (v_reaction.x < 0.0 && direction == 7)
		{
			direction = 3;
			ai_timer = 120;
		}
		if (v_reaction.x < 0.0 && direction == 0)
		{
			direction = 4;
			ai_timer = 120;
		}
		if (v_reaction.x < 0.0 && direction == 1)
		{
			direction = 5;
			ai_timer = 120;
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

void BigEye::die(jnr::JumpnRunData& data)
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
		, center + $V(0, -32), 12, Vector2D(0, 0), 0, false, 0.25, 0.25, 8);
	generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_X, REFPOINT_EXPLOSION_Y, 32, 32
		, center + $V(24, 0), 12, Vector2D(0, 0), 0, false, 0.25, 0.25, 8);
	generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_X, REFPOINT_EXPLOSION_Y, 32, 32
		, center + $V(-24, 0), 12, Vector2D(0, 0), 0, false, 0.25, 0.25, 8);
	generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32
		, center, 4, Vector2D(0, 0), 0);
	//generate<Effect>(*tl)->initialize(refx, refy, width, height
	//	, phys.p, 4, Vector2D(0, 0), 0, Turned(), 1.0, 0, 16);
	//Character::die(data);
}


void BigEye::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int offset_y = 1;

	if (!onSomething())
		offset_y = age / 20 % 2;

	int offset_x = age % 6 < 3;

	int alpha = timer_invinc.moving() ? 0xaaffffff : 0xffffffff;
	graphics->draw($P.img_bigeye, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, 0, 96, width, height, turned, alpha);
	graphics->draw($P.img_bigeye, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, 0, 0, width, height, turned, alpha);
	//$V eyep(-turned * 8 + 4 , 0);
	$V eyep = eyev;
	graphics->draw($P.img_bigeye, phys.p.x + eyep.x + 32 - data.camera.X(), phys.p.y + eyep.y + 32 - data.camera.Y(), 0
		, 192, 0, 32, 32, turned, alpha);

	for (int i = 0; i < max_num_tentacles * 2; i++)
	{

		double theta = 2.0 * M_PI / (max_num_tentacles * 2) * i;
		$V direction = $V::polar(1, theta) * 48;
		auto p = Center() + direction - $V(16, 16);
		graphics->drawSpinResize($P.img_bigeye
			, p.x - data.camera.X(), p.y - data.camera.Y(), 0,
			192, 32, 32, 32, false, 0xffffffff, 1, 1, theta);
	}

	if (Constants::debugging)
	{
		graphics->drawString(gfw::num2str(this->hp), phys.p.x - data.camera.X(), phys.p.y - data.camera.Y() + height, 48, 20);
	}
}
