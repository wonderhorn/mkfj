#pragma once
#include"jumpnrun/mover/character/Character.h"
namespace jnr
{
	class Enchant : public  Mover
	{
	public:
		Enchant(){}
		virtual void initialize(int rexx, int refy, OWNERHANDLE owner, Character* chara, int lifetime = 30);
		virtual void run(jnr::JumpnRunData& data);
		virtual void effect(jnr::JumpnRunData& data){}
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		virtual void die(jnr::JumpnRunData& data);
		virtual void cancel();
		void setNext(const Object::Pipe& p){ pipe_next = p; }
		void setPrev(const Object::Pipe& p){ pipe_prev = p; }
		Enchant* getNext(){ return static_cast<Enchant*>(pipe_next.Dst(*tl)); }

	protected:
		int lifetime;
		Object::Pipe pipe_chara, pipe_next, pipe_prev;
		gmtr::Vector2D particles[5];
		int timer_particles[5];

	};

	class EnchantSlow : public Enchant
	{
	public:
		virtual void initialize(int rexx, int refy, OWNERHANDLE owner, Character* chara
			, int lifetime, double val);
		virtual void die(jnr::JumpnRunData& data);

		double value;
	};

	class EnchantMuscle : public Enchant
	{
	public:
		virtual void initialize(int rexx, int refy, OWNERHANDLE owner, Character* chara
			, int lifetime, double val);
		virtual void die(jnr::JumpnRunData& data);

		double value;
	};

	class EnchantConcentrate : public Enchant
	{
	public:
		virtual void initialize(int rexx, int refy, OWNERHANDLE owner, Character* chara
			, int lifetime, double val);
		virtual void die(jnr::JumpnRunData& data);

		double value;
	};

	class EnchantMPHPRate : public Enchant
	{
	public:
		virtual void initialize(int rexx, int refy, OWNERHANDLE owner, Character* chara
			, int lifetime, double val);
		virtual void die(jnr::JumpnRunData& data);

		double value;
	};

	class EnchantOwner : public Enchant
	{
	public:
		virtual void initialize(int rexx, int refy, OWNERHANDLE owner, Character* chara
			, int lifetime, OWNERHANDLE owner_new);
		virtual void die(jnr::JumpnRunData& data);

		OWNERHANDLE owner_old, owner_new;
	};

	class EnchantBind : public Enchant
	{
	public:
		virtual void initialize(int rexx, int refy, OWNERHANDLE owner, Character* chara
			, int lifetime);
		void effect(jnr::JumpnRunData& data);
		virtual void die(jnr::JumpnRunData& data);
		void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
	};
}