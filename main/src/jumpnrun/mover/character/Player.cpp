#include"jumpnrun/mover/character/Character.h"
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

RegisterMoverClass(Player);

void Player::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, int player_id)
{
	Character::initialize(refx, refy, p, owner);
	timer_deckreset.reset(-1);
	writeName("character_player", name);

	speed_jump = 6.00 - 0.125;

	this->gamemes = eGameMessage::NoMessage;
	player_index = 0;
	using_card_from_deck = true;
	go_out_of_camera = false;
	resettingDeck = false;
	change_current_card_id = -1;
	this->status = eStatus::Standing;
	isOnSomething = 10;
	phys.v.y = 0.1;
	this->player_index = player_index;
	snd_damaged = $P.snd_mydamaged;
	circle_refx = REFPOINT_CIRCLE_FRAME_BLUE_X;
	circle_refy = REFPOINT_CIRCLE_FRAME_BLUE_Y;

}

const gfw::VirtualController& Player::Controller()
{
	//return $P.replaycont;
	return *$P.players[player_index].vc;
}

void Player::resetDeck()
{
	resettingDeck = true;
	//data.players[player_index].deck = $P.players[player_index].deck;
	//data.players[player_index].deck.reset();
	//$P.players[player_index].deck.reset();
}

void Player::flushHand()
{
	flushing_hand = true;
}

void Player::die(jnr::JumpnRunData& data)
{
	if (Owner() == OWNERHANDLE(0))
	{
		data.num_player_beaten++;
	}
	alive = false;
	if (data.encount_boss == 7 && this->Owner() != OWNERHANDLE(0)) //kyo2
	{
		generate<DyingBossFlame>(*tl)->initialize(refx + width * 5, refy, width, height
			, phys.p, 0, turned);
	}
	else
	{
		generate<DyingBoss>(*tl)->initialize(refx + width * 5, refy, width, height
			, phys.p, 0, turned);

		if(this->piggyback)
		{
			int dx = -(1 - turned * 2) * 18;
			generate<Effect>(*tl)->initialize(32 * 14, REFPOINT_BOSS1_Y, 64, 64
				, phys.p + gmtr::Vector2D(dx - 16, -12), 60 * 20
				, gmtr::Vector2D(0.0, 0.0), 1.5, turned, 1, 0, 0, 16);
		}
	}
}

void Player::run(jnr::JumpnRunData& data)
{
	Character::run(data);
	if (resettingDeck)
	{
		resettingDeck = false;
		$P.players[player_index].deck.reset();
		$P.players[player_index].deck_tmp = $P.players[player_index].deck;
		data.players[player_index].deck = $P.players[player_index].deck_tmp;
		//data.players[player_index].deck_tmp = $P.players[player_index].deck_tmp;
		data.players[player_index].deck_tmp = data.players[player_index].deck;
	}

	if (flushing_hand)
	{
		flushing_hand = false;
		Deck& deck = data.players[player_index].deck;
		Deck& deck_tmp = data.players[player_index].deck_tmp;
		for (int i = 0; i < deck.NumHandCard(); i++)
		{
			spell_index = i;
			Card& card = deck_tmp.getCard(spell_index);
			if (card.used)
				return;
			assert(card.value < spl::Spell::spells.size());
			deck_tmp.use(spell_index);
		}
	}


	if (change_current_card_id >= 0)
	{
		auto& deck = data.players[player_index].deck_tmp;
		Card* card = nullptr;
		card = &deck.getCard(deck.current_card);
		if (card->value < 0)
			deck.addNumRest();
		card->value = change_current_card_id;
		card->spell = spl::Spell::spells[change_current_card_id];
		card->used = false;
		deck.setDrawCounter(deck.current_card, 0);

		change_current_card_id = -1;
	}

	if (status == eStatus::Standing || status == eStatus::Walking 
		|| status == eStatus::Dashing || status == Flying)
	{
		if (animation.finished)
		{
			if (fabs(phys.v.x) > 0.0)
			{
				if (fabs(v_target.x) == 0.0)
				{
					animation.reset(refx + width * 20, refy, 12, 1, width, height);
					status = eStatus::Walking;
				}
				else if (fabs(v_target.x) <= speed_walk)
				{
					animation.reset(refx + width, refy, 12, 2, width, height);
					status = eStatus::Walking;
				}
				else
				{
					status = eStatus::Dashing;
					animation.reset(refx + width, refy, 10, 2, width, height);
				}
			}
			else
			{
				animation.reset(refx, refy, 2, 1, width, height);
				status = eStatus::Standing;
			}
		}
		if (!isOnSomething)
		{
			int offset_x = phys.v.y >= 0.4 ? 4 : 3;
			animation.reset(refx + offset_x * width, refy, 2, 1, width, height);
			status = eStatus::Flying;
		}
	}
	else if (status == Sitting)
	{
		if (animation.finished)
		{
			if(fabs(v_target.x) != 0.0)
				animation.reset(refx + width * 21, refy, 10, 2, width, height);
			else
			{
				animation.reset(refx + width * 21, refy, 12, 1, width, height);
			}
		}
	}
	else
	{
		if (animation.finished)
		{
			if (!isOnSomething)
			{
				animation.reset(refx + 3 * width, refy, 2, 1, width, height);
				status = eStatus::Flying;
			}
			else
			{
				animation.reset(refx, refy, 2, 1, width, height);
				status = eStatus::Standing;
			}
		}
	}

	//this->piggyback = $P.getFlag("pgbck");

	//interaction with stg
	auto& c = stg->ChipAt(this->Center());
	//signboard and poster
	if (c.data.size() > 0) {
		data.message = c.data;
		data.message_time = 90;
	}


	jnr::ai::CharacterStatus cs;
	cs.age = this->age;
	cs.hp = this->HP();
	cs.stg = this->stg;
	cs.turned = this->Turned();
	cs.isOnSomething = this->isOnSomething;
	cs.status = (ai::eStatus)this->status;
	cs.p = this->phys.p + gmtr::Vector2D(width / 2, height / 2);
	cs.v = this->phys.v;
	cs.tl = tl;
	cs.pipe = Object::Pipe(this, *tl);
	cs.data = &data;
	//$P.aisuperviser.push(cs, Controller());
}

void Player::actz(phsc::Physics& phys, jnr::JumpnRunData& data)
{
	//spl::bullet(*this);
	status = eStatus::Attacking;
	animation.reset(refx + width * 8, refy, 14, 2, width, height, 12);
	animation.setSpell(spl::eSpell::spl_punch, 12);
	//animation.setSpell(spl::eSpell::spl_willowispperp3, 12);
	timer_stun.reset(36);
	//animation.setSpell(spl::eSpell::spl_speedup , 15);
	v_target = $V(0, 0);
	//$P.sound_stack.push_back($P.snd_exp);
}

void Player::actx(phsc::Physics& phys, jnr::JumpnRunData& data)
{
	//generate<Circle>(*tl)->initialize(REFPOINT_CIRCLE_HEPTAGRAMMA_X, REFPOINT_CIRCLE_HEPTAGRAMMA_Y
	//	, REFPOINT_CIRCLE_FRAME_BLUE_X, REFPOINT_CIRCLE_FRAME_BLUE_Y, this->P(), 180, 160, Vector2D(0, 0), 0.01, 1, 0, 0, 4);

	if (using_card_from_deck)
	{
		Deck& deck = data.players[player_index].deck;
		Deck& deck_tmp = data.players[player_index].deck_tmp;
		spell_index = deck.current_card;

		if (spell_index >= 0)
		{
			Card& card = deck_tmp.getCard(spell_index);
			if (card.used || card.value < 0)
			{
				$P.sound_stack.push_back($P.snd_cancel);
				return;
			}
			//assert(card.value < spl::Spell::spells.size());
			spl::Spell s = spl::Spell::spells[card.value];
			bool paid = (this->mp >= s.manacost) || this->mp_hp_rate > 0.0;
			if (paid)
			{
				if (this->mp < s.manacost) // pay life
					this->hp -= (s.manacost - this->mp) * mp_hp_rate;
				this->mp -= s.manacost;
				//deck.use(spell_index);
				deck_tmp.use(spell_index);
				status = eStatus::Casting;
				animation.reset(refx + width * s.anim_type, refy, s.frame_of_one_phase, s.anim_num
					, width, height, s.final_frame_time, s.num_iteration);
				animation.setSpell(s.func_id, s.actiontime);
				timer_stun.reset(s.stuntime);
				v_target = $V(0, 0);
			}
			else
			{
				//fool();
				status = eStatus::Attacking;
				animation.reset(refx + width * 8, refy, 14, 2, width, height, 12);
				animation.setSpell(spl::spl_flower, 19);
				timer_stun.reset(36);
				v_target = $V(0, 0);
				$P.sound_stack.push_back($P.snd_cancel);
			}
		}
	}
	else
	{
		if (spell_index >= 0)
		{
			spl::Spell s = spl::Spell::spells[spell_index];
			if (this->mp >= s.manacost)
			{
				this->mp -= s.manacost;
				status = eStatus::Casting;
				animation.reset(refx + width * s.anim_type, refy, s.frame_of_one_phase, s.anim_num
					, width, height, s.final_frame_time, s.num_iteration);
				animation.setSpell(s.func_id, s.actiontime);
				//animation.setSpell(spl::eSpell::spl_bullet, 15);
				timer_stun.reset(s.stuntime);
				v_target = $V(0, 0);
			}
		}
	}
}

void Player::changeCurrentCard(int card_id)
{
	change_current_card_id = card_id;
}

void Player::recycleCard(jnr::JumpnRunData& data)
{
	Deck& deck = data.players[player_index].deck;
	Deck& deck_tmp = data.players[player_index].deck_tmp;
	Card c = deck_tmp.getCard(deck_tmp.numCards() - 1);
	if (c.used && c.value >= 0)
		change_current_card_id = c.value;
}