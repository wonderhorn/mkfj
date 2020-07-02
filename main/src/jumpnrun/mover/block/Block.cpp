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

RegisterMoverClass(Block);

void Block::initialize(int refx, int refy, gmtr::Vector2D p, OWNERHANDLE owner, double mass, 
	int HP, int top_margin)
{
	Character::initialize(refx, refy, p, owner);
	width = 32;
	height = 32 + top_margin;
	margin_top = top_margin;
	margin_bottom = margin_side = 0;
	interacting_with_blocks = true;
	smashable = false;
	breathing = false;
	enchantable = false;
	is_solid = true;
	hp = hp_max = HP;
	phys.mass = mass;
	writeName("block", name);
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
gmtr::Vector2D Block::react(const gmtr::Vector2D& v_prev, const gmtr::Vector2D& v_next, bool up)
{
	using namespace gmtr;
	const Geometry& rect = this->Shape();
	if (is_solid) {
		if (rect.includes(v_next))
		{

			gmtr::Vector2D rtn(0.0, 0.0);
			gmtr::Vector2D v = v_next - v_prev;


			gmtr::Vector2D vv(0.0, v.y);
			gmtr::Vector2D vh(v.x, 0.0);
			//if (!rect.includes(v_prev + vh) && !rect.includes(v_prev + vh))
			//	rtn.y = -1;

			if (rect.includes(v_prev + vh - $V(0, 2)) && rect.includes(v_prev + vh))
				rtn.x = -abs_add1(v.x);
			
			if (rect.includes(v_prev + vh - $V(0, 2)) && rect.includes(v_prev + vh))
				rtn.x = -abs_add1(v.x);
			if (!up 
				&& rect.includes(v_prev + vv - $V(1.0 * sign(vv.x), 0.0))
				&& rect.includes(v_prev + vv - $V(1.0 * sign(vv.x) - 1, 0.0))
				&& rect.includes(v_prev + vv - $V(1.0 * sign(vv.x) + 1, 0.0)))
				//rtn.y = -abs_add1(v.y);
				rtn.y = -abs_add01(v.y);
			if (rtn.l2() >= 4.0)
				rtn = rtn.regularized() * 4.0;
			return rtn;
		}
	}
	return $V(0, 0);
}

void Block::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	if (chara.Name().substr(0, 5) == "block")
		return;
	if (phys.v.x == 0.0 && phys.v.y == 0.0)
		return;
	if (chara.Owner() == this->Owner())
		return;
	if (!is_solid) 
		return;
	if (!chara.InteractingWithBlocks())
		return;

	$V center = phys.p + $V(width / 2, height / 2);

	int immerse_y = chara.Rect().y2 - this->P().y;
	auto& chara_shape = chara.Shape();
	//ÇﬂÇËçûÇ›ñhé~ÇÃñ⁄ìIÇ≈âüÇµñﬂÇ∑
	if (chara_shape.includes(phys.p + $V(margin_side + 2, margin_top - 1))
		|| chara_shape.includes(phys.p + $V(width - margin_side - 2, margin_top - 1))
		|| chara_shape.includes(phys.p + $V(width / 2, margin_top - 1))
		|| chara_shape.includes(phys.p + $V(width - margin_side - 2, height + 1))
		|| chara_shape.includes(phys.p + $V(margin_side + 2, height + 1)))
	{
		if (chara.Name() == "character_player") 
		{
			int a = 0;
		}
		if (immerse_y < 12 && chara.InteractingWithBlocks())
			chara.pushTo($V(chara.P().x, this->P().y + 1 - chara.Height()));
	}

	int margin = 4;
	if (chara.Shape().includes(phys.p + $V(margin_side + margin, margin + margin_top))
		|| chara.Shape().includes(phys.p + $V(width - margin_side - margin, margin + margin_top))
		|| chara.Shape().includes(phys.p + $V(width - margin_side - margin, height - margin))
		|| chara.Shape().includes(phys.p + $V(margin_side + margin, height - margin)))
	{
		if (chara.Rect().y2 < this->P().y + 10)
		{
			if(chara.InteractingWithBlocks())
				chara.pushTo($V(chara.P().x, this->P().y + 1 - chara.Height()));
		}
		else if (chara.Rect().y1 > this->P().y + height - 10)
		{
			if (chara.InteractingWithBlocks())
			{
				chara.pushTo($V(chara.P().x, this->P().y + height - chara.V().y));
				chara.V($V(chara.V().x, 0));
			}
		}
		else if (chara.damage(10, Constants::time_stun, Constants::time_invinc) >= 0)
		{
			generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, phys.p, 2, $V(0.0, 0.0), 0);
			generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, phys.p, 4, $V(0.0, 0.0), 0);
			chara.Smash(this->phys.v.regularized() * 4.0 + $V(0, -4));
			finalize();
		}
	}
}

void Block::run(jnr::JumpnRunData& data)
{
	//mc.push_left = true;
	mc.poll();

	auto c = Center();
	bool in_camera = c.x >= data.camera.X() - 128
		&& c.x <= data.camera.X() + data.camera.Width() + 128
		&& c.y >= data.camera.Y() - 64 && c.y <= data.camera.Y() + data.camera.Height() + 64;
	if (in_camera)
		Character::run(data);
	else Rect();
}
void Block::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx, refy, width, height, turned, 0xffffffff);
}
int Block::damage(int power, int stun_time, int invinc_time)
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
int Block::damage(const Damage& d)
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

void Block::die(jnr::JumpnRunData& data)
{
	alive = false;
	Vector2D center = phys.p + Vector2D(width / 2 - 16, height / 2 - 16);
	generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32
		, center, 4, Vector2D(0, 0), 0);
	int offset_refx = 32 * ($P.rand.get() % 4);
	//generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X + offset_refx, REFPOINT_STAR1_Y, 32, 32, Center() - Vector2D(16, 16)
	//	, 12, Vector2D(0, 0), 0, 6, 4.0);
	/*generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_X, REFPOINT_EXPLOSION_Y, 32, 32
		, center, 10, Vector2D(0, 0), 0, false, 0.25, 0.25, 8);
	generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32
		, center, 4, Vector2D(0, 0), 0);
	generate<Effect>(*tl)->initialize(refx, refy, width, height
		, phys.p, 4, Vector2D(0, 0), 0, Turned(), 1.0, 0, 16);
	int offset_refx = 32 * ($P.rand.get() % 4);
	generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X + offset_refx, REFPOINT_STAR1_Y, 32, 32, Center() - Vector2D(16, 16)
		, 12, Vector2D(0, 0), 0, 6, 4.0);*/
	//generate<DyingBoss>(*tl)->initialize(refx, refy, width, height
	//	, phys.p, 4, Turned());

}

const gfw::VirtualController& Block::Controller()
{
	return mc;
}