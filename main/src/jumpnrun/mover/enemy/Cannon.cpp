#pragma once
#define _USE_MATH_DEFINES
#include"jumpnrun/mover/enemy/Enemy.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/mover/bullet/Bullet.h"
#include"jumpnrun/mover/Barrier.h"
#include"jumpnrun/GRAPHICS.h"
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

RegisterMoverClass(Cannon);

void Cannon::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, bool turned)
{
	Enemy::initialize(refx, refy, p, owner);
	width = height = 32;
	direction = 0;
	margin_top = 0;
	this->turned = turned;
	direction = turned ? 4 : 0;
	this->smashable = false;
	hp = hp_max = 25;
	writeName("character_enemy_cannon", name);
}

void Cannon::run(jnr::JumpnRunData& data)
{
	Enemy::run(data);
}

void Cannon::actz(phsc::Physics& phys, jnr::JumpnRunData& data)
{
	auto v = $V::polar(8.0, M_PI / 4.0 * direction);
	Effect* e = gfw::generate<Effect>(*tl);
	e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, Center() + v + $V( - 16, -16.0), 2, $V(0, 0), 0);
	Bullet* b = gfw::generate<Bullet>(*tl);
	b->initialize(REFPOINT_BULLET_X, REFPOINT_BULLET_Y, Center() + v + $V(- 16, -16.0), Owner(), 5, v, 0);
	$P.sound_stack.push_back($P.snd_ignition);
}

void Cannon::think(jnr::JumpnRunData& data)
{
	mc.poll();
	if (age % 105 != 0)
		return;

	EnemySearcher es;
	es.initialize(this);
	auto v = es.search(64 * 5);
	auto o = Owner();
	v = myutil::filter([o](const Object::Pipe & p)
	{
		const Mover* mvr = static_cast<Mover*>(p.Dst(*tl));
		auto myo = mvr->Owner();
		char c = char(myo);
		return myo != o && mvr->Name().substr(0, 9) == "character";
	}, v);
	if (!v.empty())
	{
		auto p = P();
		std::sort(v.begin(), v.end(), [p](const Object::Pipe & p1, const Object::Pipe & p2)
		{return (p - static_cast<Mover*>(p1.Dst(*tl))->Center()).sqsum()
		< (p - static_cast<Mover*>(p2.Dst(*tl))->Center()).sqsum(); });
		auto dp = static_cast<Mover*>(v.begin()->Dst(*tl))->Center() - p;
		double t = dp.theta() + M_PI / 8.0;
		if (t >= 2 * M_PI)
			t -= 2 * M_PI;
		int qtheta = t / M_PI * 4.0;
		int prev_dir = direction;
		direction = qtheta;
		if (direction == 1 || direction == 2)
			direction = 0;
		else if (direction == 3)
			direction = 4;
		turned = direction >= 3 && direction <= 6;

		if (direction == prev_dir)
		{
			mc.push_z = true;
		}
	}
	else
	{
		if (age % 40 == 0)
		{
			direction++;
			direction %= 8;

		}
	}
}

void Cannon::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int alpha = timer_invinc.moving() ? 0xaaffffff : 0xffffffff;
	int offset_refx = 0;
	switch (direction)
	{
	default:
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		break;
	case 5:
	case 7:
		offset_refx = width;
		break;
	case 6:
		offset_refx = width * 2;
		break;
	}
	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx + offset_refx, refy, width, height, turned, alpha);
}