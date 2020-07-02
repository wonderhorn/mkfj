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

RegisterMoverClass(FlyingCarpet);

static void cpyString(char buf[8], const std::string& str)
{
	assert(str.size() < 8);
	for (int i = 0; i < str.size(); i++)
	{
		buf[i] = str[i];
	}
	buf[7] = '\0';
}

void FlyingCarpet::initialize(int refx, int refy, gmtr::Vector2D p, gmtr::Vector2D v
	, int period, eVehicleType vehicle_type)
{
	Character::initialize(refx, refy, p, owner);
	width = 64;
	height = 18;
	margin_top = margin_bottom = margin_side = 0;
	this->hp = 10;
	interacting_with_blocks = false;
	smashable = false;
	enchantable = false;
	this->phys.v = v;
	this->period = period;

	this->vehicle_type = vehicle_type;
	if(vehicle_type == eVehicleType::Flying)
		this->phys.mass = 0;
	else
		this->phys.mass = 1;
	cpyString(this->flag_id, "");
	prev_flag = false;
	ori_v = v;
	someone_on_me = false;
	moved = true;
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
gmtr::Vector2D FlyingCarpet::react(const gmtr::Vector2D& v_prev, const gmtr::Vector2D& v_next, bool up)
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

void FlyingCarpet::interact(jnr::Character& chara, jnr::JumpnRunData& data)
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
			int margin = 8;

			if (chara.Center().x < Center().x - margin)
			{
				phys.v.x = -1.0;
			}
			else if (chara.Center().x > Center().x + margin)
			{
				phys.v.x = 1.0;
			}
			else
				phys.v.x = 0;

			if (vehicle_type == eVehicleType::Flying)
			{
				if (chara.Status() == Sitting)
				{
					phys.v.y = 1.0;
				}
				else
				{
					phys.v.y = -1.0;
				}
			}

			chara.pushTo($V(chara.P().x, phys.p.y - chara.Height()));
			if (moved)
			{
				chara.pushTo($V(chara.P().x + V().x, chara.P().y));
			}
			chara.stamping();
			someone_on_me = true;
		}
	}
}

void FlyingCarpet::run(jnr::JumpnRunData& data)
{
	//update location
	location = stg->getType(Center());

	Rect();
	//mc.push_left = true;
	mc.poll();
	//Character::run();
	//if(flag_id < 0 || $P.flags[flag_id])
	auto newp = phys.p + phys.v;
	bool col = false;
	col = col || !stg->enterable(newp);
	col = col || !stg->enterable(newp + $V(width, 0));
	col = col || !stg->enterable(newp + $V(width, height));
	col = col || !stg->enterable(newp + $V(0, height));

	bool col2 = false;
	col2 = col2 || !stg->enterable(newp + $V(8, 8));
	col2 = col2 || !stg->enterable(newp + $V(width - 8, 8));
	col2 = col2 || !stg->enterable(newp + $V(width - 8, height - 8));
	col2 = col2 || !stg->enterable(newp + $V(8, height - 8));

	phys.v += phys.a;
	if (!vehicle_type == eVehicleType::Flying)
	{
		if (isOnSomething < 5)
		{
			if (location == 'w' || stg->getType(Center() + Vector2D(0, 24)) == 'w')
				phys.a = Vector2D(0.0, 0.08 * phys.mass);
			else
				phys.a = Vector2D(0.0, 0.2 * phys.mass);
		}
	}

	if (vehicle_type == Flying)
	{
		if (!someone_on_me)
		{
			phys.v.x = 0;
			phys.v.y = 0.5;
		}
		else
		{
			//someone_on_me = false;
			//phys.v.x = 0;
			//phys.v.y = 0;
		}
	}
	else if (vehicle_type == Boat)
	{
		if (location == 'w')
		{
			phys.v.y = 0;
		}
		if (phys.v.x < 0)
			turned = true;
		else if (phys.v.x > 0)
			turned = false;
 		if (!someone_on_me)
		{
			phys.v.x = 0;
		}
	}

	if ((someone_on_me && !col2) || !col)
	{
		moved = true;
		phys.p += phys.v;
	}
	else
	{
		moved = false;
	}

	//interact
	for (auto t = tl->begin(); t != tl->end(); t++)
	{
		Object* obj = getObject(t);
		if (!obj->isMover)
			continue;
		Mover* mvr = static_cast<Mover*>(obj);
		if (mvr->ID() != this->id)
		{
			mvr->interact(*this, data);
		}
	}
	if (HP() <= 0)
		die(data);

	someone_on_me = false;
	age++;
}
void FlyingCarpet::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	bool turned = this->turned;
	if (vehicle_type == eVehicleType::Flying)
		turned = age / 30 % 2;

	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx, refy, width, height, turned, 0xffffffff);
}
int FlyingCarpet::damage(int power, int stun_time, int invinc_time)
{
	if (1)
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
int FlyingCarpet::damage(const Damage& d)
{
	if (1)
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

const gfw::VirtualController& FlyingCarpet::Controller()
{
	return mc;
}