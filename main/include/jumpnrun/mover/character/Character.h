#pragma once
#include"jumpnrun/mover/Mover.h"
#include"framework/Animation.h"
#include"framework/GameUtil.h"
#include"framework/VirtualController.h"
#include"jumpnrun/spell/Spell.h"
#include"jumpnrun/spell/Deck.h"
#include"jumpnrun/system/JumpnRunData.h"
#include<tuple>
#include<math.h>
#include<algorithm>
//#include"AI.h"
namespace jnr
{
	namespace ai
	{
		class AI;
	}
	class Barrier;
	class Enchant;

	class Damage
	{
	public:
		Damage() : 
			ignoring_barrier(false),
			isPhysical(false), 
			ignoring_invinc(false),
			owner(-1),
			smash(0, 0),
			shiver_time(0)
		{};
		int power, stun_time, shiver_time, invinc_time;
		bool ignoring_barrier;
		bool isPhysical;
		bool ignoring_invinc;
		OWNERHANDLE owner;
		gmtr::Vector2D smash;
	};

	//class Mover;
	class Character : public Mover
	{
	public:
		enum eStatus { Standing, Walking, Dashing, Attacking, 
			Casting, Damaged, Fallen, Flying, Sitting, Guts };
		enum eDieEffectType{Expl, Hit, None};
		Character(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner);
		const gmtr::Geometry& Shape(){ return rect; }
		const gmtr::Rectangle& Rect();
		gmtr::Vector2D Center(){ return gmtr::Vector2D(phys.p.x + width / 2, phys.p.y + height / 2); }
		//gmtr::Vector2D Connector(){ return gmtr::Vector2D(phys.p.x + width / 2, phys.p.y + height / 2); }
		virtual void run(jnr::JumpnRunData& data);
		virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
		virtual void actz(phsc::Physics& phys, jnr::JumpnRunData& data){}
		virtual void actx(phsc::Physics& phys, jnr::JumpnRunData& data){}
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		virtual int damage(int power, int stun_time, int invinc_time);
		virtual int damage(const Damage& d);
		virtual void die(jnr::JumpnRunData& data);
		virtual void stamping();
		virtual void setInvincPhys(int period){ timer_invinc_physical.reset(period); }
		virtual const gfw::VirtualController& Controller() = 0;
		virtual spl::SpellMessage spellMessage(){ return spl::SpellMessage(); }
		void setBarrier(Barrier* b);
		Barrier* getBarrier();
		void setEnchant(Enchant* e);
		Enchant* getTopEnchant();
		void changeTopEnchant(Enchant* e);
		virtual void guts(int period = 45);
		virtual void fool(){ timer_fool = 90; }
		virtual void resetDeck() {}
		virtual void flushHand(){}
		virtual void changeCurrentCard(int card_id){}
		virtual void recycleCard(jnr::JumpnRunData& data){}

		int& HP(){ return hp; }
		int& MP(){ return mp; }
		int& HPMax(){ return hp_max; }
		int& MPMax(){ return mp_max; }
		void resetHP_HPMax(int i){ hp = hp_max = i; }
		int& Score(){ return score; }
		bool spendMP(int cost){ if (mp >= cost) { mp -= cost; return true; } return false; }
		int MPInterval(){ return mp_interval; }
		void MPInterval(int i){ mp_interval = i; }
		int Width(){ return width; }
		int Height(){ return height; }
		double SpeedDash(){ return speed_dash; }
		void SpeedDash(double d){ speed_dash = d; }
		double SpeedWalk(){ return speed_walk; }
		void SpeedWalk(double d){ speed_walk = d; }
		double SpeedJump() { return speed_jump; }
		void SpeedJump(double d) { speed_jump = d; }
		int Muscle(){ return muscle; }
		void Muscle(double d){ muscle = d; }
		int Concentration() { return concentration; }
		void Concentration(double d) { concentration = d; }
		float MPHPRate() { return mp_hp_rate; }
		void MPHPRate(double f) { mp_hp_rate = f; }
		gmtr::Vector2D DirectionSpell() { return direction_spell; }

		eStatus Status(){ return status; }
		void onItem(){ on_item = true; }
		bool pickingItem(){ return pick_item; }
		void pickingItem(bool b){ pick_item = b; }
		int MaxNumJump(){ return max_num_jump; }
		void MaxNumJump(int i) { max_num_jump = i; }
		int JumpCostMP() { return jump_cost_mp; }
		void JumpCostMP(int i) { jump_cost_mp = i; }
		int JumpCostHP() { return jump_cost_hp; }
		void JumpCostHP(int i) { jump_cost_hp = i; }
		void JumpHighestY(float y) { jump_highest_y = y; }
		void CircleRef(int x, int y){ circle_refx = x;  circle_refy = y; }
		int CircleRefX(){ return circle_refx; }
		int CircleRefY(){ return circle_refy; }
		bool onSomething(){ return isOnSomething; }
		virtual std::tuple<int, int> MyCircleFrame(){ return std::tuple<int, int > (0, 0); }
		bool interactingWithItems(){ return interacting_with_items; }
		void enterDoor();
		bool isWet(){ return wet; }
		bool isStunned() { return timer_stun.moving(); }
		void piggyBack(bool b) { this->piggyback = b; }

	protected:
		//gmtr::Rectangle rect;
		int hp, mp, hp_max, mp_max, score;
		int mp_interval, mp_gain;
		jnr::Animation animation;
		gmtr::Vector2D v_target;
		gmtr::Vector2D direction_spell;
		eStatus status;
		float speed_walk, speed_dash, speed_jump, accel, decel;
		float jump_highest_y;
		int muscle, // + physical power
			concentration, // + mp increase speed
			th_damage;
		float mp_hp_rate;
		gfw::Timer timer_stun, timer_shiver, timer_invinc, timer_dash, timer_ascend, timer_invinc_physical;
		gfw::Object::Pipe barrier;
		gfw::Object::Pipe enchants;
		int num_jump, max_num_jump;
		int jump_cost_hp, jump_cost_mp;
		int spell_index;
		bool climb_steps, on_item , pick_item
			, interacting_with_items, entering_door, enchantable
			, wet, breathing;
		bool fallen_enabled;
		int circle_refx, circle_refy;
		eDieEffectType dieeffect;
		int timer_fool;
		bool piggyback;
		SoundHandler snd_damaged;
	};

	class Player : public Character
	{
	public:
		Player(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, int player_id);
		virtual void run(jnr::JumpnRunData& data);
		virtual void die(jnr::JumpnRunData& data);
		virtual void actz(phsc::Physics& phys, jnr::JumpnRunData& data);
		virtual void actx(phsc::Physics& phys, jnr::JumpnRunData& data);
		virtual const gfw::VirtualController& Controller();
		virtual void resetDeck();
		virtual void flushHand();
		virtual void changeCurrentCard(int card_id);
		virtual void recycleCard(jnr::JumpnRunData& data);

		void setXSpell(int i){ this->spell_index = i; }
		enum eGameMessage{ ChangeStage, NoMessage };
		eGameMessage getGameMessage();
		void PlayerIndex(int i){ player_index = i; }
		void UsingCardFromDeck(bool using_card_from_deck){ this->using_card_from_deck = using_card_from_deck; }
		std::tuple<int, int> MyCircleFrame(){ return std::tuple<int, int >(0, 0); }
	protected:
		eGameMessage gamemes;
		gfw::Timer timer_deckreset;
		int player_index;
		bool using_card_from_deck;
		bool resettingDeck, flushing_hand;
		int change_current_card_id;
		//int hancards[5];
	};

	class AIPlayer: public Player
	{
	public:
		AIPlayer(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, bool turned, ai::AI* ai, int hp = 50);
		virtual void run(jnr::JumpnRunData& data);
		virtual const gfw::VirtualController& Controller();
		virtual void actz(phsc::Physics& phys, jnr::JumpnRunData& data);
		virtual void actx(phsc::Physics& phys, jnr::JumpnRunData& data);
		void setXSpellSet(const std::vector<jnr::spl::Spell>& v);
		void setZSpellSet(const std::vector<jnr::spl::Spell>& v);
		void changeXSpell(int i){ this->current_x_spell = i; }
		void changeZSpell(int i){ this->current_z_spell = i; }
		bool use_spellset;
	private:
		jnr::spl::Spell x_spells[5], z_spells[5];
		int current_x_spell, current_z_spell;
		ai::AI* ai;
	};

	class HangedKyo: public Character
	{
	public:
		HangedKyo(){}
		virtual void initialize(int refx, int refy, gmtr::Vector2D p);
		virtual void run(jnr::JumpnRunData& data);
		virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
		virtual int damage(int power, int stun_time, int invinc_time);
		virtual int damage(const Damage& d);
		virtual void die(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		virtual const gfw::VirtualController& Controller(){ return mc; }
	private:
		int timer_damaged;
		gfw::ManualController mc;
	};
};