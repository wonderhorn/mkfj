#define _USE_MATH_DEFINES
#include<cmath>
#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/mover/bullet/Bullet.h"
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

RegisterMoverClass(Sword);

void Sword::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v, double mass
	, bool rotate_to_direction)
{
	Bullet::initialize(refx, refy, p, owner, power, v, mass, rotate_to_direction);

	writeName("bullet_sword", name);
	margin_top = margin_side = margin_bottom = 8;
	this->rotate_to_direction = rotate_to_direction;
}
void Sword::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	if (chara.Owner() == this->Owner())
		return;
	$V center = phys.p + $V(width / 2, height / 2);
	if (chara.Shape().includes(center))
	{
		auto smashv = this->phys.v.regularized();
		if (smashv.l2() <= 0.1)
			smashv = $V(0, -1);
		Damage dam;
		dam.power = power;
		dam.stun_time = Constants::time_stun;
		dam.invinc_time = Constants::time_invinc;
		dam.isPhysical = false;
		if (smashv.l2() == 0)
			smashv = $V(0, -1);
		dam.smash = smashv * 4.0 + $V(0, -4) - chara.V();
		dam.owner = this->Owner();
		int d = chara.damage(dam);
		if (d >= 0)
		{
			generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, phys.p, 2, $V(0.0, 0.0), 0);
			generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, phys.p, 4, $V(0.0, 0.0), 0);

			//chara.Smash(smashv * 4.0 + $V(0, -4) - chara.V());
			finalize();
		}
	}
}
void Sword::run(jnr::JumpnRunData& data)
{
	if (lifetime < age)
	{
		finalize();
		return;
	}

	if (age < 60)
	{
		phys.v = $V(0, 0);
	}
	else if (age == 60)
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
			auto p = this->Center();
			std::sort(v.begin(), v.end(), [p](const Object::Pipe & p1, const Object::Pipe & p2)
			{return (p - static_cast<Mover*>(p1.Dst(*tl))->Center()).sqsum()
			< (p - static_cast<Mover*>(p2.Dst(*tl))->Center()).sqsum(); });
			auto dp = static_cast<Mover*>(v.begin()->Dst(*tl))->Center() - p;
			double sqsum_dp = dp.sqsum();
			if (sqsum_dp < 192 * 192 && sqsum_dp > 0)
			{
				phys.v = dp / sqrt(sqsum_dp) * 8.0;
			}
			else
			{
				int dir = 1 - 2 * turned;
				phys.v = $V(8 * dir, 0);
			}
		}
		else
		{
			int dir = 1 - 2 * turned;
			phys.v = $V(8 * dir, 0);
		}
	}

	Mover::run(data);

}
void Sword::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int offset_y = 0;
	if (!rotate_to_direction)
		graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx, refy + offset_y, width, height, turned);
	else
	{
		auto v = this->V();
		bool t = (v.x < 0);
		//v = $V(fabs(v.x), v.y);
		double theta = v.sqsum() == 0.0? 0.0 : v.theta();
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