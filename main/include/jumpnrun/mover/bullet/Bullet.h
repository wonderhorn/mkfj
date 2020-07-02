#pragma once
#include"jumpnrun/mover/Mover.h"
#include"utils/Sprine.h"

namespace jnr
{
	class Bullet: public Mover
	{
	public:
		Bullet(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v, double mass, bool rotate_to_direction = false);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);

		int LifeTime(){ return lifetime; }
		void LifeTime(int t){ lifetime = t; }
		void Radius(int i){ this->radius = i; }
		SoundHandler snd_hit;
		int y_anim_interval, y_anim_num;
	protected:
		int power, lifetime;
		bool rotate_to_direction;
		int num_bounding;
		double bound_rate;
		int radius;
	};

	class ShotGun : public Bullet
	{
	public:
		void run(jnr::JumpnRunData& data);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
	};

	class Explosion: public Bullet
	{
	public:
		enum eEnchant{None, };
		Explosion(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, int power
			, gmtr::Vector2D v = gmtr::Vector2D(0, 0)
			, double mass = 0.0, int lifetime = 12);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
	protected:
		int alpha, timer_dissapear;
		double rate, dr_dt;
	};

	class Gas : public Bullet
	{
	public:
		Gas(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v = gmtr::Vector2D(0, 0), double mass = 0.0);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
	};

	class Tackle : public Bullet
	{
	public:
		virtual void initialize(Character& chara, int power, gmtr::Vector2D v);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		void setParticleRef(int x, int y) { particle_refx = x; particle_refy = y; }

	protected:
		gfw::Object::Pipe pipe;
		int timer_finish;
		int particle_refx, particle_refy;
	};

	class Flame : public Bullet
	{
	public:
		Flame(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v, double mass, int rank = 5);
		//virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);

	protected:
		int rank;
	};
	class Fire : public Bullet
	{
	public:
		Fire(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v, double mass, bool homing = false);
		//virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);

	protected:
		int rank;
		bool homing;
	};


	class Sonicboom : public Bullet
	{
	public:
		Sonicboom(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v, double mass, int rank = 1);
		//virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
	protected:
		int rank;
	};

	class Bomb : public Bullet
	{
	public:
		enum Type { TNone, TExplosion, TGas };
		
		Bomb(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v, double mass
			, bool rotate_to_direction, bool homing = false, bool yanim = true, Type type = TExplosion);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);

	protected:
		bool isLighted;
		bool homing;
		bool yanim;
		Type type;
	};

	class Laser : public Bullet
	{
	public:
		virtual void initialize(int color, double length
			, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v, int num_ref = 0);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
	protected:
		int color;
		double length;
		int num_reflect;
		bool reflected;
		gmtr::Vector2D p_ter;
	};

	class LaserSlash : public Bullet
	{
	public:
		virtual void initialize(int color, double length, double theta_center
			, jnr::Character* chara, int power);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
	protected:
		int color;
		double length;
		double theta_center;
		int num_reflect;
		bool reflected;
		gmtr::Vector2D p_ter;
		gfw::Object::Pipe pipe;
	};

	class Wind : public Bullet
	{
	public:
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v, double mass, bool rotate_to_direction = false);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
	};

	class Thunder : public Bullet
	{
	public:
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v, double mass, bool rotate_to_direction = false);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		//virtual void run();
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
	};

	class Sword : public Bullet
	{
	public:
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v, double mass, bool rotate_to_direction = false);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
	};

	class SwordStream : public Bullet
	{
	public:
		virtual void initialize(jnr::Character& chara, int power, gmtr::Vector2D v, double mass,
			bool rotate_to_direction = false);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data) {}

	private:
		gfw::Object::Pipe pipe;
		int interval, generate_counter;
	};

	class StarStream: public Bullet
	{
	public:
		virtual void initialize(jnr::Character& chara, int power, gmtr::Vector2D v, double mass,
			bool rotate_to_direction = false);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data){}

	private:
		gfw::Object::Pipe pipe;
		int interval, generate_counter;
	};

	class BubbleStream : public Bullet
	{
	public:
		virtual void initialize(jnr::Character& chara, int power, gmtr::Vector2D v, double mass, bool rotate_to_direction = false);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data){}

	private:
		gfw::Object::Pipe pipe;
		int interval, generate_counter;
	};

	class Thunderbolt : public Bullet
	{
	public:
		virtual void initialize(jnr::Character& chara, int power, gmtr::Vector2D v, double mass, bool rotate_to_direction = false);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data){}

	private:
		gfw::Object::Pipe pipe;
		int interval, generate_counter;
	};
	class EmitFire : public Bullet
	{
	public:
		virtual void initialize(gmtr::Vector2D p, gmtr::Vector2D v, OWNERHANDLE o, int power);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data) {}

	private:
		gfw::Object::Pipe pipe;
		int interval, generate_counter;
	};

	class EmitGas : public Bullet
	{
	public:
		virtual void initialize(jnr::Character* chara, OWNERHANDLE owner, int power, gmtr::Vector2D p,
			gmtr::Vector2D v, double mass, bool rotate_to_direction = false);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data){}

	private:
		gfw::Object::Pipe pipe;
		int interval, generate_counter;
	};

	class SlowBall : public Bullet
	{
	public:
		SlowBall(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v, double mass
			, bool rotate_to_direction);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		//virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);

	protected:
		double value;
		int period;
	};

	class DarkWave : public Bullet
	{
	public:
		virtual void initialize(jnr::Character& chara, int power, gmtr::Vector2D v,
			double mass, bool rotate_to_direction = false);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);

	private:
		gfw::Object::Pipe pipe;
		int interval, generate_counter;
	};

	class Whip : public Bullet
	{
	public:
		virtual void initialize(jnr::Character& chara, int power, gmtr::Vector2D v, gmtr::Vector2D connector, double mass);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		void tenseForward();
		void tenseBackward();
		void reelup();

	protected:
		double max_length_seg;
		int max_num_seg;
		int alive_length, timer_dissapear;
		gmtr::Sprine sprine;
		unsigned char r, g, b;
		gfw::Object::Pipe pipe;
		gmtr::Vector2D connector;
	};

	class BigEyeTentacle: public Whip
	{
	public:
		virtual void initialize(jnr::Character& chara, int power, gmtr::Vector2D v, gmtr::Vector2D connector, double mass);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);

	};
	class Chandelier : public Bullet
	{
	public:
		virtual void initialize(gmtr::Vector2D p, int power,
			gmtr::Vector2D v, double mass);
		virtual void run(jnr::JumpnRunData& data);
		void interact(jnr::Character& chara, jnr::JumpnRunData& data);

		//virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);

	};

};