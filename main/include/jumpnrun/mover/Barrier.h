#pragma once
#include"jumpnrun/mover/character/Character.h"
namespace jnr
{
	class Barrier: public Mover
	{
	public:
		Barrier(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, Character* chara);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		virtual int damage(int power, int stun_time, int invinc_time);
		virtual int damage(const Damage& d);
		virtual void die(jnr::JumpnRunData& data);
		virtual void cancel();

	protected:
		int lifetime;
		Object::Pipe pipe;
	};

	class HealBarrier : public Barrier
	{
	public:
		HealBarrier(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, Character* chara);
		virtual void run(jnr::JumpnRunData& data);
	};

	class BubbleBarrier : public Barrier
	{
	public:
		BubbleBarrier() {}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, Character* chara);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		virtual int damage(int power, int stun_time, int invinc_time);
		virtual int damage(const Damage& d);
	};

	class Wing : public Barrier
	{
	public:
		Wing(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, Character* chara);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		virtual int damage(int power, int stun_time, int invinc_time);
		virtual int damage(const Damage& d);
		virtual void die(jnr::JumpnRunData& data);
		virtual void cancel();

		int max_num_jump_ori;
		int jump_const_mp_ori;
	};

	class WillowispPerpetual : public Barrier
	{
	public:
		WillowispPerpetual() {}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, Character* chara, int num_fire);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		virtual int damage(int power, int stun_time, int invinc_time);
		virtual int damage(const Damage& d);
		virtual void die(jnr::JumpnRunData& data);
		virtual void cancel();

		const static int MAX_NUM_PIPE = 10;
		gfw::Object::Pipe pipes[10];
		int pipe_timers[MAX_NUM_PIPE];
		int num_fire;
	};
};