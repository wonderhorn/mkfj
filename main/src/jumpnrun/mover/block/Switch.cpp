#include"jumpnrun/mover/block/Switch.h"
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

RegisterMoverClass(Switch);

static void cpyString(char buf[8], const std::string& str)
{
	assert(str.size() < 8);
	for (int i = 0; i < str.size(); i++)
	{
		buf[i] = str[i];
	}
	buf[7] = '\0';
}

void Switch::setOn(bool b) 
{
	isOn_prev = isOn = b;
}


void Switch::initialize(int refx, int refy, gmtr::Vector2D p, OWNERHANDLE owner, double mass)
{
	Character::initialize(refx, refy, p, owner);
	width = height = 32;
	margin_top = margin_bottom = margin_side = 0;
	interacting_with_blocks = true;
	smashable = false;
	isOn_prev = isOn = false;
	cpyString( flag_id, "");
	time_up = -1;
	phys.mass = mass;
	writeName("switch", name);
}

void Switch::run(jnr::JumpnRunData& data)
{
	//mc.push_left = true;
	mc.poll();
	if (timer > 0)
		timer--;
	if (timer == 0)
	{
		timer = -1;
		isOn = false;
	}
	if (flag_id[0] > 0)
	{
		if (isOn != isOn_prev)
			$P.flags[flag_id] = isOn;
	}
	isOn_prev = isOn;

	Character::run(data);
}
void Switch::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int offset_x = isOn * width;
	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx + offset_x, refy, width, height, turned, 0xffffffff);
}
int Switch::damage(int power, int stun_time, int invinc_time)
{
	if (timer_invinc.moving())
		return -1;
	if (barrier.Dst(*tl))
	{
		Barrier* b = static_cast<Barrier*>(barrier.Dst(*tl));
		b->damage(power, stun_time, invinc_time);
	}
	if (invinc_time > 0)
		timer_invinc.reset(45);
	timer_stun.reset(stun_time);
	v_target = $V(0, 0);
	if (toggle)
		isOn = !isOn;
	else
	{
		if (!isOn)
		{
			isOn = true;
			timer = time_up;
		}
	}
	if (auto* ptr = pipe.Dst(*tl))
	{
		SwitchReciever* sr = (SwitchReciever*)ptr;
		sr->receive(isOn);
	}
	return power;
}
int Switch::damage(const Damage& d)
{
	if (timer_invinc.moving())
		return -1;
	if (barrier.Dst(*tl))
	{
		Barrier* b = static_cast<Barrier*>(barrier.Dst(*tl));
		b->damage(d);
	}
	if (d.invinc_time > 0)
		timer_invinc.reset(45);
	timer_stun.reset(d.stun_time);
	v_target = $V(0, 0);
	if (toggle)
		isOn = !isOn;
	else
	{
		if (!isOn)
		{
			isOn = true;
			timer = time_up;
		}
	}
	if (gfw::Object* ptr = pipe.Dst(*tl))
	{
		SwitchReciever* sr = static_cast<SwitchReciever*>(ptr);
		sr->receive(isOn);
	}
	return d.power;
}

const gfw::VirtualController& Switch::Controller()
{
	return mc;
}
