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

RegisterMoverClass(Door);

static void cpyString(char buf[8], const std::string& str)
{
	assert(str.size() < 8);
	for (int i = 0; i < str.size(); i++)
	{
		buf[i] = str[i];
	}
	buf[7] = '\0';
}

void Door::initialize(int refx, int refy, gmtr::Vector2D p, OWNERHANDLE owner)
{
	Character::initialize(refx, refy, p, owner);
	phys.mass = 0;
	width = 32;
	height = 64;
	margin_top = margin_bottom = margin_side = 0;
	interacting_with_blocks = true;
	smashable = false;
	isClosed = true;
	breathing = false;
	cpyString(flag_id, "");
	writeName("door", name);
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
gmtr::Vector2D Door::react(const gmtr::Vector2D& v_prev, const gmtr::Vector2D& v_next, bool up)
{
	if (!isClosed)
		return $V(0, 0);
	using namespace gmtr;
	const Geometry& rect = this->Shape();

	/*if (rect.includes(v_prev))
		return $V(0, 0);*/

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

void Door::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{

}

void Door::run(jnr::JumpnRunData& data)
{
	//mc.push_left = true;
	if (flag_id[0] > 0)
		this->isClosed = !$P.flags[flag_id];
	mc.poll();
	Character::run(data);
}
void Door::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	/*if (this->isClosed)
		graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx, refy, width, height, turned, 0xffffffff);*/
	int offset_refx = width * !isClosed;
	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx + offset_refx, refy, width, height, turned, 0xffffffff);
}
int Door::damage(int power, int stun_time, int invinc_time)
{
	if (isClosed)
		return eDamage::Repelled;
	else
		return eDamage::Through;
}
int Door::damage(const Damage& d)
{
	if(isClosed)
		return eDamage::Repelled;
	else
		return eDamage::Through;
}

const gfw::VirtualController& Door::Controller()
{
	return mc;
}

void Door::receive(bool signal)
{
	this->isClosed = signal;
}
