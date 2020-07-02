#pragma once
#include"utils/OwnerHandle.h"
#include"jumpnrun/mover/effect/Effect.h"

namespace jnr
{
	class Circle : public Effect
	{
	public:
		virtual void initialize(int refx, int refy, int refx_color, int refy_color
			, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime, gmtr::Vector2D v, double spin_speed
			, double diverge_start_rate = 1.0, double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		virtual void run(jnr::JumpnRunData& data);
		virtual void actionOnTime(jnr::JumpnRunData& data){}
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		virtual void cancel(){ alive = false; }
		int LeftLifeTime(){ return lifetime - age; }

	protected:
		int refx_color, refy_color;
		double theta, spin_speed;
		int actiontime;
	};

	template <class _Action> class ActionCircle : public Circle
	{
	public:
		void initialize(int refx, int refy, int refx_color, int refy_color
			, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime, gmtr::Vector2D v, double spin_speed
			, const std::string& name, double diverge_start_rate = 1.0
			, double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0)
		{
			Circle::initialize(refx, refy, refx_color, refy_color, p, lifetime, actiontime, v, spin_speed
				, diverge_start_rate, diverge_speed, dissapear_speed, appear_speed);
			writeName(name.c_str(), this->name);
		}
		void run()
		{
			Circle::run();
			if (age >= actiontime)
			{
				actiontime = INT_MAX;
				_Action()(this);
			}
		}
	private:
	};

	class CircleExplosion : public Circle
	{
	public:
		void initialize(int refx, int refy, int refx_color, int refy_color
			, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime, gmtr::Vector2D v, double spin_speed
			, double diverge_start_rate = 1.0, double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		void actionOnTime(jnr::JumpnRunData& data);
	};

	class CircleGas: public Circle
	{
	public:
		void initialize(int refx, int refy, int refx_color, int refy_color
			, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime, gmtr::Vector2D v, double spin_speed
			, double diverge_start_rate = 1.0, double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		void actionOnTime(jnr::JumpnRunData& data);
	};

	class CircleBubble : public Circle
	{
	public:
		void initialize(int refx, int refy, int refx_color, int refy_color
			, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime, gmtr::Vector2D v, double spin_speed
			, double diverge_start_rate = 1.0, double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		void actionOnTime(jnr::JumpnRunData& data);
	};

	class CircleBarrier : public Circle
	{
	public:
		void initialize(int refx, int refy, int refx_color, int refy_color
			, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime, gmtr::Vector2D v, double spin_speed
			, double diverge_start_rate = 1.0, double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		void actionOnTime(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		bool barrier_started;
	};

	class CircleSummonFrog : public Circle
	{
	public:
		void initialize(int refx, int refy, int refx_color, int refy_color
			, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
			, gmtr::Vector2D v, bool turned, double spin_speed
			, double diverge_start_rate = 1.0, double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		void actionOnTime(jnr::JumpnRunData& data);
		bool summon_turned;
	};

	class CircleSummonMushroom : public Circle
	{
	public:
		void initialize(int refx, int refy, int refx_color, int refy_color
			, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
			, gmtr::Vector2D v, bool turned, double spin_speed
			, double diverge_start_rate = 1.0, double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		void actionOnTime(jnr::JumpnRunData& data);
		bool summon_turned;
	};

	class CircleSummonDirtCube : public Circle
	{
	public:
		void initialize(int refx, int refy, int refx_color, int refy_color
			, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
			, gmtr::Vector2D v, bool turned, double spin_speed
			, double diverge_start_rate = 1.0, double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		void actionOnTime(jnr::JumpnRunData& data);
	};

	class CircleSummonBombBox : public Circle
	{
	public:
		void initialize(int refx, int refy, int refx_color, int refy_color
			, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
			, gmtr::Vector2D v, bool turned, double spin_speed
			, double diverge_start_rate = 1.0, double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		void actionOnTime(jnr::JumpnRunData& data);
	};

	class CircleSummonGhost : public Circle
	{
	public:
		void initialize(int refx, int refy, int refx_color, int refy_color
			, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
			, gmtr::Vector2D v, bool turned, double spin_speed
			, double diverge_start_rate = 1.0, double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		void actionOnTime(jnr::JumpnRunData& data);
		bool summon_turned;
	};

	class CircleSummonKnight : public Circle
	{
	public:
		void initialize(int refx, int refy, int refx_color, int refy_color
			, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
			, gmtr::Vector2D v, bool turned, double spin_speed
			, double diverge_start_rate = 1.0, double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		void actionOnTime(jnr::JumpnRunData& data);
		bool summon_turned;
	};

	class CircleSummonCandle : public Circle
	{
	public:
		void initialize(int refx, int refy, int refx_color, int refy_color
			, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
			, gmtr::Vector2D v, bool turned, double spin_speed
			, double diverge_start_rate = 1.0, double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		void actionOnTime(jnr::JumpnRunData& data);
		bool summon_turned;
	};

	class CircleSummonCannon : public Circle
	{
	public:
		void initialize(int refx, int refy, int refx_color, int refy_color
			, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
			, gmtr::Vector2D v, bool turned, double spin_speed
			, double diverge_start_rate = 1.0, double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		void actionOnTime(jnr::JumpnRunData& data);
		bool summon_turned;
	};

	class CircleSummonPumpkin : public Circle
	{
	public:
		void initialize(int refx, int refy, int refx_color, int refy_color
			, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
			, gmtr::Vector2D v, bool turned, double spin_speed
			, double diverge_start_rate = 1.0, double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		void actionOnTime(jnr::JumpnRunData& data);
		bool summon_turned;
	};

	class CircleSummonFairyBlue : public Circle
	{
	public:
		void initialize(int refx, int refy, int refx_color, int refy_color
			, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
			, gmtr::Vector2D v, bool turned, double spin_speed
			, double diverge_start_rate = 1.0, double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		void actionOnTime(jnr::JumpnRunData& data);
		bool summon_turned;
	};

	class CircleSummonLamp : public Circle
	{
	public:
		void initialize(int refx, int refy, int refx_color, int refy_color
			, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
			, gmtr::Vector2D v, bool turned, double spin_speed
			, double diverge_start_rate = 1.0, double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		void actionOnTime(jnr::JumpnRunData& data);
		bool summon_turned;
	};

	class CircleSummonBigSlime : public Circle
	{
	public:
		void initialize(int refx, int refy, int refx_color, int refy_color
			, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
			, gmtr::Vector2D v, bool turned, double spin_speed
			, double diverge_start_rate = 1.0, double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		void actionOnTime(jnr::JumpnRunData& data);
		bool summon_turned;
	};

	class CircleSummonAngel : public Circle
	{
	public:
		void initialize(int refx, int refy, int refx_color, int refy_color
			, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
			, gmtr::Vector2D v, bool turned, double spin_speed
			, double diverge_start_rate = 1.0, double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		void actionOnTime(jnr::JumpnRunData& data);
		bool summon_turned;

		Object::Pipe  pipe;
	};

	class CircleSummonElf : public Circle
	{
	public:
		void initialize(int refx, int refy, int refx_color, int refy_color
			, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
			, gmtr::Vector2D v, bool turned, double spin_speed
			, double diverge_start_rate = 1.0, double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		void actionOnTime(jnr::JumpnRunData& data);
		bool summon_turned;

		Object::Pipe  pipe;
	};

	class CircleSummonManaFlower : public Circle
	{
	public:
		void initialize(int refx, int refy, int refx_color, int refy_color
			, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
			, gmtr::Vector2D v, double spin_speed
			, double diverge_start_rate = 1.0, double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		void actionOnTime(jnr::JumpnRunData& data);
		bool summon_turned;
	};

	class CircleFlame : public Circle
	{
	public:
		void initialize(int refx, int refy, int refx_color, int refy_color
			, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
			, gmtr::Vector2D v, double spin_speed
			, double diverge_start_rate = 1.0, double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		void actionOnTime(jnr::JumpnRunData& data);
	};

	class CircleTrapExplosion : public Circle
	{
	public:
		void initialize(int refx, int refy, int refx_color, int refy_color
			, gmtr::Vector2D p, OWNERHANDLE owner, int lifetime, int actiontime
			, gmtr::Vector2D v, double spin_speed
			, double diverge_start_rate = 1.0, double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		void run(jnr::JumpnRunData& data);
		void actionOnTime(jnr::JumpnRunData& data);
	};
};