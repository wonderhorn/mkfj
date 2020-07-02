#define _USE_MATH_DEFINES
#include<cmath>
#include"jumpnrun/mover/bullet/Bullet.h"
#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/MoverUtilities.h"
#include"jumpnrun/system/Parmanent.h"
#include"utils/MyCppUtil.h"
#include"jumpnrun/GRAPHICS.h"
#include<cmath>
#include<functional>
#include<algorithm>
using namespace jnr;
using namespace gfw;
using namespace gmtr;
#define $V gmtr::Vector2D

RegisterMoverClass(Bomb);


void Bomb::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v, double mass
	, bool rotate_to_direction, bool homing, bool yanim, Type type)
{
	Bullet::initialize(refx, refy, p, owner, power, v, mass, rotate_to_direction);

	writeName("bullet_bomb", name);
	margin_top = margin_side = margin_bottom = 8;
	this->rotate_to_direction = rotate_to_direction;
	isLighted = true;
	this->homing = homing;
	this->type = type;
	this->yanim = yanim;
}
void Bomb::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	if (chara.Owner() == this->Owner())
		return;
	$V center = phys.p + $V(width / 2, height / 2);
	if (chara.Shape().includes(center))
	{
		Damage dam;
		dam.power = power;
		dam.stun_time = Constants::time_stun;
		dam.invinc_time = 3;
		dam.isPhysical = false;
		auto smashv = this->phys.v.regularized();
		if (smashv.l2() == 0)
			smashv = $V(0, -1);
		dam.smash = smashv * 4.0 + $V(0, -4) - chara.V();
		dam.owner = this->Owner();
		if (chara.damage(dam) >= 0)
		{
			generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, phys.p, 2, $V(0.0, 0.0), 0);
			generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, phys.p, 4, $V(0.0, 0.0), 0);
			//chara.Smash(this->phys.v.regularized() * 4.0 + $V(0, -4));
			finalize();

			if (type == TExplosion)
			{
				if (isLighted && !chara.isWet())
				{
					generate<Explosion>(*tl)->initialize(REFPOINT_EXPLOSION_X, REFPOINT_EXPLOSION_Y, P(),
						-1, 12, $V(0, 0), 0, 7);
					generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32,
						P(), 6, Vector2D(0, 0), 0);
				}
			}
			else if (type == TGas)
			{
				if (isLighted)
				{
					generate<EmitGas>(*tl)->initialize(nullptr, Owner(), 2, P(), $V(0, 0), 0.5);
					generate<EmitGas>(*tl)->initialize(nullptr, Owner(), 2, P(), $V(0, 0), 0.5);
					generate<EmitGas>(*tl)->initialize(nullptr, Owner(), 2, P(), $V(0, 0), 0.5);
					generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32,
						P(), 6, Vector2D(0, 0), 0);
				}
			}
		}
	}
}
void Bomb::run(jnr::JumpnRunData& data)
{
	if (lifetime < age)
	{
		finalize();
		return;
	}

	if (location == 'w')
	{
		isLighted = false;
	}

	if (homing)
	{
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
			auto p = P() + V() * 15;
			std::sort(v.begin(), v.end(), [p](const Object::Pipe & p1, const Object::Pipe & p2)
			{return (p - static_cast<Mover*>(p1.Dst(*tl))->Center()).sqsum()
			< (p - static_cast<Mover*>(p2.Dst(*tl))->Center()).sqsum(); });
			auto dp = static_cast<Mover*>(v.begin()->Dst(*tl))->Center() - Center();
			if (dp.sqsum() < 192 * 192)
			{
				double t1 = V().theta();
				double t2 = dp.theta();
				double dt = t2 - t1;
				if (dt >= 2 * M_PI)
					dt -= 2 * M_PI;
				if (dt < 0)
					dt += 2 * M_PI;
				double theta_speed = M_PI / 60;
				if (dt <= M_PI)
					phys.v = Vector2D::polar(phys.v.l2(), phys.v.theta() + theta_speed);
				else phys.v = Vector2D::polar(phys.v.l2(), phys.v.theta() - theta_speed);
			}
		}
	}

	Mover::run(data);

	if (collided_with_earth)
	{
		if (type == TExplosion)
		{
			if (isLighted)
			{
				generate<Explosion>(*tl)->initialize(REFPOINT_EXPLOSION_X, REFPOINT_EXPLOSION_Y, P(),
					-1, 12, $V(0, 0), 0, 7);
				generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32,
					P(), 6, Vector2D(0, 0), 0);
			}
		}
		else if (type == TGas)
		{
			if (isLighted)
			{
				generate<EmitGas>(*tl)->initialize(nullptr, Owner(), 2, P(), $V(0, 0), 0.5);
				generate<EmitGas>(*tl)->initialize(nullptr, Owner(), 2, P(), $V(0, 0), 0.5);
				generate<EmitGas>(*tl)->initialize(nullptr, Owner(), 2, P(), $V(0, 0), 0.5);
				generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32,
					P(), 6, Vector2D(0, 0), 0);
			}
		}
	}
}
void Bomb::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int offset_y = yanim ? (age / 8 % 2) * height : 0;
	if (!rotate_to_direction)
		graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx, refy + offset_y, width, height, turned);
	else
	{
		auto v = this->V();
		bool t = (v.x < 0);
		//v = $V(fabs(v.x), v.y);
		double theta = v.theta();
		if (t)
		{
			theta = 2 * M_PI - theta;
			theta += M_PI;
			if (theta > 2 * M_PI)
				theta -= 2 * M_PI;
		}
		graphics->drawSpinResize($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
			, refx, refy + offset_y, width, height, t, 0xffffffff, t ? -1.0 : 1.0, 1.0, theta);

	}
}