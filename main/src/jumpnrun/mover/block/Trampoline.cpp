#include"jumpnrun/mover/block/block.h"
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

RegisterMoverClass(Trampoline);

void Trampoline::initialize(int refx, int refy, gmtr::Vector2D p, OWNERHANDLE owner)
{
	Character::initialize(refx, refy, p, owner);
	width = height = 32;
	phys.mass = 1;
	margin_top = 0;
	margin_bottom = margin_side = 2;
	interacting_with_blocks = true;
	smashable = false;
	enchantable = false;
	counter_spring = 0;
	writeName("block_spring", name);
}

static double sign(double d)
{
	if (d > 0)
		return 1.0;
	else if (d < 0)
		return -1.0;
	else return 0;
}
static double abs_add01(double d)
{
	if (d > 0)
		return (d + .0);
	else if (d < 0)
		return (d - .0);
	else return 0.0;
}
static double abs_add1(double d)
{
	if (d > 0)
		return (d + .2);
	else if (d < 0)
		return (d - .2);
	else return 0.0;
}
gmtr::Vector2D Trampoline::react(const gmtr::Vector2D& v_prev, const gmtr::Vector2D& v_next, bool up)
{
	using namespace gmtr;
	const Geometry& rect = this->Shape();
	if (rect.includes(v_next))
	{
		gmtr::Vector2D rtn(0.0, 0.0);
		gmtr::Vector2D v = v_next - v_prev;


		gmtr::Vector2D vv(0.0, v.y);
		gmtr::Vector2D vh(v.x, 0.0);
		if (rect.includes(v_prev + vh - $V(0, 2)) && rect.includes(v_prev + vh))
			rtn.x = -abs_add1(v.x);
		if (rect.includes(v_prev + vv - $V(1.0 * sign(vv.x), 0.0))
			&& rect.includes(v_prev + vv - $V(1.0 * sign(vv.x) - 1, 0.0))
			&& rect.includes(v_prev + vv - $V(1.0 * sign(vv.x) + 1, 0.0)))
			rtn.y = -abs_add01(v.y);
		return rtn;
	}
	return $V(0, 0);
}

void Trampoline::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	if (chara.Name().substr(0, 5) == "block")
		return;
	if (phys.v.x == 0.0 && phys.v.y == 0.0)
		return;
	if (chara.Owner() == this->Owner())
		return;
	$V center = phys.p + $V(width / 2, height / 2);

	// reset JumpHighestY to cancel falling 
	if (chara.Shape().includes(phys.p + $V(6, -16))
		|| chara.Shape().includes(phys.p + $V(width / 2, -16))
		|| chara.Shape().includes(phys.p + $V(width - 6, -16)))
	{
		chara.JumpHighestY(this->P().y - chara.Height());
	}
	if (chara.Shape().includes(phys.p + $V(6, -2))
		|| chara.Shape().includes(phys.p + $V(width - 6, -2)))
	{
		/*
		if (counter_spring == 0)
			counter_spring = 38;
		if (chara.V().y < -0.5)
			chara.Smash($V(0, -12));
		else if (counter_spring == 30)
			chara.Smash($V(0, -12));
		*/
		chara.JumpHighestY(this->P().y - chara.Height());
		if (counter_spring == 0)
			counter_spring = 42;
		if (chara.V().y < -0.5)
		{
			//chara.V(chara.V() + $V(0, -1.5));
			if (counter_spring >= 30)
			{
				chara.V($V(chara.V().x, -8.5));
				$P.sound_stack.push_back($P.snd_spring);
			}
		}
		else if (counter_spring == 30)
		{
			chara.V(chara.V() + $V(0, -4.25));
			$P.sound_stack.push_back($P.snd_jumping);
		}
	}

	/*
	if (chara.Shape().includes(phys.p + $V(2, 2))
		|| chara.Shape().includes(phys.p + $V(width - 2, 2))
		|| chara.Shape().includes(phys.p + $V(width - 2, height - 2))
		|| chara.Shape().includes(phys.p + $V(2, height - 2)))
	{
		if (chara.damage(15, Constants::time_stun, Constants::time_invinc) >= 0)
		{
			generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, phys.p, 2, $V(0.0, 0.0), 0);
			generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, phys.p, 4, $V(0.0, 0.0), 0);
			chara.Smash(this->phys.v.regularized() * 4.0 + $V(0, -4));
			finalize();
		}
	}*/
}

void Trampoline::run(jnr::JumpnRunData& data)
{
	//mc.push_left = true;
	mc.poll();
	if (counter_spring > 0)
		counter_spring--;
	Character::run(data);
}
void Trampoline::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int drefx = 32 * (counter_spring > 0 && counter_spring < 31);
	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx + drefx, refy, width, height, turned, 0xffffffff);
}
int Trampoline::damage(int power, int stun_time, int invinc_time)
{
	return Through;
	/*if (timer_invinc.moving())
		return -1;
	if (barrier.Dst(*tl))
	{
		Barrier* b = static_cast<Barrier*>(barrier.Dst(*tl));
		b->damage(power, stun_time, invinc_time);
	}
	animation.reset(refx + width * 5, refy, stun_time, 1, width, height);
	status = eStatus::Damaged;
	if (invinc_time > 0)
		timer_invinc.reset(12);
	timer_stun.reset(stun_time);
	v_target = $V(0, 0);
	hp -= power;
	return power;*/
}
int Trampoline::damage(const Damage& d)
{
	return Through;
	/*if (timer_invinc.moving())
		return -1;
	if (barrier.Dst(*tl))
	{
		Barrier* b = static_cast<Barrier*>(barrier.Dst(*tl));
		b->damage(d);
	}
	animation.reset(refx + width * 5, refy, d.stun_time, 1, width, height);
	status = eStatus::Damaged;
	if (d.invinc_time > 0)
		timer_invinc.reset(12);
	timer_stun.reset(d.stun_time);
	v_target = $V(0, 0);
	hp -= d.power;
	return d.power;*/
}

const gfw::VirtualController& Trampoline::Controller()
{
	return mc;
}