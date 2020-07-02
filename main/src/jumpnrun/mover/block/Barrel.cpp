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

RegisterMoverClass(Barrel);

void Barrel::initialize(int refx, int refy, gmtr::Vector2D p, OWNERHANDLE owner)
{
	Block::initialize(refx, refy, p, owner, 1, 15);
	width = height = 32;
	margin_top = 0;
	margin_bottom = 0;
	margin_side = 2;
	interacting_with_blocks = true;
	smashable = true;
	enchantable = false;
	rolling = 0;
	writeName("block", name);
}

void Barrel::run(jnr::JumpnRunData& data)
{
	if (v_smash.x != 0)
	{
		rolling = (v_smash.x > 0) * 2 - 1;
	}
	if (v_reaction.x != 0)
		rolling = 0;

	if (rolling > 0)
	{
		v_smash = $V(0, 0);
		mc.push_right = true;
	}
	else if (rolling < 0)
	{
		v_smash = $V(0, 0);
		mc.push_left = true;

	}

	Block::run(data);
}

void Barrel::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int offset_x = rolling != 0 ? (age / 15 % 2) + 1 : 0;
	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx + width * offset_x, refy, width, height, turned, 0xffffffff);
}