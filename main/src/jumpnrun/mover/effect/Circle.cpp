#define _USE_MATH_DEFINES
#include"jumpnrun/mover/effect/Circle.h"
#include"jumpnrun/mover/bullet/Bullet.h"
#include"jumpnrun/mover/enemy/Enemy.h"
#include"jumpnrun/mover/block/block.h"
#include"jumpnrun/mover/bullet/Bubble.h"
#include"jumpnrun/MoverUtilities.h"
#include"utils/MyCppUtil.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/GRAPHICS.h"
#include<math.h>
using namespace gfw;
using namespace jnr;
using namespace gmtr;

RegisterMoverClass(Circle);
RegisterMoverClass(CircleBarrier);
RegisterMoverClass(CircleBubble);
RegisterMoverClass(CircleExplosion);
RegisterMoverClass(CircleFlame);
RegisterMoverClass(CircleGas);
RegisterMoverClass(CircleSummonAngel);
RegisterMoverClass(CircleSummonBigSlime);
RegisterMoverClass(CircleSummonCandle);
RegisterMoverClass(CircleSummonDirtCube);
RegisterMoverClass(CircleSummonFrog);
RegisterMoverClass(CircleSummonGhost);
RegisterMoverClass(CircleSummonLamp);
RegisterMoverClass(CircleSummonManaFlower);
RegisterMoverClass(CircleSummonMushroom);

void Circle::initialize(int refx, int refy, int refx_color, int refy_color
	, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime, gmtr::Vector2D v, double spin_speed
	, double diverge_start_rate, double diverge_speed, int dissapear_speed, int appear_speed)
{
	Effect::initialize(refx, refy, 64, 64, p, lifetime, v, 0, false
		, diverge_start_rate, diverge_speed, dissapear_speed, appear_speed);
	this->owner = owner;
	this->refx_color = refx_color;
	this->refy_color = refy_color;
	theta = 0.0;
	this->spin_speed = spin_speed;
	this->actiontime = actiontime;
	writeName("effect_circle", name);
	$P.sound_stack.push_back($P.snd_magic);
}
void Circle::run(jnr::JumpnRunData& data)
{
	Effect::run(data);
	if (age >= actiontime)
	{
		actiontime = INT_MAX;
		actionOnTime(data);
	}
	theta += M_PI * spin_speed;
}

void Circle::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	graphics->drawSpinResize($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx, refy, width, height, turned, alpha, rate, rate / 2, theta);
	graphics->drawSpinResize($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx_color, refy_color, width, height, turned, alpha, rate, rate / 2, theta);
}


void CircleExplosion::initialize(int refx, int refy, int refx_color, int refy_color
	, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime, gmtr::Vector2D v, double spin_speed
	, double diverge_start_rate, double diverge_speed, int dissapear_speed, int appear_speed)
{
	Circle::initialize(refx, refy, refx_color, refy_color, p, owner, lifetime, actiontime, v, spin_speed
		, diverge_start_rate, diverge_speed, dissapear_speed, appear_speed);
	writeName("circle_explosion", this->name);
}
void CircleExplosion::actionOnTime(jnr::JumpnRunData& data)
{
	generate<Explosion>(*tl)->initialize(REFPOINT_EXPLOSION_X, REFPOINT_EXPLOSION_Y, 
		P() + Vector2D(16, -36), -1, 24);
	generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32,
		P() + Vector2D(16, 0), 6, Vector2D(0, 0), 0);
}

void CircleGas::initialize(int refx, int refy, int refx_color, int refy_color
	, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime, gmtr::Vector2D v, double spin_speed
	, double diverge_start_rate, double diverge_speed, int dissapear_speed, int appear_speed)
{
	Circle::initialize(refx, refy, refx_color, refy_color, p, owner, lifetime, actiontime, v, spin_speed
		, diverge_start_rate, diverge_speed, dissapear_speed, appear_speed);
	writeName("circle_gas", this->name);
}
void CircleGas::actionOnTime(jnr::JumpnRunData& data)
{
	for (int i = 0; i < 2; i++)
	{
		Vector2D dir = Vector2D::polar(1.0, (double)$P.rand.get() / RAND_MAX * M_PI + M_PI);
		generate<Gas>(*tl)->initialize(REFPOINT_GAS_X, REFPOINT_GAS_Y, 
			P() + Vector2D(16, -36), Owner(), 1, dir, 1);
		$P.sound_stack.push_back($P.snd_blink);
	}
	actiontime = age + 20;
}

void CircleBubble::initialize(int refx, int refy, int refx_color, int refy_color
	, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime, gmtr::Vector2D v, double spin_speed
	, double diverge_start_rate, double diverge_speed, int dissapear_speed, int appear_speed)
{
	Circle::initialize(refx, refy, refx_color, refy_color, p, owner, lifetime, actiontime, v, spin_speed
		, diverge_start_rate, diverge_speed, dissapear_speed, appear_speed);
	writeName("circle_bubble", this->name);
}
void CircleBubble::actionOnTime(jnr::JumpnRunData& data)
{
	//for (int i = 0; i < 3; i++)
	{
		Vector2D dir = Vector2D($P.rand.get() % 5 - 2, -4);
		generate<Bubble>(*tl)->initialize(REFPOINT_BUBBLE_X, REFPOINT_BUBBLE_Y
			, P() + Vector2D(16, -36), dir, 1, Owner());
		$P.sound_stack.push_back($P.snd_blink);
	}
	actiontime = age + 10;
}


class CircleBarrierDummyCharacter : public Character
{
public:
	virtual void initialize(gmtr::Vector2D p, int w, int h, OWNERHANDLE owner)
	{
		Character::initialize(-1, -1, p, owner);
		this->width = w;
		this->height = h;
		this->margin_bottom = 0;
		this->margin_top = 0;
		this->margin_side = 0;
		this->interacting_with_items = false;
		this->wet = true;
	}

	virtual const gfw::VirtualController& Controller() { return mc; }
	virtual int damage(int power, int stun_time, int invinc_time)
	{
		return Repelled;
	}
	virtual int damage(const Damage& d) { return Repelled; }

	void render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
	{
		BaseGraphics* graphics = media.getGraphics();
		graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
			, REFPOINT_CIRCLE_LIGHT_X, REFPOINT_CIRCLE_LIGHT_Y, width, height, false, 0xaaffffff);
	}
	gfw::ManualController mc;
};

void CircleBarrier::initialize(int refx, int refy, int refx_color, int refy_color
	, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime, gmtr::Vector2D v, double spin_speed
	, double diverge_start_rate, double diverge_speed, int dissapear_speed, int appear_speed)
{
	Circle::initialize(refx, refy, refx_color, refy_color, p, owner, lifetime, actiontime, v, spin_speed
		, diverge_start_rate, diverge_speed, dissapear_speed, appear_speed);
	barrier_started = false;
	writeName("circle_barrier", this->name);
}
void CircleBarrier::actionOnTime(jnr::JumpnRunData& data)
{
	if (!barrier_started)
	{
		generate<LightColumns>(*tl)->initialize(REFPOINT_LIGHT_COLUMN_X, REFPOINT_LIGHT_COLUMN_Y, 64, 64
			, P() + Vector2D(0, -40), lifetime - age, Vector2D(0, 0), 0);
	}

	barrier_started = true;
	CircleBarrierDummyCharacter dummy;
	dummy.initialize(P() - gmtr::Vector2D(0, 64), 64, 64, -1);
	dummy.Rect();
	for (auto t = tl->begin(); t != tl->end(); t++)
	{
		Object* obj = getObject(t);
		if (!obj->isMover)
			continue;
		Mover* mvr = static_cast<Mover*>(obj);
		if (mvr->ID() != this->id)
		{
			mvr->interact(dummy, data);
		}
	}
	actiontime = age + 1;
}
void CircleBarrier::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	Circle::render(media, data);
	if (barrier_started)
	{
		BaseGraphics* graphics = media.getGraphics();
	}
}

void CircleFlame::initialize(int refx, int refy, int refx_color, int refy_color
	, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime, gmtr::Vector2D v, double spin_speed
	, double diverge_start_rate, double diverge_speed, int dissapear_speed, int appear_speed)
{
	Circle::initialize(refx, refy, refx_color, refy_color, p, owner, lifetime, actiontime, v, spin_speed
		, diverge_start_rate, diverge_speed, dissapear_speed, appear_speed);
	writeName("circle_flame", this->name);
}
void CircleFlame::actionOnTime(jnr::JumpnRunData& data)
{
	//for (int i = 0; i < 3; i++)
	{
		Vector2D dir = Vector2D($P.rand.get() % 5 - 2, -4);
		generate<Flame>(*tl)->initialize(REFPOINT_FIRE_X, REFPOINT_FIRE_Y, P() + Vector2D(16 - 8, 0)
			, Owner(), 15, gmtr::Vector2D(0, -4), 0, 4);
		generate<Flame>(*tl)->initialize(REFPOINT_FIRE_X, REFPOINT_FIRE_Y, P() + Vector2D(16 + 8, 0)
			, Owner(), 15, gmtr::Vector2D(0, -4), 0, 4);
		Effect* e = gfw::generate<Effect>(*tl);
		e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32
			, P() + Vector2D(16, 0), 4, gmtr::Vector2D(0, 0), 0);
	}
	actiontime = age + 20;
}

void CircleTrapExplosion::initialize(int refx, int refy, int refx_color, int refy_color
	, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime, gmtr::Vector2D v, double spin_speed
	, double diverge_start_rate, double diverge_speed, int dissapear_speed, int appear_speed)
{
	Circle::initialize(refx, refy, refx_color, refy_color, p, owner, lifetime, actiontime, v, spin_speed
		, diverge_start_rate, diverge_speed, dissapear_speed, appear_speed);
	writeName("circle_trap_explosion", this->name);
}
void CircleTrapExplosion::run(jnr::JumpnRunData& data)
{
	Effect::run(data);
	if (age >= actiontime)
	{
		//actiontime = INT_MAX;
		//actionOnTime(data);
		bool found = false;
		EnemySearcher es;
		es.initialize(this);
		auto v = es.search(64 * 2);
		auto o = Owner();
		v = myutil::filter([o](const Object::Pipe & p)
		{
			const Mover* mvr = static_cast<Mover*>(p.Dst(*tl));
			auto myo = mvr->Owner();
			char c = char(myo);
			return mvr->Name().substr(0, 9) == "character";
		}, v);
		if (!v.empty())
		{
			auto p = this->Center() - gmtr::Vector2D(0, 48);
			std::sort(v.begin(), v.end(), [p](const Object::Pipe & p1, const Object::Pipe & p2)
			{return (p - static_cast<Mover*>(p1.Dst(*tl))->Center()).sqsum()
				< (p - static_cast<Mover*>(p2.Dst(*tl))->Center()).sqsum(); });
			auto dp = static_cast<Mover*>(v.begin()->Dst(*tl))->Center() - p;
			double sqsum_dp = dp.sqsum();
			if (sqsum_dp < 48 * 48)
			{
				found = true;
			}
		}

		if (found)
		{
			generate<Explosion>(*tl)->initialize(REFPOINT_EXPLOSION_X, REFPOINT_EXPLOSION_Y, P() + Vector2D(16, -36), -1, 25);
			generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32,
				P() + Vector2D(16, 0), 6, Vector2D(0, 0), 0);
			actiontime = INT_MAX;
			lifetime = age + 20;
		}
	}
	theta += M_PI * spin_speed;
}
void CircleTrapExplosion::actionOnTime(jnr::JumpnRunData& data)
{
	//for (int i = 0; i < 3; i++)
	{
	}
}


void CircleSummonMushroom::initialize(int refx, int refy, int refx_color, int refy_color
	, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
	, gmtr::Vector2D v, bool turned, double spin_speed
	, double diverge_start_rate, double diverge_speed, int dissapear_speed, int appear_speed)
{
	Circle::initialize(refx, refy, refx_color, refy_color, p, owner, lifetime, actiontime, v, spin_speed
		, diverge_start_rate, diverge_speed, dissapear_speed, appear_speed);
	summon_turned = turned;
	writeName("circle_summon_mushroom", this->name);
}
void CircleSummonMushroom::actionOnTime(jnr::JumpnRunData& data)
{
	generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32, Center() - Vector2D(16, 16)
		, 12, Vector2D(0, 0), 0, 6, 5.0);
	auto* ptr = generate<enm::Mushroom>(*tl);
	ptr->initialize(REFPOINT_MUSHROOM_X, REFPOINT_MUSHROOM_Y, P() + Vector2D(16, -48), Owner(), summon_turned);
	ptr->resetHP_HPMax(15);
}

void CircleSummonFrog::initialize(int refx, int refy, int refx_color, int refy_color
	, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
	, gmtr::Vector2D v, bool turned, double spin_speed
	, double diverge_start_rate, double diverge_speed, int dissapear_speed, int appear_speed)
{
	Circle::initialize(refx, refy, refx_color, refy_color, p, owner, lifetime, actiontime, v, spin_speed
		, diverge_start_rate, diverge_speed, dissapear_speed, appear_speed);
	summon_turned = turned;
	writeName("circle_summon_frog", this->name);
}
void CircleSummonFrog::actionOnTime(jnr::JumpnRunData& data)
{
	generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32, Center() - Vector2D(16, 16)
		, 12, Vector2D(0, 0), 0, 6, 5.0);
	auto* ptr = generate<enm::Frog>(*tl);
	ptr->initialize(REFPOINT_FROG_X, REFPOINT_FROG_Y, P() + Vector2D(16, -48), Owner(), summon_turned);
	ptr->resetHP_HPMax(15);
}


void CircleSummonDirtCube::initialize(int refx, int refy, int refx_color, int refy_color
	, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
	, gmtr::Vector2D v, bool turned, double spin_speed
	, double diverge_start_rate, double diverge_speed, int dissapear_speed, int appear_speed)
{
	Circle::initialize(refx, refy, refx_color, refy_color, p, owner, lifetime, actiontime, v, spin_speed
		, diverge_start_rate, diverge_speed, dissapear_speed, appear_speed);
	writeName("circle_summon_dirtcube", this->name);
}
void CircleSummonDirtCube::actionOnTime(jnr::JumpnRunData& data)
{
	generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32, Center() - Vector2D(16, 16)
		, 12, Vector2D(0, 0), 0, 6, 5.0);
	generate<Block>(*tl)->initialize(REFPOINT_BLOCK_X + 32, REFPOINT_BLOCK_Y, P() + Vector2D(16, -48), Owner());
}

void CircleSummonBombBox::initialize(int refx, int refy, int refx_color, int refy_color
	, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
	, gmtr::Vector2D v, bool turned, double spin_speed
	, double diverge_start_rate, double diverge_speed, int dissapear_speed, int appear_speed)
{
	Circle::initialize(refx, refy, refx_color, refy_color, p, owner, lifetime, actiontime, v, spin_speed
		, diverge_start_rate, diverge_speed, dissapear_speed, appear_speed);
	writeName("circle_summon_bombbox", this->name);
}
void CircleSummonBombBox::actionOnTime(jnr::JumpnRunData& data)
{
	generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32, Center() - Vector2D(16, 16)
		, 12, Vector2D(0, 0), 0, 6, 5.0);
	generate<BombBox>(*tl)->initialize(REFPOINT_BOMBBOX_X, REFPOINT_BOMBBOX_Y, P() + Vector2D(16, -48), Owner());
}

void CircleSummonGhost::initialize(int refx, int refy, int refx_color, int refy_color
	, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
	, gmtr::Vector2D v, bool turned, double spin_speed
	, double diverge_start_rate, double diverge_speed, int dissapear_speed, int appear_speed)
{
	Circle::initialize(refx, refy, refx_color, refy_color, p, owner, lifetime, actiontime, v, spin_speed
		, diverge_start_rate, diverge_speed, dissapear_speed, appear_speed);
	summon_turned = turned;
	writeName("circle_summon_ghost", this->name);
}
void CircleSummonGhost::actionOnTime(jnr::JumpnRunData& data)
{
	auto* ptr = generate<enm::Ghost>(*tl);
	ptr->initialize(REFPOINT_GHOST_X, REFPOINT_GHOST_Y, P() + Vector2D(16, -48), Owner(), summon_turned);
	ptr->resetHP_HPMax(5);
	generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32, Center() - Vector2D(16, 16)
		, 12, Vector2D(0, 0), 0, 6, 5.0);
}

void CircleSummonKnight::initialize(int refx, int refy, int refx_color, int refy_color
	, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
	, gmtr::Vector2D v, bool turned, double spin_speed
	, double diverge_start_rate, double diverge_speed, int dissapear_speed, int appear_speed)
{
	Circle::initialize(refx, refy, refx_color, refy_color, p, owner, lifetime, actiontime, v, spin_speed
		, diverge_start_rate, diverge_speed, dissapear_speed, appear_speed);
	summon_turned = turned;
	writeName("circle_summon_knight", this->name);
}
void CircleSummonKnight::actionOnTime(jnr::JumpnRunData& data)
{
	auto* ptr = generate<enm::Knight>(*tl);
	ptr->initialize(REFPOINT_KNIGHT_X, REFPOINT_KNIGHT_Y, P() + Vector2D(16, -48), Owner(), summon_turned);
	ptr->resetHP_HPMax(20);
	generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32, Center() - Vector2D(16, 16)
		, 12, Vector2D(0, 0), 0, 6, 5.0);
}

void CircleSummonCandle::initialize(int refx, int refy, int refx_color, int refy_color
	, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
	, gmtr::Vector2D v, bool turned, double spin_speed
	, double diverge_start_rate, double diverge_speed, int dissapear_speed, int appear_speed)
{
	Circle::initialize(refx, refy, refx_color, refy_color, p, owner, lifetime, actiontime, v, spin_speed
		, diverge_start_rate, diverge_speed, dissapear_speed, appear_speed);
	summon_turned = turned;
	writeName("circle_summon_cabdle", this->name);
}
void CircleSummonCandle::actionOnTime(jnr::JumpnRunData& data)
{
	auto* ptr = generate<enm::Candle>(*tl);
	ptr->initialize(REFPOINT_CANDLE_X, REFPOINT_CANDLE_Y, P() + Vector2D(16, -48), Owner(), summon_turned);
	ptr->resetHP_HPMax(5);
	generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32, Center() - Vector2D(16, 16)
		, 12, Vector2D(0, 0), 0, 6, 5.0);
}

void CircleSummonCannon::initialize(int refx, int refy, int refx_color, int refy_color
	, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
	, gmtr::Vector2D v, bool turned, double spin_speed
	, double diverge_start_rate, double diverge_speed, int dissapear_speed, int appear_speed)
{
	Circle::initialize(refx, refy, refx_color, refy_color, p, owner, lifetime, actiontime, v, spin_speed
		, diverge_start_rate, diverge_speed, dissapear_speed, appear_speed);
	summon_turned = turned;
	writeName("circle_summon_cannon", this->name);
}
void CircleSummonCannon::actionOnTime(jnr::JumpnRunData& data)
{
	auto* ptr = generate<enm::Cannon>(*tl);
	ptr->initialize(REFPOINT_CANNON_X, REFPOINT_CANNON_Y, P() + Vector2D(16, -48), Owner(), summon_turned);
	ptr->resetHP_HPMax(20);
	
	generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32, Center() - Vector2D(16, 16)
		, 12, Vector2D(0, 0), 0, 6, 5.0);
}

void CircleSummonPumpkin::initialize(int refx, int refy, int refx_color, int refy_color
	, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
	, gmtr::Vector2D v, bool turned, double spin_speed
	, double diverge_start_rate, double diverge_speed, int dissapear_speed, int appear_speed)
{
	Circle::initialize(refx, refy, refx_color, refy_color, p, owner, lifetime, actiontime, v, spin_speed
		, diverge_start_rate, diverge_speed, dissapear_speed, appear_speed);
	summon_turned = turned;
	writeName("circle_summon_ghost", this->name);
}
void CircleSummonPumpkin::actionOnTime(jnr::JumpnRunData& data)
{
	auto* ptr = generate<enm::Pumpkin>(*tl);
	ptr->initialize(REFPOINT_PUMPKIN_X, REFPOINT_PUMPKIN_Y, P() + Vector2D(16, -48), Owner(), summon_turned);
	ptr->resetHP_HPMax(18);
	generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32, Center() - Vector2D(16, 16)
		, 12, Vector2D(0, 0), 0, 6, 5.0);
}

void CircleSummonFairyBlue::initialize(int refx, int refy, int refx_color, int refy_color
	, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
	, gmtr::Vector2D v, bool turned, double spin_speed
	, double diverge_start_rate, double diverge_speed, int dissapear_speed, int appear_speed)
{
	Circle::initialize(refx, refy, refx_color, refy_color, p, owner, lifetime, actiontime, v, spin_speed
		, diverge_start_rate, diverge_speed, dissapear_speed, appear_speed);
	summon_turned = turned;
	writeName("circle_summon_fairy_blue", this->name);
}
void CircleSummonFairyBlue::actionOnTime(jnr::JumpnRunData& data)
{
	auto* ptr = generate<enm::Fairy>(*tl);
	ptr->initialize(REFPOINT_FAIRY_BLUE_X, REFPOINT_FAIRY_BLUE_Y, P() + Vector2D(16, -48), Owner(), summon_turned);
	ptr->resetHP_HPMax(8);
	generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32, Center() - Vector2D(16, 16)
		, 12, Vector2D(0, 0), 0, 6, 5.0);
}


void CircleSummonLamp::initialize(int refx, int refy, int refx_color, int refy_color
	, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
	, gmtr::Vector2D v, bool turned, double spin_speed
	, double diverge_start_rate, double diverge_speed, int dissapear_speed, int appear_speed)
{
	Circle::initialize(refx, refy, refx_color, refy_color, p, owner, lifetime, actiontime, v, spin_speed
		, diverge_start_rate, diverge_speed, dissapear_speed, appear_speed);
	summon_turned = turned;
	writeName("circle_summon_lamp", this->name);
}
void CircleSummonLamp::actionOnTime(jnr::JumpnRunData& data)
{
	auto* ptr = generate<enm::Lamp>(*tl);
	ptr->initialize(REFPOINT_LAMP_X, REFPOINT_LAMP_Y, P() + Vector2D(16, -48), Owner(), summon_turned);
	//ptr->resetHP_HPMax(5);
	generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32, Center() - Vector2D(16, 16)
		, 12, Vector2D(0, 0), 0, 6, 5.0);
}

void CircleSummonBigSlime::initialize(int refx, int refy, int refx_color, int refy_color
	, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
	, gmtr::Vector2D v, bool turned, double spin_speed
	, double diverge_start_rate, double diverge_speed, int dissapear_speed, int appear_speed)
{
	Circle::initialize(refx, refy, refx_color, refy_color, p, owner, lifetime, actiontime, v, spin_speed
		, diverge_start_rate, diverge_speed, dissapear_speed, appear_speed);
	summon_turned = turned;
	writeName("circle_summon_bigslime", this->name);
}
void CircleSummonBigSlime::actionOnTime(jnr::JumpnRunData& data)
{
	auto* ptr = generate<enm::BigSlime>(*tl);
	ptr->initialize(REFPOINT_MERCURY_X, REFPOINT_MERCURY_Y, P() + Vector2D(16, -48), Owner(), summon_turned);
	ptr->resetHP_HPMax(20);
	generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32, Center() - Vector2D(16, 16)
		, 12, Vector2D(0, 0), 0, 6, 5.0);
}


void CircleSummonAngel::initialize(int refx, int refy, int refx_color, int refy_color
	, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
	, gmtr::Vector2D v, bool turned, double spin_speed
	, double diverge_start_rate, double diverge_speed, int dissapear_speed, int appear_speed)
{
	Circle::initialize(refx, refy, refx_color, refy_color, p, owner, lifetime, actiontime, v, spin_speed
		, diverge_start_rate, diverge_speed, dissapear_speed, appear_speed);
	summon_turned = turned;
	writeName("circle_summon_angel", this->name);
}
void CircleSummonAngel::actionOnTime(jnr::JumpnRunData& data)
{
	generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32, Center() - Vector2D(16, 16)
		, 12, Vector2D(0, 0), 0, 6, 5.0);
	auto* ptr = generate<enm::Angel>(*tl);
	ptr->initialize(REFPOINT_ANGEL_X, REFPOINT_ANGEL_Y, P() + Vector2D(16, -48), Owner(), summon_turned);
	ptr->pipe = pipe;
}

void CircleSummonElf::initialize(int refx, int refy, int refx_color, int refy_color
	, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
	, gmtr::Vector2D v, bool turned, double spin_speed
	, double diverge_start_rate, double diverge_speed, int dissapear_speed, int appear_speed)
{
	Circle::initialize(refx, refy, refx_color, refy_color, p, owner, lifetime, actiontime, v, spin_speed
		, diverge_start_rate, diverge_speed, dissapear_speed, appear_speed);
	summon_turned = turned;
	writeName("circle_summon_elf", this->name);
}
void CircleSummonElf::actionOnTime(jnr::JumpnRunData& data)
{
	generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32, Center() - Vector2D(16, 16)
		, 12, Vector2D(0, 0), 0, 6, 5.0);
	auto* ptr = generate<enm::Elf>(*tl);
	ptr->initialize(REFPOINT_ELF_X, REFPOINT_ELF_Y, P() + Vector2D(16, -48), Owner(), summon_turned);
}

void CircleSummonManaFlower::initialize(int refx, int refy, int refx_color, int refy_color
	, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
	, gmtr::Vector2D v, double spin_speed
	, double diverge_start_rate, double diverge_speed, int dissapear_speed, int appear_speed)
{
	Circle::initialize(refx, refy, refx_color, refy_color, p, owner, lifetime, actiontime, v, spin_speed
		, diverge_start_rate, diverge_speed, dissapear_speed, appear_speed);
	summon_turned = false;
	writeName("circle_summon_manaflower", this->name);
}
void CircleSummonManaFlower::actionOnTime(jnr::JumpnRunData& data)
{
	generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32, Center() - Vector2D(16, 16)
		, 12, Vector2D(0, 0), 0, 6, 5.0);
	generate<enm::ManaFlower>(*tl)->initialize(REFPOINT_MANAFLOWER_X, REFPOINT_MANAFLOWER_Y, P() + Vector2D(16, -48), -1);
}