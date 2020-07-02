#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/spell/Spell.h"
#include"jumpnrun/GRAPHICS.h"
using namespace jnr;
using namespace gfw;
using namespace gmtr;
#define $V Vector2D

RegisterMoverClass(HangedKyo);

void HangedKyo::initialize(int refx, int refy, gmtr::Vector2D p)
{
	Character::initialize(refx, refy, p, -1);
	phys.mass = 0;
	turned = true;
	margin_top = 24;
	hp = hp_max = 10000;
	interacting_with_blocks = false;
	writeName("effect_hangedkyo", name);
}

void HangedKyo::run(jnr::JumpnRunData& data)
{
	if (timer_damaged > 0)
		timer_damaged--;

	if ($P.getFlag("dspkyo"))
		alive = false;

	v_smash = $V(0, 0);
	Character::run(data);
	if (animation.finished)
	{
		if (!isOnSomething)
		{
			animation.reset(refx + 3 * width, refy, 2, 1, width, height);
			status = eStatus::Flying;
		}
		else
		{
			animation.reset(refx, refy, 2, 1, width, height);
			status = eStatus::Standing;
		}
	}
}
void HangedKyo::drive(phsc::Physics& phys, jnr::JumpnRunData& data)
{}

int HangedKyo::damage(int power, int stun_time, int invinc_time)
{
	if (timer_invinc.moving())
		return eDamage::Through;

	if (th_damage > power)
		return eDamage::Repelled;
	animation.reset(refx + width * 5, refy, stun_time, 1, width, height);
	status = eStatus::Damaged;
	timer_invinc.reset(invinc_time);
	timer_stun.reset(stun_time);
	v_target = $V(0, 0);
	hp -= power;
	if (stun_time > 0)
	{
		if (snd_damaged >= 0)
			$P.sound_stack.push_back($P.snd_mydamaged);
	}
	timer_damaged = 3;
	return power;
}

int HangedKyo::damage(const Damage& d)
{
	if (!d.ignoring_invinc && timer_invinc.moving())
		return eDamage::Through;
	if (d.isPhysical && timer_invinc_physical.moving())
		return eDamage::Through;

	if (th_damage > d.power)
		return eDamage::Repelled;

	animation.reset(refx + width * 5, refy, d.stun_time, 1, width, height);
	status = eStatus::Damaged;
	timer_invinc.reset(d.invinc_time);
	timer_stun.reset(d.stun_time);
	v_target = $V(0, 0);
	hp -= d.power;
	if (d.stun_time > 0)
	{
		if (snd_damaged >= 0)
			$P.sound_stack.push_back($P.snd_mydamaged);
	}
	timer_damaged = 3;

	return d.power;
}


void HangedKyo::die(jnr::JumpnRunData& data)
{
	alive = false;
}

void HangedKyo::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	if ($P.flags["dspkyo"] || !visible)
		return;

	BaseGraphics* graphics = media.getGraphics();
	bool t = turned;
	int alpha = 0xffffffff;
	int offset_refx = (status == Damaged || hp_max - hp > 50) * width;
	int offset_y = 0;
	if (status != Damaged)
	{
		int n = (age % 210);
		offset_y = -1 * (0 <= n && n <= 4);
	}
	else
	{
		offset_y = -5 * (timer_damaged != 0);
	}

	graphics->drawLine(phys.p.x + width / 2 + 6 - data.camera.X(), phys.p.y + offset_y - 32 - data.camera.Y()
		, phys.p.x + width / 2 + 6 - data.camera.X(), phys.p.y + height / 2 + offset_y - data.camera.Y(), 128, 64, 0, 255, 1);

	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y + offset_y - data.camera.Y(), 0
		, refx + offset_refx, refy, width, height, t, alpha);
	if (Constants::debugging)
	{
		graphics->drawString(gfw::num2str(this->hp), phys.p.x - data.camera.X(), phys.p.y - data.camera.Y() + height, 48, 20);
		graphics->drawString(gfw::num2str(this->score), phys.p.x - data.camera.X(), phys.p.y - data.camera.Y() + height + 20, 48, 20);
	}
}