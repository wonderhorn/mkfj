#include"jumpnrun/mover/bullet/Bubble.h"
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

RegisterMoverClass(Willowisp);

void Willowisp::initialize(int refx, int refy, jnr::Character& chara
	, gmtr::Vector2D p, gmtr::Vector2D v, int power, OWNERHANDLE owner
	, double phase_offset)
{
	Bubble::initialize(refx, refy, p, v, power, owner);
	pipe = gfw::Object::Pipe(&chara, *tl);
	phys.mass = 0;
	this->phase_offset = phase_offset;
	interacting_with_stages = false;
	interacting_with_blocks = false;
	touch_and_dissapear = false;
	writeName("willowisp", name);

}
void Willowisp::run(jnr::JumpnRunData& data)
{
	is_damaging = (age >= 45);
	if (Character* c = static_cast<Character*>(pipe.Dst(*tl)))
	{
		auto p_target = c->Center() - $V(width / 2, height / 2)
			+ $V::polar(48, (float)age / 45 + phase_offset);
		auto dp = p_target - P();
		phys.v = dp / 6;
	}
	else
	{
		finalize();
	}
	Bubble::run(data);
}
void Willowisp::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int phase = age / 12 % 4;
	int alpha = (age / 6 % 2) >= 1 ? 0xffffffff : 0xddffffff;
	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx, refy + height * (phase % 2), width, height, phase >= 2, alpha);
}
