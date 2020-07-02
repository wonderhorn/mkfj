#pragma once
#include"jumpnrun/mover/character/Character.h"

namespace jnr
{
	class SwitchReciever: public Character
	{
	public:
		virtual void receive(bool signal) = 0;
	};

	class Door : public SwitchReciever
	{
	public:
		Door(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		virtual int damage(int power, int stun_time, int invinc_time);
		virtual int damage(const Damage& d);
		virtual const gfw::VirtualController& Controller();
		virtual gmtr::Vector2D react(const gmtr::Vector2D& v_prev, const gmtr::Vector2D& v_next, bool up);
		void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void receive(bool signal);
		char flag_id[JNR_FLAG_SIZE];
	protected:
		bool isClosed;
		gfw::ManualController mc;
	};

	class Switch : public Character
	{
	public:
		Switch(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, double mass = 0.0);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		virtual int damage(int power, int stun_time, int invinc_time);
		virtual int damage(const Damage& d);
		virtual const gfw::VirtualController& Controller();
		void setReciever(gfw::Object* obj){ pipe = gfw::Object::Pipe(obj, *tl); }
		void setSwitchType(bool toggle, int time_up){ this->toggle = toggle;  timer = this->time_up = time_up; }
		void setOn(bool b);
		//void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		char flag_id[JNR_FLAG_SIZE];
	protected:
		gfw::Object::Pipe pipe;
		gfw::ManualController mc;
		bool toggle;
		int time_up, timer;
		bool isOn, isOn_prev;
	};
};