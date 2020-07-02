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

RegisterMoverClass(DisappearingFloor);

static void cpyString(char buf[JNR_FLAG_SIZE], const std::string& str)
{
	assert(str.size() < JNR_FLAG_SIZE);
	for (int i = 0; i < str.size(); i++)
	{
		buf[i] = str[i];
	}
	buf[JNR_FLAG_SIZE - 1] = '\0';
}

void DisappearingFloor::initialize(int refx, int refy, gmtr::Vector2D p, gmtr::Vector2D v, int period, int delay)
{
	Character::initialize(refx, refy, p, owner);
	width = 64;
	height = 18;
	margin_top = margin_bottom = margin_side = 0;
	interacting_with_blocks = false;
	smashable = false;
	enchantable = false;
	this->phys.v = v;
	this->period = period;
	this->phys.mass = 0;
	cpyString(this->flag_id, "");
	this->delay = delay;
	prev_flag = false;
	ori_v = v;
	is_visible = true;
	writeName("movingfloor", name);
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
gmtr::Vector2D DisappearingFloor::react(const gmtr::Vector2D& v_prev, const gmtr::Vector2D& v_next, bool up)
{
	/*
	using namespace gmtr;
	Rect();
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
	}*/
	return $V(0, 0);
}

void DisappearingFloor::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	if (!is_visible)
		return;
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
			chara.pushTo($V(chara.P().x, phys.p.y - chara.Height()));
			chara.stamping();
		}
	}
}

void DisappearingFloor::run(jnr::JumpnRunData& data)
{
	Rect();
	//mc.push_left = true;
	mc.poll();
	//Character::run();
	//if(flag_id < 0 || $P.flags[flag_id])
	phys.p += phys.v;

	if (flag_id[0] != '\0')
	{
		if ($P.flags[flag_id])
		{
			if (period > 0)
			{
				if ((age + delay) % period == period / 2)
				{
					phys.v = ori_v;
					is_visible = true;
				}
			}
			else
			{
				is_visible = $P.flags[flag_id];
			}
		}
		prev_flag = $P.flags[flag_id];
	}
	else
	{
		//if ($P.flags[flag_id])
		if (period > 0)
		{
			if ((age + delay) % period == period / 2)
			{
				phys.v = ori_v;
				is_visible = true;
			}
		}
		else
			is_visible = true;
	}

	if (period == 0)
	{
		if (flag_id[0] == '\0')
			is_visible = true;
		else
			is_visible = $P.getFlag(flag_id);
	}
	else if ((age + delay) % period == period - 1)
	{
		phys.v = -phys.v;
		is_visible = false;
		if (flag_id[0] == '\0' || $P.flags[flag_id])
		{
			generate<Effect>(*tl)->initialize(REFPOINT_STEP_DISSAPPEARED_X, REFPOINT_STEP_DISSAPPEARED_Y
				, width, height, P(), 6, $V(0, 0), 0);
		}
	}
	age++;
}
void DisappearingFloor::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	if (is_visible)
	{
		graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
			, refx, refy, width, height, turned, 0xffffffff);
	}
}
int DisappearingFloor::damage(int power, int stun_time, int invinc_time)
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
int DisappearingFloor::damage(const Damage& d)
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

const gfw::VirtualController& DisappearingFloor::Controller()
{
	return mc;
}