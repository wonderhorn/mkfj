#include"jumpnrun/mover/character/BarrierMan.h"
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

RegisterMoverClass(BarrierMan);

void BarrierMan::initialize(int refx, int refy, gmtr::Vector2D p, gmtr::Vector2D v, OWNERHANDLE owner)
{
	Character::initialize(refx, refy, p, owner);
	this->phys.v = v;
	this->hp = this->hp_max = 99999;
	width = height = 64;
	margin_top = margin_bottom = margin_side = 0;
	interacting_with_blocks = true;
	this->touch_and_dissapear = false;
	phys.mass = 0.0;
	lifetime = 60 * 5;
	writeName("barrierman", name);
}

void BarrierMan::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	/*
	if (chara.Owner() == this->Owner())
		return;
	$V center = phys.p + $V(width / 2, height / 2);
	if (chara.Shape().includes(phys.p + $V(2, 2))
		|| chara.Shape().includes(phys.p + $V(width - 2, 2))
		|| chara.Shape().includes(phys.p + $V(width - 2, height - 2))
		|| chara.Shape().includes(phys.p + $V(2, height - 2)))
	{
		if (chara.damage(0, Constants::time_stun, 0) >= 0)
		{
			generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, phys.p, 2, $V(0.0, 0.0), 0);
			generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, phys.p, 4, $V(0.0, 0.0), 0);
			auto smashv = this->phys.v.regularized();
			if (smashv.l2() == 0)
				smashv = $V(0, -1);
			chara.Smash(smashv * 4.0 + $V(0, -4) - chara.V());
			finalize();
		}
	}
	*/
}

void BarrierMan::run(jnr::JumpnRunData& data)
{
	//mc.push_left = true;
	if (age >= lifetime)
	{
		die(data);
		return;
	}
	mc.poll();
	Character::run(data);
}
void BarrierMan::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx, refy, width, height, turned, 0xffffffff);
}
int BarrierMan::damage(int power, int stun_time, int invinc_time)
{
	return -1;
}
int BarrierMan::damage(const Damage& d)
{
	return -1;

}

const gfw::VirtualController& BarrierMan::Controller()
{
	return mc;
}