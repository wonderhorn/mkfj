#define _USE_MATH_DEFINES
#include<cmath>
#include"jumpnrun/mover/bullet/Bullet.h"
#include"jumpnrun/mover/Enchant.h"
#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/MoverUtilities.h"
#include"jumpnrun/system/Parmanent.h"
#include"utils/MyCppUtil.h"
#include"jumpnrun/GRAPHICS.h"
#include<cmath>
#include<functional>
#include<algorithm>
using namespace jnr;
using namespace gfw;
using namespace gmtr;
#define $V gmtr::Vector2D

RegisterMoverClass(SlowBall);

void SlowBall::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v, double mass
	, bool rotate_to_direction)
{
	Bullet::initialize(refx, refy, p, owner, power, v, mass, rotate_to_direction);

	writeName("bullet_slowball", name);
	margin_top = margin_side = margin_bottom = 8;
	this->rotate_to_direction = rotate_to_direction;
	bound_rate = 0.8;
	num_bounding = 50;
	lifetime = 300;
	margin_top = 12;
	touch_and_dissapear = false;
	value = 0.25;
	period = 10 * 60;
}
void SlowBall::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	if (chara.Owner() == this->Owner())
		return;
	$V center = phys.p + $V(width / 2, height / 2);
	if (chara.Shape().includes(center))
	{
		Damage dam;
		dam.power = power;
		dam.stun_time = Constants::time_stun;
		dam.invinc_time = Constants::time_invinc;
		dam.isPhysical = false;
		dam.owner = this->Owner();
		auto smashv = this->phys.v.regularized();
		if (smashv.l2() == 0)
			smashv = $V(0, -1);
		dam.smash = smashv * 4.0 + $V(0, -4) - chara.V();
		int d = chara.damage(dam);
		if (d >= 0)
		{
			generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32, chara.Center() - Vector2D(16, 16)
				, 12, Vector2D(0, 0), 0, 6, 5.0);
			auto* e0 = generate<EnchantSlow>(*tl);
			e0->initialize(REFPOINT_CURSE_X, REFPOINT_CURSE_Y, 0, &chara, 60 * 10, 0.25);
			chara.setEnchant(e0);
			generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, phys.p, 2, $V(0.0, 0.0), 0);
			generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, phys.p, 4, $V(0.0, 0.0), 0);
			chara.Smash(this->phys.v.regularized() * 4.0 + $V(0, -4));
			finalize();
		}
	}
}

void SlowBall::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int offset_y = (age / 8 % 2) * height;
		graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx, refy + offset_y, width, height, turned);
}