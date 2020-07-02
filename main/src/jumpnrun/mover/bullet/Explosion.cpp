#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/mover/bullet/Bullet.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/GRAPHICS.h"
using namespace jnr;
using namespace gfw;
using namespace gmtr;
#define $V gmtr::Vector2D

RegisterMoverClass(Explosion);

void Explosion::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v, double mass, int lifetime)
{
	Mover::initialize(refx, refy, p, owner);
	this->power = power;
	phys.v = v;
	phys.mass = mass;
	width = height = 32;
	this->lifetime = lifetime;// 12;
	touch_and_dissapear = true;
	writeName("bullet_explosion", name);
	turned = v.x < 0.0;
	margin_top = margin_side = margin_bottom = 8;
	rate = 0.0;
	alpha = 0xffffffff;
	timer_dissapear = 255;
	dr_dt = 0.5;
	$P.sound_stack.push_back($P.snd_exp);

	generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32, p// - $V(16, 16)
		, 14, $V(0, 0), 0, 6, 5.0);

}
void Explosion::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	if (chara.Owner() == this->Owner())
		return;
	$V center = phys.p + $V(width / 2, height / 2);
	auto dp = center + $V(0, 12)  - chara.Center();
	bool included = dp.l2() < 32 * rate * 0.55;
	if (included)
	{
		Damage d;
		d.power = power;
		d.isPhysical = false;
		d.stun_time = Constants::time_stun;
		d.invinc_time = Constants::time_invinc;
		d.ignoring_invinc = false;
		d.smash = -dp.regularized() * 8.0 + $V(0, -4);
		d.owner = this->Owner();
		if (chara.damage(d) >= 0)
		//if (chara.damage(power, Constants::time_stun, Constants::time_invinc) >= 0)
		{
			generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, chara.Center() - $V(16, 16), 2, $V(0.0, 0.0), 0);
			generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, chara.Center() - $V(16, 16), 4, $V(0.0, 0.0), 0);
			dp += $V(0, 16);
			chara.Smash(d.smash);
		}
	}
}
void Explosion::run(jnr::JumpnRunData& data)
{
	if (lifetime < age)
	{
		finalize();
		return;
	}

	rate += dr_dt;
	dr_dt -= 0.02;
	if (dr_dt < 0)
		dr_dt = 0;
	if (rate < 0)
		rate = 0;

	const int dissapear_speed = 8;
	if (timer_dissapear > dissapear_speed)
		timer_dissapear -= dissapear_speed;
	else
		timer_dissapear = 0;
	alpha = (timer_dissapear << 24) | (alpha & 0x00FFFFFF);

	Mover::run(data);
}
void Explosion::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	graphics->drawSpinResize($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx, refy, width, height, turned, alpha, rate, rate, 0.0);
}