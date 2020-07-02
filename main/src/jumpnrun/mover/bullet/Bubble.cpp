#include"jumpnrun/mover/bullet/Bubble.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/mover/effect/Circle.h"
#include"jumpnrun/mover/Barrier.h"
#include"jumpnrun/mover/bullet/Bullet.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/spell/Spell.h"
#include"jumpnrun/GRAPHICS.h"
using namespace jnr;
using namespace gfw;
using namespace gmtr;
#define $V Vector2D

RegisterMoverClass(Bubble);

void Bubble::initialize(int refx, int refy
	, gmtr::Vector2D p, gmtr::Vector2D v, int power, OWNERHANDLE owner)
{
	Character::initialize(refx, refy, p, owner);
	this->power = power;
	this->phys.v = v;
	this->hp = this->hp_max = 1;
	width = height = 32;
	margin_top = margin_bottom = margin_side = 0;
	interacting_with_blocks = true;
	this->touch_and_dissapear = true;
	this->interacting_with_items = false;
	this->lifetime = 60 * 12;
	is_damaging = true;

	phys.mass = 0.5;
	wet = true;
	writeName("bubble", name);
}

void Bubble::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	if (chara.Name().substr(0, 6) == "bubble")
		return;
	if (chara.Owner() == this->Owner() || ! this->is_damaging)
		return;
	$V center = phys.p + $V(width / 2, height / 2);
	if (chara.Shape().includes(phys.p + $V(2, 2))
		|| chara.Shape().includes(phys.p + $V(width - 2, 2))
		|| chara.Shape().includes(phys.p + $V(width - 2, height - 2))
		|| chara.Shape().includes(phys.p + $V(2, height - 2)))
	{
		Damage dam;
		dam.power = power;
		dam.stun_time = Constants::time_stun / 2;
		dam.invinc_time = 0;
		dam.isPhysical = false;
		dam.smash = this->phys.v.regularized() * 2.0 + $V(0, -3);
		dam.owner = this->Owner();

		if (chara.damage(dam) >= 0)
		{
			generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, phys.p, 2, $V(0.0, 0.0), 0);
			generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, phys.p, 4, $V(0.0, 0.0), 0);
			//chara.Smash(dam.smash);
			finalize();
		}
	}
}

void Bubble::run(jnr::JumpnRunData& data)
{
	//mc.push_left = true;
	if (lifetime <= age)
		finalize();
	mc.poll();
	Character::run(data);
}
void Bubble::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx, refy, width, height, turned, 0xffffffff);
}
int Bubble::damage(int power, int stun_time, int invinc_time)
{
	Damage d;
	d.power = power;
	d.stun_time = stun_time;
	d.invinc_time = invinc_time;
	//d.owner = this->Owner();
	return damage(d);
}
int Bubble::damage(const Damage& d)
{
	if (timer_invinc.moving())
		return -1;

	animation.reset(refx + width * 5, refy, d.stun_time, 1, width, height);
	status = eStatus::Damaged;

	v_target = $V(0, 0);
	hp -= d.power;
	return d.power;
}

void Bubble::die(jnr::JumpnRunData& data)
{
	generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, phys.p, 2, $V(0.0, 0.0), 0);
	generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, phys.p, 4, $V(0.0, 0.0), 0);
	finalize();
}

const gfw::VirtualController& Bubble::Controller()
{
	return mc;
}