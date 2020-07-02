#pragma once
#include"jumpnrun/GRAPHICS.h"
#include"jumpnrun/mover/enemy/PunchingMachine.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/mover/Barrier.h"
#include"jumpnrun/mover/Item.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/stage/ItemDropper.h"
using namespace jnr;
using namespace enm;
using namespace gfw;
using namespace gmtr;
#define $V Vector2D

RegisterMoverClass(PunchingMachine);

void PunchingMachine::initialize(gmtr::Vector2D p, OWNERHANDLE owner, bool turned)
{
	Character::initialize(refx, refy, p, owner);
	refx = REFPOINT_PUNCHING_MACHINE_X;
	refy = REFPOINT_PUNCHING_MACHINE_Y;
	this->interacting_with_items = false;
	is_damaging = false;
	smashable = false;
	timer_deactivate_damage = 0;
	this->lifetime = 999999;
	hp = hp_max = 999999;
	meter = 0;
	meter_max = 150;
}

void PunchingMachine::run(jnr::JumpnRunData& data)
{
	if (age % 32 == 0 && meter > 0)
		meter -= 1;
	think(data);
	mc.poll();
	Character::run(data);
	if (lifetime < age)
		alive = false; //not die
	if (timer_deactivate_damage > 0)
		timer_deactivate_damage--;
	if (timer_deactivate_damage == 1)
		is_damaging = true;

	if (meter > meter_max)
	{
		generate<GetCard>(*tl)->initialize(
			REFPOINT_CARD_X, REFPOINT_CARD_Y,
			this->P() + $V(0, -96),
			0, 1.0,
			52);
		gfw::generate<Particles>(*tl)->initialize(
			REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32,
			this->P() + $V(0, -96) + $V(0, 0), 8, $V(0, 0), 0);
		alive = false;
	}
}

void PunchingMachine::think(jnr::JumpnRunData& data)
{}


void PunchingMachine::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int alpha = timer_invinc.moving() ? 0xaaffffff : 0xffffffff;
	int offsetx = 0;
	if (!animation.finished)
		offsetx = (age / 4) % 2;
	graphics->draw($P.img_obj, phys.p.x - data.camera.X() + offsetx, phys.p.y - data.camera.Y(), 0
		, refx, refy, width, height, turned, alpha);

	const int interval = 4;
	for (int i = 0; i < meter / interval; i++)
	{
		graphics->draw($P.img_obj
			, phys.p.x - data.camera.X() + 48
			, phys.p.y - data.camera.Y() + height - (i - 1) * interval - 8, 0
			, refx + 32, refy, width, interval, turned, alpha);
	}
	for (int i = 0; i < meter_max / interval; i++) 
	{
		graphics->draw($P.img_obj
			, phys.p.x - data.camera.X() + 48
			, phys.p.y - data.camera.Y() + height - (i - 1) * interval - 8, 0
			, refx + 32, refy + 32, width, interval, turned, alpha);
	}
}


int PunchingMachine::damage(const Damage& d)
{
	if (!d.ignoring_invinc && timer_invinc.moving())
		return eDamage::Through;
	if (d.isPhysical && timer_invinc_physical.moving())
		return eDamage::Through;
	if (auto* ptr = barrier.Dst(*tl))
	{
		Barrier* b = static_cast<Barrier*>(ptr);
		//return b->damage(d);
		int barrier_result = b->damage(d);
		if (barrier_result != eDamage::Through)
		{
			return barrier_result;
		}
	}
	if (th_damage > d.power)
		return eDamage::Repelled;

	hp -= d.power;
	meter += d.power;
	if (d.stun_time > 0)
	{
		if (snd_damaged >= 0)
			$P.sound_stack.push_back($P.snd_mydamaged);
		int offset_refx = (d.smash.x >= 0) ^ turned ? 23 : 24;
		animation.reset(refx + width * offset_refx, refy, d.stun_time, 1, width, height);
		status = eStatus::Damaged;
		timer_invinc.reset(d.invinc_time);
		timer_stun.reset(d.stun_time / 4);
		v_target = $V(0, 0);
	}
	this->Smash(d.smash);
	return d.power;
}