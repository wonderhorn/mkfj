#define _USE_MATH_DEFINES
#include<cmath>
#include"jumpnrun/mover/bullet/Bullet.h"
#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/MoverUtilities.h"
#include"utils/MyCppUtil.h"
#include"jumpnrun/GRAPHICS.h"
using namespace jnr;
using namespace gfw;
using namespace gmtr;
#define $V gmtr::Vector2D

RegisterMoverClass(Fire);

void Fire::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v, double mass, bool homing)
{
	Bullet::initialize(refx, refy, p, owner, power, v, mass);
	this->homing = homing;

	lifetime = 120;

	if (isnan(p.x))
	{
		int a = 0;
	}
}
//virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
void Fire::run(jnr::JumpnRunData& data)
{
	Bullet::run(data);

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
			auto p = Center() + V() * 15;
			std::sort(v.begin(), v.end(), [p](const Object::Pipe & p1, const Object::Pipe & p2)
			{return (p - static_cast<Mover*>(p1.Dst(*tl))->Center()).sqsum()
			< (p - static_cast<Mover*>(p2.Dst(*tl))->Center()).sqsum(); });
			auto dp = static_cast<Mover*>(v.begin()->Dst(*tl))->Center() - p;
			if (dp.sqsum() < 96 * 96)
			{
				double t1 = V().theta();
				double t2 = dp.theta();
				double dt = t2 - t1;
				if (dt >= 2 * M_PI)
					dt -= 2 * M_PI;
				if (dt < 0)
					dt += 2 * M_PI;
				double theta_speed = M_PI / 64;
				if (dt <= M_PI)
					phys.v = Vector2D::polar(phys.v.l2(), phys.v.theta() + theta_speed);
				else phys.v = Vector2D::polar(phys.v.l2(), phys.v.theta() - theta_speed);
			}
		}
	}

}
void Fire::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int phase = age / 20 % 4;
	int alpha = (age / 3 % 2) >= 1 ? 0xffffffff : 0xddffffff;
	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx, refy + height * (phase % 2), width, height, phase >= 2, alpha);
}


void Flame::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v, double mass, int rank)
{
	Bullet::initialize(refx, refy, p, owner, power, v, mass);
	this->rank = rank;
	lifetime = 12;

	if (isnan(p.x))
	{
		int a = 0;
	}
}
//virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
void Flame::run(jnr::JumpnRunData& data)
{
	if (rank != 0 && age == 3)
	{
		int d = 3;
		//$V v_rnd($P.rand.get() % 2, $P.rand.get() % 8 - 4);
		$V v_rnd2($P.rand.get() % 3, $P.rand.get() % 4 - 2);
		$V v_rnd = $V::polar(v_rnd2.l2(), v_rnd2.theta() + phys.v.theta());
		if (isnan(v_rnd.x))
		{
			int a = 0;
		}
		double vy = $P.rand.get() % 8 - 4;
		double vx = $P.rand.get() % 2;
		//generate<Flame>(*tl)->initialize(refx, refy, phys.p + this->V() * d, owner, power, $V(phys.v.x, vy), phys.mass, rank - 1);
		//generate<Flame>(*tl)->initialize(refx, refy, phys.p + this->V() * d, owner, power, $V(phys.v.x + vx, vy), phys.mass, rank - 1);
		generate<Flame>(*tl)->initialize(refx, refy, phys.p + this->V() * d, owner, power, phys.v + v_rnd, phys.mass, rank - 1);
		generate<Flame>(*tl)->initialize(refx, refy, phys.p + this->V() * d, owner, power, phys.v + v_rnd, phys.mass, rank - 1);
		/*generate<Flame>(*tl)->initialize(refx, refy, phys.p + this->V() * (lifetime - 6)
			, owner, power, $V(phys.v.x, v_rnd.y), phys.mass, rank - 1);
		generate<Flame>(*tl)->initialize(refx, refy, phys.p + this->V() * (lifetime - 6) + v_rnd
			, owner, power, $V(phys.v.x + v_rnd.x, v_rnd.y), phys.mass, rank - 1);*/
	}

	if (lifetime < age)
	{
		finalize();
		//Effect* e = gfw::generate<Effect>(*tl);
		//e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, P(), 2, $V(0, 0), 0);
		return;
	}
	age++;
}
void Flame::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int phase = age / 4 % 4;
	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx, refy + height * (phase % 2), width, height, phase >= 2, 0xaaffffff);
}