#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/mover/effect/Circle.h"
#include"jumpnrun/mover/Barrier.h"
#include"jumpnrun/mover/Enchant.h"
#include"jumpnrun/mover/Enchant.h"
#include"jumpnrun/mover/bullet/Bullet.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/spell/Spell.h"
#include"jumpnrun/GRAPHICS.h"
using namespace jnr;
using namespace gfw;
using namespace gmtr;
#define $V Vector2D

//RegisterMoverClass(Character);

void Character::initialize(int refx, int refy, gmtr::Vector2D p, OWNERHANDLE owner)
{
	Mover::initialize(refx, refy, p, owner);
	jump_highest_y = p.y;
	num_jump = 0;
	max_num_jump = 1;
	jump_cost_hp = 0;
	jump_cost_mp = 0;
	hp_max = hp = 50;
	mp_max = mp = 50;
	mp_gain = 1;
	mp_interval = 30;
	score = 0;
	//rect = gmtr::Rectangle(p, p + $V(32, 64));
	width = 32;
	height = 64;
	phys.mass = 1.0;
	v_target.x = v_target.y = 0.0;
	speed_walk = 1.25;
	speed_dash = 2.5;
	speed_jump = 6.0;
	accel = 0.2;
	decel = 0.1;
	margin_top = 8;
	margin_side = 4;
	animation.reset(refx, refy, 2, 1, width, height);
	timer_shiver.reset(0);
	spell_index = -1;
	interacting_with_blocks = true;
	climb_steps = false;
	pick_item = false;
	this->stamping();
	isOnSomething = 3;
	interacting_with_items = true;
	entering_door = false;
	enchantable = true;
	wet = false;
	smashable = true;
	breathing = true;
	fallen_enabled = (refy == 0);
	direction_spell = $V(1 - turned * 2, 0);
	circle_refx = REFPOINT_CIRCLE_FRAME_YELLOW_X;
	circle_refx = REFPOINT_CIRCLE_FRAME_YELLOW_Y;
	snd_damaged = $P.snd_damaged;
	timer_fool = -1;
	th_damage = -1;
	concentration = 0;
	mp_hp_rate = 0.0;
	dieeffect = eDieEffectType::Expl;
	piggyback = false;
	writeName("character", name);
}

const gmtr::Rectangle& Character::Rect()
{
	rect.x1 = phys.p.x + margin_side;
	rect.x2 = phys.p.x + width - margin_side;
	rect.y1 = phys.p.y + margin_top + (status == Sitting) * (height / 2 - margin_top + 1);
	rect.y2 = phys.p.y + height;
	return rect;
}

void Character::setEnchant(Enchant* e)
{
	if (!enchantable)
	{
		e->cancel();
		return;
	}
	Object::Pipe p_new = Object::Pipe(e, *tl);
	auto* e_old = static_cast<Enchant*>(enchants.Dst(*tl));
	if (e_old)
		e_old->setPrev(p_new);
	e->setNext(enchants);
	enchants = p_new;
}
void Character::changeTopEnchant(Enchant* e)
{
	Object::Pipe p_new = Object::Pipe(e, *tl);
	enchants = p_new;
}
Enchant* Character::getTopEnchant()
{
	return static_cast<Enchant*>(enchants.Dst(*tl));
}


void Character::setBarrier(Barrier* b)
{
	if (Barrier* bb = static_cast<Barrier*>(barrier.Dst(*tl)))
	{
		if(b)
			bb->cancel();
		barrier = Object::Pipe(b, *tl);
	}
	else barrier = Object::Pipe(b, *tl);
}

Barrier* Character::getBarrier()
{
	return static_cast<Barrier*>(this->barrier.Dst(*tl));
}

int Character::damage(int power, int stun_time, int invinc_time)
{
	Damage d;
	d.power = power;
	d.stun_time = stun_time;
	d.invinc_time = invinc_time;
	d.smash = $V(0, 0);
	return damage(d);
}

int Character::damage(const Damage& d)
{
	if (!d.ignoring_invinc && timer_invinc.moving())
		return eDamage::Through;
	if (d.isPhysical && timer_invinc_physical.moving())
		return eDamage::Through;
	if (auto* ptr = barrier.Dst(*tl))
	{
		Barrier* b = static_cast<Barrier*>(ptr);
		//return b->damage(d);
		int barrier_result = b->damage(d);
		if (barrier_result != eDamage::Through)
		{
			return barrier_result;
		}
	}
	if (th_damage > d.power)
		return eDamage::Repelled;

	if (d.stun_time > 0)
	{
		if (snd_damaged >= 0)
			$P.sound_stack.push_back($P.snd_mydamaged);
		if (fallen_enabled && !piggyback && d.power >= hp / 3)//need further consideration
		{

			int offset_refx = (d.smash.x >= 0) ^ turned ? 25 : 31;
			animation.reset(refx + width * offset_refx, refy, d.stun_time * 2 / 3, 3, width*2, height);
			status = eStatus::Fallen;
			timer_invinc.reset(d.invinc_time);
			timer_stun.reset(d.stun_time * 2);
			timer_shiver.reset(d.shiver_time * 2);
		}
		else
		{
			int offset_refx = (d.smash.x >= 0) ^ turned ? 23 : 24;
			animation.reset(refx + width * offset_refx, refy, d.stun_time, 1, width, height);
			status = eStatus::Damaged;
			timer_invinc.reset(d.invinc_time);
			timer_stun.reset(d.stun_time);
			timer_shiver.reset(d.shiver_time);
		}
		
		v_target = $V(0, 0);
	}
	hp -= d.power;
	this->Smash(d.smash);
	if (std::string(name) == "character_player")
		$P.total_damage += d.power;
	return d.power;
}

void Character::run(jnr::JumpnRunData& data)
{
	timer_stun.advance();
	timer_shiver.advance();
	timer_invinc.advance();
	timer_dash.advance();
	timer_ascend.advance();
	timer_invinc_physical.advance();
	if (timer_fool > 0)
		timer_fool--;
	
	//MPMax‚ª‘å‚«‚¢‚Æ‚«‘¬‚­MP‰ñ•œ
	int vitual_mp_interval = max(mp_interval - ((mp_max / 10) - 5) - concentration, 1);
	if (age % vitual_mp_interval == 0 && mp < mp_max)
	{
		mp += mp_gain;
		if (mp > mp_max)
			mp = mp_max;
	}

	char prev_location = location;
	if (isOnSomething)
	{
		if (fallen_enabled && jump_highest_y < this->P().y - (64 * 3 + 32))
		{
			int offset_refx = 25;
			const int stun_time = 90;
			animation.reset(refx + width * offset_refx, refy, 6, 2, width * 2, height, stun_time - 6);
			status = eStatus::Fallen;
			//timer_invinc.reset(d.invinc_time);
			timer_stun.reset(stun_time);
			v_target = $V(0, 0);
			$P.sound_stack.push_back($P.snd_kick);

		}

		jump_highest_y = this->P().y;
	}
	Mover::run(data);
	
	if (isOnSomething == 0 && num_jump == 0)
		num_jump = 1;

	if (location == 'w')
	{
		num_jump = 0;
		jump_highest_y = this->P().y;
	}

	if ((location == 'w' && prev_location != 'w'
		|| location != 'w' && prev_location == 'w') && age >= 3)
	{
		auto* p1 = generate<Effect>(*tl);
		p1->initialize(REFPOINT_DUST_X + 16, REFPOINT_DUST_Y, 16, 16,
			this->P() + $V(-8 + 4, height / 2 - 16), 8, $V(0.0, 0.0), 0);
		p1->y_anim_num = 2;
		p1->y_anim_interval = 4;
		auto* p2 = generate<Effect>(*tl);
		p2->initialize(REFPOINT_DUST_X + 16, REFPOINT_DUST_Y, 16, 16,
			this->P() + $V(-8 - 4 + width, height / 2 - 16), 8, $V(0.0, 0.0), 0, true);
		p2->y_anim_num = 2;
		p2->y_anim_interval = 4;
	}

	if (!animation.finished && animation.counter == animation.spell_time)
	{
		void(*sf)(jnr::Character&, JumpnRunData&) = spl::SpellFunc(animation.spell);
		if (sf)
		{
			sf(*this, data);
		}
	}
	animation.Advance();

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

	//generate bubbles in water
	if (location == 'w' && breathing)
	{
		if($P.rand.get() % 40 == 0)
			generate<WaterBubble>(*tl)->initialize(REFPOINT_BUBBLE_CLEAR_X, REFPOINT_BUBBLE_CLEAR_Y, 32, 32,
				Center() - $V(16, 16 + 16), 20, $V(0.0, -2.0), 0);

	}
}

void Character::drive(phsc::Physics& phys, jnr::JumpnRunData& data)
{
	double mag_speed = 1.0;
	if (location == 'w')
		mag_speed = 0.75;

	//update target velocity
	int sign = turned ? -1 : 1;
	switch (status)
	{
	case eStatus::Walking:
		v_target.x = sign * speed_walk * mag_speed;
		break;
	case eStatus::Dashing:
		v_target.x = sign * speed_dash * mag_speed;
		break;
	}

	if (!timer_stun.moving())
	{
		//walk
		bool rd = Controller().Right().isDown();
		if (rd)
		{
			turned = false;
			if (v_target.x < speed_walk)
			{
				status = eStatus::Walking;
				v_target.x = speed_walk * mag_speed;
			}
		}
		else if (Controller().Left().isDown())
		{
			turned = true;
			//phys.v.x = -1.5;

			if (v_target.x > -speed_walk)
			{
				status = eStatus::Walking;
				v_target.x = -speed_walk * mag_speed;
			}
		}
		else
		{
			v_target.x = 0.0;
		}

		//dash
		if (Controller().Right().pushed())
		{
			if (timer_dash.moving())
			{
				status = eStatus::Dashing;
				v_target.x = speed_dash * mag_speed;
			}
		}
		else if (Controller().Left().pushed())
		{
			if (timer_dash.moving())
			{
				status = eStatus::Dashing;
				v_target.x = -speed_dash * mag_speed;
			}
		}

		if (Controller().Left().pushed() || Controller().Right().pushed())
			timer_dash.reset(15);

		//attack
		if (Controller().Z().pushed())
		{
			actz(phys, data);
		}
		if (Controller().X().pushed())
		{
			actx(phys, data);
		}
	}

	//sit walk
	if(status == Sitting)
	{
		//walk
		bool rd = Controller().Right().isDown();
		if (rd)
		{
			turned = false;
			if (v_target.x < speed_walk)
			{
				v_target.x = speed_walk * 0.20;
			}
		}
		else if (Controller().Left().isDown())
		{
			turned = true;
			//phys.v.x = -1.5;

			if (v_target.x > -speed_walk)
			{
				v_target.x = -speed_walk * 0.20;
			}
		}
		else
		{
			v_target.x = 0.0;
		}
	}

	direction_spell = $V(1 - turned * 2, 0);

	//drive and stop
	if (v_target.x > 0)
	{
		phys.v.x += accel;
		if (phys.v.x >= v_target.x)
			phys.v.x = v_target.x;
	}
	else if (v_target.x < 0)
	{
		phys.v.x -= accel;
		if (phys.v.x <= v_target.x)
			phys.v.x = v_target.x;
	}
	else
	{
		if (phys.v.x > 0)
		{
			phys.v.x -= decel;
			if (phys.v.x < 0)
				phys.v.x = 0;
		}
		else if (phys.v.x < 0)
		{
			phys.v.x += decel;
			if (phys.v.x > 0)
				phys.v.x = 0;
		}
	}

	this->pick_item = false;
	if (timer_stun.rest() <= 1)
	{

		//jump
		if (Controller().Up().pushed())
		{
			//phys.v.x = -1.5;
			//if (isOnSomething)
			if (num_jump < max_num_jump)
			{
				this->mp -= jump_cost_mp;
				if (this->mp < 0)
					this->mp = 0;
				this->hp -= jump_cost_hp;
				if (this->hp < 0)
					this->hp = 0;
				if(location != 'w')
					phys.v.y = -speed_jump;
				else
					phys.v.y = (-speed_jump / 2 - 0.5); // add -2 to ease landing from water
				timer_ascend.reset(8);
				if(location != 'w') // in water
					num_jump++;
				$P.sound_stack.push_back($P.snd_jumping);

			}

		}

		//sit
		if (Controller().Down().isDown())
		{
			if (!on_item)
			{
				if (isOnSomething)
				{
					const int st = 12;
					//animation.reset(refx + width * 16, refy, st, 1, width, height);
					status = eStatus::Sitting;
					timer_stun.reset(st);
					v_target.x = 0.0;
				}
			}
			else
			{
				//pick_item = true;
				timer_stun.reset(15);
			}
		}
		else
		{
			status = eStatus::Standing;
		}

		if (Controller().Down().pushed()) 
		{
			pick_item = true;
			timer_stun.reset(15);
		}
		else 
		{
			pick_item = false;
		}

	}
	this->on_item = false;
}

void Character::stamping()
{
	Mover::stamping();
	if(!timer_ascend.moving())
		num_jump = 0;
}

void Character::enterDoor()
{
	animation.reset(19 * width + refx, refy, 60, 1, 32, 64);
}

void Character::guts(int period)
{
	animation.reset(18 * width, 0, 45, 1, 32, 64);
	timer_stun.reset(period);
	v_target.x = 0.0;
	phys.v.x = 0.0;
	status = eStatus::Guts;

}


void Character::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	if (!visible)
		return;
	int num_enchants = 0;
	Enchant* e = (Enchant*)this->enchants.Dst(*tl);
	while (e)
	{
		num_enchants++;
		e = e->getNext();
	}
	bool t = turned;
	if (status == Guts)
		t = false;
	int alpha = timer_invinc.moving() ? 0xaaffffff : 0xffffffff;

	//if (1)
	if (timer_fool > 0)
		graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y() - 32 + 10 + margin_top, 0
		, REFPOINT_FLOWER_X, REFPOINT_FLOWER_Y, 32, 32, t, alpha);

	int pg_offset_x = (2 * t - 1) * 10;
	int pg_offset_y = 16 * int(status == Sitting);
	if (piggyback)
	{
		graphics->draw($P.img_obj,
			phys.p.x + pg_offset_x - data.camera.X(),
			phys.p.y + pg_offset_y - data.camera.Y(),
			0
			, REFPOINT_PIGGYBACK_X, REFPOINT_PIGGYBACK_Y
			, 32, 64, t, alpha);
	}

	// special process for fallen
	int refx_offset = 0;
	if (status == eStatus::Fallen && animation.Frame() == 1)
	{
		if (!isOnSomething)
			refx_offset -= 64;
		else 
		{
			if(animation.finished || animation.Duration() - animation.counter < 30)
				refx_offset += 64;
		}
	}
	if (status == eStatus::Fallen && animation.Frame() == 2)
	{
		if (!isOnSomething)
			refx_offset -= 128;
	}

	int x_offset = (animation.w - this->Width()) * t;
	if(timer_shiver.moving())
		x_offset += ((timer_shiver.rest() / 2) % 2) * 1;
	graphics->draw($P.img_obj, phys.p.x - x_offset - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, animation.RefX() + refx_offset, animation.RefY()
		,animation.w, animation.h, t, alpha);

	if (piggyback)
	{
		graphics->draw($P.img_obj,
			phys.p.x + pg_offset_x - data.camera.X(),
			phys.p.y + pg_offset_y - data.camera.Y(),
			0
			, REFPOINT_PIGGYBACK_X + 32, REFPOINT_PIGGYBACK_Y
			, 32, 64, t, alpha);
	}

	if (Constants::debugging)
	{
		graphics->drawString(gfw::num2str(this->hp), phys.p.x - data.camera.X(), phys.p.y - data.camera.Y() + height, 48, 20);
		graphics->drawString(gfw::num2str(this->score), phys.p.x - data.camera.X(), phys.p.y - data.camera.Y() + height + 20, 48, 20);
		graphics->drawString(gfw::num2str(num_enchants), phys.p.x - data.camera.X(), phys.p.y - data.camera.Y() + height + 40, 48, 20);
	}
}

void Character::die(jnr::JumpnRunData& data)
{
	alive = false;
	Vector2D center = phys.p + Vector2D(width / 2 - 16, height / 2 - 16);
	int offset_refx = 32 * ($P.rand.get() % 4);
	switch (this->dieeffect)
	{
	case eDieEffectType::Expl:
		generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_X, REFPOINT_EXPLOSION_Y, 32, 32
			, center, 10, Vector2D(0, 0), 0, false, 0.25, 0.25, 8);
		generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32
			, center, 4, Vector2D(0, 0), 0);
		generate<Effect>(*tl)->initialize(refx, refy, width, height
			, phys.p, 4, Vector2D(0, 0), 0, Turned(), 1.0, 0, 16);
		generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X + offset_refx, REFPOINT_STAR1_Y, 32, 32, Center() - Vector2D(16, 16)
			, 12, Vector2D(0, 0), 0, 6, 4.0);
		break;
	case eDieEffectType::Hit:
		generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32,
			center, 2, $V(0.0, 0.0), 0);
		generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32,
			center, 4, $V(0.0, 0.0), 0);
		break;
	case eDieEffectType::None:
		break;
	}
	//generate<DyingBoss>(*tl)->initialize(refx, refy, width, height
	//	, phys.p, 4, Turned());

}