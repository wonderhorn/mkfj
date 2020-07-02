#pragma once
#include"jumpnrun/mover/character/Character.h"

namespace jnr
{
	class Bubble: public Character
	{
	public:
		Bubble(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, gmtr::Vector2D v, int power, OWNERHANDLE owner);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		virtual int damage(int power, int stun_time, int invinc_time);
		virtual int damage(const Damage& d);
		virtual void die(jnr::JumpnRunData& data);
		virtual const gfw::VirtualController& Controller();
		void interact(jnr::Character& chara, jnr::JumpnRunData& data);
	protected:
		int power;
		int lifetime;
		bool is_damaging;
		gfw::ManualController mc;
	};

	class Willowisp : public Bubble
	{
		public:
		Willowisp() {}
		virtual void initialize(int refx, int refy, jnr::Character& chara
			, gmtr::Vector2D p, gmtr::Vector2D v, int power, OWNERHANDLE owner
			, double phase_offset);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);

	protected:
		gfw::Object::Pipe pipe;
		double phase_offset;
	};

	class Ball : public Bubble
	{
	public:
		Ball() {}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, gmtr::Vector2D v, int power, OWNERHANDLE owner);
		virtual void run(jnr::JumpnRunData& data);
		virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
		void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual int damage(const Damage& d);

	protected:
	};
};