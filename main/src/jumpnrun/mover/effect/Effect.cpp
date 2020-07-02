#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/GRAPHICS.h"
#include"jumpnrun/system/Parmanent.h"
using namespace jnr;
using namespace gfw;
using namespace gmtr;
using std::min;
using std::max;
RegisterMoverClass(Effect);
RegisterMoverClass(BossDummy);
RegisterMoverClass(DyingBoss);
RegisterMoverClass(EffectOnChara);
RegisterMoverClass(WaterBubble);

void Effect::initialize(int refx, int refy, int w, int h
	, gmtr::Vector2D p, int lifetime, gmtr::Vector2D v, double mass, bool turned, double diverge_start_rate
	, double diverge_speed, int dissapear_speed, int appear_speed)
{
	Mover::initialize(refx, refy, p, owner);
	this->lifetime = lifetime;
	phys.v = v;
	phys.mass = mass;
	width = w;
	height = h;
	alpha = 0xffffffff;
	this->diverge_speed = diverge_speed;
	this->dissapear_speed = dissapear_speed;
	this->appear_speed = appear_speed;
	timer_dissapear = 0xff;
	timer_appear = 0;
	rate = diverge_start_rate;
	this->turned = turned;
	period_onoff = 0;
	y_anim_num = 1;
	y_anim_interval = 30;
	writeName("effect", name);
}

void Effect::run(jnr::JumpnRunData& data)
{
	if (diverge_speed)
	{
		rate += diverge_speed;
		if (rate < 0)
			rate = 0;
	}

	if (appear_speed)
	{
		timer_appear += appear_speed;
		if (timer_appear >= 0xff)
			timer_appear = 0xff;
		alpha = (timer_appear << 24) | (alpha & 0x00FFFFFF);
	}
	else timer_appear = 0xff;
	if (dissapear_speed && timer_appear == 0xff)
	{
		if (timer_dissapear > dissapear_speed)
			timer_dissapear -= dissapear_speed;
		else
			timer_dissapear = 0;
		alpha = (timer_dissapear << 24) | (alpha & 0x00FFFFFF);
	}

	if (age > lifetime)
	{
		finalize();
	}
	Mover::run(data);
}
void Effect::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int refy_offset = height * (age / y_anim_interval % y_anim_num);
	if (period_onoff)
	{
		if (age / period_onoff % 2 == 1)
			return;
	}
	if (rate == 1.0 && alpha == 0xffffffff)
		graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx, refy + refy_offset, width, height, turned);
	else
		graphics->drawSpinResize($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx, refy + refy_offset, width, height, turned, alpha, rate * (1.0 - 2.0*turned), rate, 0.0);

}


void EffectOnChara::initialize(int refx, int refy, int w, int h
	, gmtr::Vector2D p, Character* chara, int lifetime, gmtr::Vector2D v, double mass, bool turned, double diverge_start_rate
	, double diverge_speed, int dissapear_speed, int appear_speed)
{
	Effect::initialize(refx, refy, w, h
		, p, lifetime, v, mass, turned, diverge_start_rate
		, diverge_speed, dissapear_speed, appear_speed);
	connector = p - chara->P();
	pipe = Pipe(chara, *tl);
}


void EffectOnChara::run(jnr::JumpnRunData& data)
{
	Effect::run(data);
	if (auto* ptr = pipe.Dst(*tl))
	{
		Character* c = static_cast<Character*>(ptr);
		if (!c->alive)
		{
			finalize();
			pipe = Pipe(nullptr, *tl);
		}
		else
		{
			phys.p = connector + c->P();
		}
	}
}

void WaterBubble::run(jnr::JumpnRunData& data)
{
	Effect::run(data);
	if (location != 'w')
	{
		alive = false;
	}
}

void BossDummy::initialize(int refx, int refy, int w, int h
	, gmtr::Vector2D p, double mass, bool turned)
{
	Effect::initialize(refx, refy, w, h, p, 9999999, gmtr::Vector2D(0, 0), mass, turned);
	to_turn = false;
	writeName("effect_bossdummy", name);
}
void BossDummy::run(jnr::JumpnRunData& data)
{
	turned = $P.turn_boss_dummy;
	//if (data.encount_boss >= 0 && ! to_turn && data.turn_boss_dummy)
	//{
	//	turned = !turned;
	//	to_turn = true;
	//}
	if (data.in_boss_battle)
	{
		alive = false;
	}
	Effect::run(data);
}
void BossDummy::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	turned = $P.turn_boss_dummy;
	if ($P.appear_boss_dummy)
		Effect::render(media, data);
}


void BigEyeDummy::initialize(int refx, int refy, int w, int h
	, gmtr::Vector2D p, double mass, bool turned)
{
	Effect::initialize(refx, refy, w, h, p, 9999999, gmtr::Vector2D(0, 0), mass, turned);
	writeName("effect_bossdummy", name);
}
void BigEyeDummy::run(jnr::JumpnRunData& data)
{
	turned = $P.turn_boss_dummy;
	//if (data.encount_boss >= 0 && ! to_turn && data.turn_boss_dummy)
	//{
	//	turned = !turned;
	//	to_turn = true;
	//}
	if (data.in_boss_battle)
	{
		alive = false;
	}
	Effect::run(data);
}
void BigEyeDummy::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	if ($P.appear_boss_dummy)
	{
		BaseGraphics* graphics = media.getGraphics();
		int refy_offset = height * (age / y_anim_interval % y_anim_num);

		graphics->draw($P.img_bigeye, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
				, refx, refy + refy_offset, width, height, turned);
	}
}

void DyingBoss::initialize(int refx, int refy, int w, int h
	, gmtr::Vector2D p, double mass, bool turned)
{
	Effect::initialize(refx, refy, w, h, p, 105, gmtr::Vector2D(0, 0), mass, turned
		, 1.0, 0, 2, 0);
	writeName("effect_dyingboss", name);
	$P.sound_stack.push_back($P.snd_die);
}
void DyingBoss::run(jnr::JumpnRunData& data)
{
	if (age % 15 == 0)
	{
		int reflist_x[] = { 0, 32, 32 };
		int reflist_y[] = { 0, 0, 32 };
		int idx = (age / 15) % 3;
		double theta = (double)age / lifetime * 3.14 * 2;
		Vector2D center = phys.p + Vector2D(width / 2 - 16, height / 2 - 16) + Vector2D(12 * cos(theta), 12 * sin(theta));
		generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_X + reflist_x[idx], REFPOINT_EXPLOSION_Y + reflist_y[idx], 32, 32
			, center, 10, Vector2D(0, 0), 0, false, 0.25, 0.25, 8);
		generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32
			, center, 4, Vector2D(0, 0), 0);
		generate<Effect>(*tl)->initialize(refx, refy, width, height
			, phys.p, 4, Vector2D(0, 0), 0, Turned(), 1.0, 0, 16);
		$P.sound_stack.push_back($P.snd_exp);
	}
	if (age == lifetime - 1)
	{
		generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32, Center() - Vector2D(16, 16)
			, 18, Vector2D(0, 0), 0, 8, 5.0);
		$P.sound_stack.push_back($P.snd_exp);
		$P.sound_stack.push_back($P.snd_exp);
		int lt = 300 - 71 ; // 180 - 71
		if (refy == REFPOINT_ME_Y)
			lt = 180;
		//$P.sound_stack.push_back($P.snd_die);
		generate<Effect>(*tl)->initialize(refx + 9 * width, refy, 64, 64
			, phys.p - gmtr::Vector2D(16.0, 12), lt, gmtr::Vector2D(0.0, 0.0), 1.5, turned, 1, 0, 0, 16);
	}
	Effect::run(data);
}

void DyingBossFlame::initialize(int refx, int refy, int w, int h
	, gmtr::Vector2D p, double mass, bool turned)
{
	Effect::initialize(refx, refy, w, h, p, 210, gmtr::Vector2D(0, 0), mass, turned
		, 1.0, 0, 1, 0);
	writeName("effect_dyingbossf", name);
	$P.sound_stack.push_back($P.snd_die);
}
void DyingBossFlame::run(jnr::JumpnRunData& data)
{
	if (age % 15 == 0)
	{
		int reflist_x[] = { 0, 32, 32 };
		int reflist_y[] = { 0, 0, 32 };
		int idx = (age / 15) % 3;
		double theta = (double)age / lifetime * 3.14 * 2;
		Vector2D center = phys.p + Vector2D(width / 2 - 16, height / 2 - 16) + Vector2D(12 * cos(theta), 12 * sin(theta));
		generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_X + reflist_x[idx], REFPOINT_EXPLOSION_Y + reflist_y[idx], 32, 32
			, center, 10, Vector2D(0, 0), 0, false, 0.25, 0.25, 8);
		generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32
			, center, 4, Vector2D(0, 0), 0);
		generate<Effect>(*tl)->initialize(refx, refy, width, height
			, phys.p, 4, Vector2D(0, 0), 0, Turned(), 1.0, 0, 16);
		$P.sound_stack.push_back($P.snd_exp);
	}
	if (age >= 60 && age % 15 == 0)
	{
		if(age % 30 == 0)
			generate<Particles>(*tl)->initialize(REFPOINT_FIRE_X, REFPOINT_FIRE_Y, 32, 32, Center() - Vector2D(16, 16)
				, 20, Vector2D(0, 0), 0, 8, 5.0);
		else
			generate<Particles>(*tl)->initialize(REFPOINT_FIRE_X, REFPOINT_FIRE_Y, 32, 32, Center() - Vector2D(16, 16)
				, 20, Vector2D(0, 0), 0, 12, 5.0);
		$P.sound_stack.push_back($P.snd_ignition);

		
	}
	if (age == lifetime - 60)
	{
		$P.flags["kyo2ign"] = true;
	}
	if (age == lifetime - 1)
	{
		generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32, Center() - Vector2D(16, 16)
			, 18, Vector2D(0, 0), 0, 8, 5.0);
		$P.sound_stack.push_back($P.snd_exp);
		$P.sound_stack.push_back($P.snd_exp);
		int lt = 300 - 71; // 180 - 71
		if (refy == REFPOINT_ME_Y)
			lt = 180;
		//$P.sound_stack.push_back($P.snd_die);
		generate<Effect>(*tl)->initialize(refx + 9 * width, refy, 64, 64
			, phys.p - gmtr::Vector2D(16.0, 12), lt, gmtr::Vector2D(0.0, 0.0), 1.5, turned, 1, 0, 0, 16);
	}
	Effect::run(data);
}

