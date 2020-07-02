#pragma once
#include"jumpnrun/mover/character/Character.h"

namespace jnr
{
	class BarrierMan : public Character
	{
	public:
		BarrierMan(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, gmtr::Vector2D v, OWNERHANDLE owner);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		virtual int damage(int power, int stun_time, int invinc_time);
		virtual int damage(const Damage& d);
		virtual const gfw::VirtualController& Controller();
		void interact(jnr::Character& chara, jnr::JumpnRunData& data);
	protected:
		gfw::ManualController mc;
		int lifetime;
	};
};