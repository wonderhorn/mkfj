#define _USE_MATH_DEFINES
#include<algorithm>
#include<cmath>
#include<vector>
#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/mover/bullet/Bullet.h"
#include"jumpnrun/MoverUtilities.h"
#include"utils/MyCppUtil.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/GRAPHICS.h"
using namespace jnr;
using namespace gfw;
using namespace gmtr;
using std::min;
using std::max;
#define $V gmtr::Vector2D

RegisterMoverClass(BigEyeTentacle);


void BigEyeTentacle::initialize(jnr::Character& chara, int power, gmtr::Vector2D v, 
	gmtr::Vector2D connector, double mass)
{
	Whip::initialize(chara, power, v, connector, mass);
	max_length_seg = 12;
	max_num_seg = 12;
	lifetime = 60 * 4;
	writeName("bullet_bigeyetentacle", name);

}

void BigEyeTentacle::run(jnr::JumpnRunData& data)
{
	const int disappear_speed = 2;
	if (timer_dissapear < 0 && lifetime - 120 < age)
	{
		timer_dissapear = 120;
	}
	if (lifetime < age)
	{
		finalize();
		return;
	}
	if (timer_dissapear > 0)
	{
		timer_dissapear--;
		//alive_length = (double)sprine.length * timer_dissapear / (disappear_speed * sprine.length);
		if (timer_dissapear % disappear_speed == 0)
		{
			reelup();
			//tenseForward();
		}
	}
	if (timer_dissapear == 0)
	{
		finalize();
		return;
	}

	auto newphys = phys;
	newphys.a = Vector2D(0.0, 0.2 * phys.mass);
	newphys.v += newphys.a;
	$V p_ter = sprine.points[sprine.length - 1];
	p_ter += V();
	newphys.p = p_ter;
	//phys = newphys;

	if (!stg->enterable(P()))
	//if (!stg->enterable(P()) || sprine.length >= 12)
	{
		//if (timer_dissapear < 0)
		//	timer_dissapear = disappear_speed * sprine.length;
		//alive = false;

		for (int i = sprine.length - 2; i >= 0; i--)
		{

			//phys = newphys;
			$V newp = sprine.points[i] + V();
			if (!stg->enterable(newp))
			{
				//if (timer_dissapear < 0)
				//	timer_dissapear = disappear_speed * sprine.length;
				//alive = false;
			}
			else
			{
				sprine.points[i] = newp;
				break;
			}
		}
	}
	else
	{
		phys = newphys;
		sprine.points[sprine.length - 1] = p_ter;
	}

	double l = (p_ter - sprine.points[sprine.length - 2]).l2();
	if (l >= max_length_seg)
	{
		if (sprine.length < sprine.max_length && sprine.length < max_num_seg)
		{
			if (timer_dissapear < 0)
			{
				sprine.push_back(p_ter);
				alive_length = sprine.length;
			}
		}
		else
		{
			//phys.v = $V(0, 0);
		}
	}
	tenseBackward();
	if (Character* c = static_cast<Character*>(pipe.Dst(*tl)))
	{
		sprine.points[0] = connector + c->P();
		//sprine.points[0] = c->Center();
	}
	tenseForward();

	if (1)
	{
		EnemySearcher es;
		es.initialize(this);
		auto v = es.search(64 * 5);
		const auto o = Owner();
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
			auto dp = static_cast<Mover*>(v.begin()->Dst(*tl))->Center() - p;
			if (dp.sqsum() < 192 * 192)
			{
				double t1 = V().theta();
				double t2 = dp.theta();
				double dt = t2 - t1;
				if (dt >= 2 * M_PI)
					dt -= 2 * M_PI;
				if (dt < 0)
					dt += 2 * M_PI;
				double theta_speed = M_PI / 48;
				if (dt <= M_PI)
					phys.v = Vector2D::polar(phys.v.l2(), phys.v.theta() + theta_speed);
				else phys.v = Vector2D::polar(phys.v.l2(), phys.v.theta() - theta_speed);
			}
		}
	}

	age++;

	//Mover::run(data);
}

void BigEyeTentacle::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	for (int i = 0; i < min(sprine.length, alive_length) - 1; i++)
	{
		//graphics->drawLine(sprine.points[i].x - data.camera.X(), sprine.points[i].y - data.camera.Y()
		//	, sprine.points[i + 1].x - data.camera.X(), sprine.points[i + 1].y - data.camera.Y()
		//	, r, g, b, 255, 1);

		double theta = $V(sprine.points[i + 1].x - sprine.points[i].x, sprine.points[i + 1].y - sprine.points[i].y).theta();
		graphics->drawSpinResize($P.img_bigeye
			, sprine.points[i].x - 16 - data.camera.X(), sprine.points[i].y - 16 - data.camera.Y(), 0,
			192, 32, 32, 32, false, 0xffffffff, 1, 1, theta);
	}
}