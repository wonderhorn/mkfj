#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/system/Parmanent.h"
#include<iostream>
using namespace jnr;

RegisterMoverClass(AIPlayer);

void AIPlayer::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, bool turned, ai::AI* ai, int hp)
{
	Player::initialize(refx, refy, p, owner, 1);
	gfw::writeName("character_aiplayer", name);

	this->ai = ai;
	this->turned = turned;
	hp_max = this->hp = hp;

	current_x_spell = current_z_spell = 0;
	z_spells[0] = { 15, 8, 2, 60, 25, 35, 1, 0, spl::spl_laserslash, 32 * 11, 32, 'u', -1, 1, "laserslash" };
	z_spells[1] = { 15, 8, 2, 60, 25, 35, 1, 0, spl::spl_laserslash, 32 * 11, 32, 'u', -1, 1, "laserslash" };
	z_spells[2] = { 15, 8, 2, 60, 25, 35, 1, 0, spl::spl_laserslash, 32 * 11, 32, 'u', -1, 1, "laserslash" };
	z_spells[3] = { 15, 8, 2, 60, 25, 35, 1, 0, spl::spl_laserslash, 32 * 11, 32, 'u', -1, 1, "laserslash" };
	z_spells[4] = { 15, 8, 2, 60, 25, 35, 1, 0, spl::spl_laserslash, 32 * 11, 32, 'u', -1, 1, "laserslash" };

	x_spells[0] = { 10, 17, 1, 30, 20, 15, 1, 0, spl::spl_barrier, 32 * 6, 0, 'u', -1, 0, "barrier" };
	x_spells[1] = { 10, 17, 1, 30, 20, 15, 1, 0, spl::spl_barrier, 32 * 6, 0, 'u', -1, 0, "barrier" };
	x_spells[2] = { 10, 17, 1, 30, 20, 15, 1, 0, spl::spl_barrier, 32 * 6, 0, 'u', -1, 0, "barrier" };
	x_spells[3] = { 10, 17, 1, 30, 20, 15, 1, 0, spl::spl_barrier, 32 * 6, 0, 'u', -1, 0, "barrier" };
	x_spells[4] = { 10, 17, 1, 30, 20, 15, 1, 0, spl::spl_barrier, 32 * 6, 0, 'u', -1, 0, "barrier" };
	use_spellset = true;
}

void AIPlayer::run(jnr::JumpnRunData& data)
{
	Player::run(data);

	jnr::ai::CharacterStatus cs;
	int numn_of_friends = 0;
	for (auto t = tl->begin(); t != tl->end(); t++)
	{
		Object* obj = getObject(t);
		if (!obj->isMover)
			continue;
		Mover* mvr = static_cast<Mover*>(obj);
		if (mvr->ID() != this->ID())
		{
			if (
				mvr->alive
				&& mvr->Owner() == this->Owner()
				&& mvr->Name().substr(0, 9) == "character"
				)
				numn_of_friends++;
		}
	}

	cs.id = this->ID();
	cs.age = this->age;
	cs.score = this->score;
	cs.hp = this->HP();
	cs.mp = this->MP();
	cs.num_of_friends = numn_of_friends;
	cs.location = this->location;
	cs.stg = this->stg;
	cs.turned = this->Turned();
	cs.isOnSomething = this->isOnSomething;
	cs.status = (ai::eStatus)this->status;
	cs.p = this->phys.p + gmtr::Vector2D(width / 2, height / 2);
	cs.v = this->phys.v;
	//std::cout << this->v_reaction.x << std::endl;
	cs.v_reaction = this->v_reaction;
	cs.tl = tl;
	cs.pipe = Object::Pipe(this, *tl);
	cs.owner = this->owner;
	cs.data = &data;

	ai->think(cs);
	ai->Controller().poll();
	this->current_x_spell = ai->CurrentXSpell();
	this->current_z_spell = ai->CurrentZSpell();
}

const gfw::VirtualController& AIPlayer::Controller()
{
	return ai->Controller();
}

void AIPlayer::actz(phsc::Physics& phys, jnr::JumpnRunData& data)
{
	spl::Spell s = z_spells[current_z_spell];
	//if (this->mp >= s.manacost)
	{
		this->mp -= s.manacost;
		if (this->mp < 0)
			this->mp = 0;
		status = eStatus::Casting;
		animation.reset(refx + width * s.anim_type, refy, s.frame_of_one_phase, s.anim_num
			, width, height, s.final_frame_time, s.num_iteration);
		animation.setSpell(s.func_id, s.actiontime);
		//animation.setSpell(spl::eSpell::spl_bullet, 15);
		timer_stun.reset(s.stuntime);
		v_target = gmtr::Vector2D(0, 0);
	}
}
void AIPlayer::actx(phsc::Physics& phys, jnr::JumpnRunData& data)
{
	if (use_spellset)
	{
		spl::Spell s = x_spells[current_x_spell];
		//if (this->mp >= s.manacost)
		{
			this->mp -= s.manacost;
			if (this->mp < 0)
				this->mp = 0;
			status = eStatus::Casting;
			animation.reset(refx + width * s.anim_type, refy, s.frame_of_one_phase, s.anim_num
				, width, height, 0, s.num_iteration);
			animation.setSpell(s.func_id, s.actiontime);
			//animation.setSpell(spl::eSpell::spl_bullet, 15);
			timer_stun.reset(s.stuntime);
			v_target = gmtr::Vector2D(0, 0);
		}
	}
	else
		Player::actx(phys, data);
}
void AIPlayer::setXSpellSet(const std::vector<jnr::spl::Spell>& v)
{
	for (int i = 0; i < 5; i++)
	{
		x_spells[i] = v[i];
	}
}
void AIPlayer::setZSpellSet(const std::vector<jnr::spl::Spell>& v)
{
	for (int i = 0; i < 5; i++)
	{
		z_spells[i] = v[i];
	}
}