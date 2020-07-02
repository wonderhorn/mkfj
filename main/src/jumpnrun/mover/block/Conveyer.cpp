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

RegisterMoverClass(Conveyer);

static void cpyString(char buf[8], const std::string& str)
{
	assert(str.size() < 8);
	for (int i = 0; i < str.size(); i++)
	{
		buf[i] = str[i];
	}
	buf[7] = '\0';
}

void Conveyer::initialize(int refx, int refy, gmtr::Vector2D p, char rotation)
{
	Character::initialize(refx, refy, p, owner);
	width = 32;
	height = 16;
	margin_top = margin_bottom = margin_side = 0;
	interacting_with_blocks = false;
	smashable = false;
	enchantable = false;
	this->period = period;
	this->phys.mass = 0;
	cpyString(this->flag_id, "");
	prev_flag = false;
	this->rotation = rotation;// 0: stop, 1: right, 2: left
	writeName("conveyer", name);
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
gmtr::Vector2D Conveyer::react(const gmtr::Vector2D& v_prev, const gmtr::Vector2D& v_next, bool up)
{
	return $V(0, 0);
}

void Conveyer::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{

	$V center = phys.p + $V(width / 2, height / 2);
	if (this->Shape().includes(chara.P() + $V(0, chara.Height()))
		|| this->Shape().includes(chara.P() + $V(chara.Width(), chara.Height())))
		/*if (chara.Shape().includes(phys.p + $V(2, 2))
		|| chara.Shape().includes(phys.p + $V(width - 2, 2))
		|| chara.Shape().includes(phys.p + $V(width - 2, height - 2))
		|| chara.Shape().includes(phys.p + $V(2, height - 2)))*/
	{
		if (chara.V().y > 0)
		{
			double vx = 0;
			if (rotation == 1)
				vx = 2;
			else if (rotation == 2)
				vx = -2;
			chara.pushTo($V(chara.P().x, phys.p.y - chara.Height()));
			chara.pushTo($V(chara.P().x + vx, chara.P().y));
			chara.stamping();
		}
	}
}

void Conveyer::run(jnr::JumpnRunData& data)
{
	Rect();
	//mc.push_left = true;
	mc.poll();
	//Character::run();
	//if(flag_id < 0 || $P.flags[flag_id])
	phys.p += phys.v;

	if (flag_id[0] != 0)
	{
		rotation = $P.flags[flag_id];
		prev_flag = $P.flags[flag_id];
	}
	else
	{
		if ((age / 2) % period == 0)
		{
			//phys.v = ori_v;
		}
	}

	age++;
}
void Conveyer::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	int refy_offset = 0;
	if(rotation == 1)
		refy_offset = (age / 5) % 3;
	if (rotation == 2)
		refy_offset = ((INT_MAX - age) / 5) % 3;
	BaseGraphics* graphics = media.getGraphics();
	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx, refy + refy_offset * height, width, height, turned, 0xffffffff);
}
int Conveyer::damage(int power, int stun_time, int invinc_time)
{
	if (timer_invinc.moving())
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
	return power;
}
int Conveyer::damage(const Damage& d)
{
	if (timer_invinc.moving())
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
	return d.power;
}

const gfw::VirtualController& Conveyer::Controller()
{
	return mc;
}