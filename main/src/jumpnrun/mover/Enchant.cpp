#include"jumpnrun/mover/Enchant.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/GRAPHICS.h"
using namespace gfw;
using namespace jnr;

RegisterMoverClass(Enchant);
RegisterMoverClass(EnchantBind);
RegisterMoverClass(EnchantMuscle);
RegisterMoverClass(EnchantSlow);

void Enchant::initialize(int refx, int refy, OWNERHANDLE owner, Character* chara, int lifetime)
{
	Mover::initialize(-1, -1, gmtr::Vector2D(0, 0), owner);
	this->pipe_next = Object::Pipe(nullptr, *tl);
	this->pipe_prev = Object::Pipe(nullptr, *tl);
	this->pipe_chara = Object::Pipe(chara, *tl);
	this->lifetime = lifetime;
	for (int i = 0; i < 5; i++)
	{
		particles[i].x = particles[i].y = 0;
		timer_particles[i] = -1;
	}
	this->refx = refx;
	this->refy = refy;
	width = height = 32;
	writeName("enchant_noname", name);
}
void Enchant::run(jnr::JumpnRunData& data)
{
	Mover::run(data);

	for (int i = 0; i < 5; i++)
	{
		particles[i].y -= 0.5;
		if (timer_particles[i] >= 0)
			timer_particles[i]--;
		else if ($P.rand.get() % 20 == 0)
		{
			timer_particles[i] = 55 + $P.rand.get() % 30;
			particles[i].y = 0;
			particles[i].x = ($P.rand.get() % 56) - 28;
		}
	}
	if (!pipe_chara.Dst(*tl) || !pipe_chara.Dst(*tl)->alive)
	{
		die(data);
		return;
	}
	else
	{
		effect(data);
		Character* c = static_cast<Character*>(pipe_chara.Dst(*tl));
		this->phys.p = c->P() + gmtr::Vector2D(0, +c->Height());
	}
	if (lifetime >= 0 && age > lifetime)
		die(data);
}

void Enchant::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	//if (!pipe_chara.Dst(*tl) || !pipe_chara.Dst(*tl)->alive)
	//	return;
	//Character* c = static_cast<Character*>(pipe_chara.Dst(*tl));
	for (int i = 0; i < 5; i++)
	{
		if (timer_particles[i] >= 0)
		{
			graphics->draw($P.img_obj, particles[i].x + P().x - data.camera.X()
				, particles[i].y + P().y - height - data.camera.Y(), 0
				, refx, refy, width, height, i % 2);
		}
	}
}

void Enchant::die(jnr::JumpnRunData& data)
{
	if (Object* ptr = pipe_next.Dst(*tl))
	{
		Enchant* next = static_cast<Enchant*>(ptr);
		next->setPrev(pipe_prev);
	}
	if (Object* ptr = pipe_prev.Dst(*tl))
	{
		Enchant* prev = static_cast<Enchant*>(ptr);
		prev->setNext(pipe_next);
	}
	else if (Character* c = static_cast<Character*>(pipe_chara.Dst(*tl)))
	{
		if (c->getTopEnchant() == this)
		{
			auto* ptr = pipe_next.Dst(*tl);
			c->changeTopEnchant(static_cast<Enchant*>(ptr));
		}
	}

	alive = false;
}
void Enchant::cancel()
{
	lifetime = 0;

}


void EnchantSlow::initialize(int refx, int refy, OWNERHANDLE owner, Character* chara
	, int lifetime, double val)
{
	Enchant::initialize(refx, refy, owner, chara, lifetime);
	writeName("enchant_slow", name);
	double s = chara->SpeedDash();
	chara->SpeedDash(s * val);
	s = chara->SpeedWalk();
	chara->SpeedWalk(s * val);
	value = val;
}
void EnchantSlow::die(jnr::JumpnRunData& data)
{
	if (auto* chara = static_cast<Character*>(pipe_chara.Dst(*tl)))
	{
		double s = chara->SpeedDash();
		chara->SpeedDash(s / value);
		s = chara->SpeedWalk();
		chara->SpeedWalk(s / value);
	}
	Enchant::die(data);
}

void EnchantMuscle::initialize(int refx, int refy, OWNERHANDLE owner, Character* chara
	, int lifetime, double val)
{
	Enchant::initialize(refx, refy, owner, chara, lifetime);
	writeName("enchant_muscle", name);
	double s = chara->Muscle();
	chara->Muscle(s + val);
	value = val;
}
void EnchantMuscle::die(jnr::JumpnRunData& data)
{
	if (auto* chara = static_cast<Character*>(pipe_chara.Dst(*tl)))
	{
		double s = chara->Muscle();
		chara->Muscle(s - value);
	}
	Enchant::die(data);
}

void EnchantConcentrate::initialize(int refx, int refy, OWNERHANDLE owner, Character* chara
	, int lifetime, double val)
{
	Enchant::initialize(refx, refy, owner, chara, lifetime);
	writeName("enchant_concentrate", name);
	double s = chara->Concentration();
	chara->Concentration(s + val);
	value = val;
}
void EnchantConcentrate::die(jnr::JumpnRunData& data)
{
	if (auto* chara = static_cast<Character*>(pipe_chara.Dst(*tl)))
	{
		double s = chara->Concentration();
		chara->Concentration(s - value);
	}
	Enchant::die(data);
}


void EnchantMPHPRate::initialize(int refx, int refy, OWNERHANDLE owner, Character* chara
	, int lifetime, double val)
{
	Enchant::initialize(refx, refy, owner, chara, lifetime);
	writeName("enchant_mphprate", name);
	double s = chara->MPHPRate();
	chara->MPHPRate(val);
	value = val;
}
void EnchantMPHPRate::die(jnr::JumpnRunData& data)
{
	if (auto* chara = static_cast<Character*>(pipe_chara.Dst(*tl)))
	{
		double s = chara->MPHPRate();
		chara->MPHPRate(value);
	}
	Enchant::die(data);
}


void EnchantOwner::initialize(int refx, int refy, OWNERHANDLE owner, Character* chara
	, int lifetime, OWNERHANDLE owner_new)
{
	Enchant::initialize(refx, refy, owner, chara, lifetime);
	writeName("enchant_owner", name);
	this->owner_new = owner_new;
	this->owner_old = chara->OwnerOriginal();
	chara->Owner(owner_new);
}
void EnchantOwner::die(jnr::JumpnRunData& data)
{
	if (auto* chara = static_cast<Character*>(pipe_chara.Dst(*tl)))
	{
		chara->Owner(this->owner_old);
	}
	Enchant::die(data);
}


void EnchantBind::initialize(int refx, int refy, OWNERHANDLE owner, Character* chara
	, int lifetime)
{
	Enchant::initialize(refx, refy, owner, chara, lifetime);
	writeName("enchant_bind", name);
}

void EnchantBind::effect(jnr::JumpnRunData& data){}
void EnchantBind::die(jnr::JumpnRunData& data){}
void EnchantBind::render(gfw::BaseMedia& media, jnr::JumpnRunData& data){}