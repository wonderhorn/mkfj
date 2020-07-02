#pragma once
#include"jumpnrun/mover/Mover.h"

namespace jnr
{
	class Effect : public Mover
	{
	public:
		Effect(){}
		virtual void initialize(int refx, int refy, int w, int h
			, gmtr::Vector2D p, int lifetime, gmtr::Vector2D v, double mass, bool turned = false
			, double diverge_start_rate = 1.0 , double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);

		int period_onoff;
		int y_anim_num, y_anim_interval;
	protected:
		int lifetime, alpha;
		double diverge_speed;
		int dissapear_speed, appear_speed;
		unsigned int timer_dissapear, timer_appear;
		double rate;
	};

	/*class EffectAnim : public Effect
	{
	public:
		EffectAnim() {}
		virtual void initialize(int refx, int refy, int w, int h
			, gmtr::Vector2D p, int lifetime, gmtr::Vector2D v, double mass, bool turned
			, int drefx, int drefy, int num_frame, int interval
			, double diverge_start_rate = 1.0 , double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);

	private:
		int drefx, drefy, num_frame, interval;
	};*/

	class WaterBubble : public Effect
	{
	public:
		WaterBubble() {}
		virtual void run(jnr::JumpnRunData& data);

	};

	class LightColumns : public Effect
	{
	public:
		LightColumns() {}
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
	private:
		int column_heights[20];
	};

	class EffectOnChara: public Effect
	{
	public:
		virtual void initialize(int refx, int refy, int w, int h
			, gmtr::Vector2D p, Character* chara, int lifetime, gmtr::Vector2D v, double mass, bool turned = false
			, double diverge_start_rate = 1.0, double diverge_speed = 0, int dissapear_speed = 0, int appear_speed = 0);
		virtual void run(jnr::JumpnRunData& data);

	protected:
		Pipe pipe;
		gmtr::Vector2D connector;
	};


	class Particle
	{
	public:
		Particle() {}
		Particle(const gmtr::Vector2D& p, const gmtr::Vector2D& v)
			: p(p), v(v), age(-1), lifetime(15) {}
		gmtr::Vector2D p, v;
		int lifetime, age;
	};

	class Particles : public Effect
	{
	public:
		enum  ParticleType { ParticleRadiation, ParticleSpiral };

		Particles(){}
		virtual void initialize(int refx, int refy, int w, int h
			, gmtr::Vector2D p, int lifetime, gmtr::Vector2D v, double mass
			, int num_particles = max_num_particles, double speed = 4
			, ParticleType type = ParticleRadiation, bool turn=false);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);

	private:
		

		static const int max_num_particles = 12;
		int num_particles;
		bool turned;
		Particle particles[max_num_particles];
		int speed;
		ParticleType type;
	};

	class Flow : public Effect
	{
	public:

		Flow() {}
		virtual void initialize(Character* chara, int refx, int refy, int w, int h
			, gmtr::Vector2D p, int lifetime, double mass
			, int num_particles = max_num_particles);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);

	private:

		static const int max_num_particles = 12;
		int num_particles;
		bool turned;
		Particle particles[max_num_particles];
		int speed;
		Object::Pipe pipe;
	};


	class BossDummy : public Effect
	{
	public:
		virtual void initialize(int refx, int refy, int w, int h
			, gmtr::Vector2D p, double mass, bool turned = false);
		virtual void run(jnr::JumpnRunData& data);
		void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		bool to_turn;
	};

	class DyingBoss : public Effect
	{
	public:
		virtual void initialize(int refx, int refy, int w, int h
			, gmtr::Vector2D p, double mass, bool turned = false);
		virtual void run(jnr::JumpnRunData& data);

	};
	class DyingBossFlame : public Effect
	{
	public:
		virtual void initialize(int refx, int refy, int w, int h
			, gmtr::Vector2D p, double mass, bool turned = false);
		virtual void run(jnr::JumpnRunData& data);

	};

	class BigEyeDummy : public Effect
	{
	public:
		virtual void initialize(int refx, int refy, int w, int h
			, gmtr::Vector2D p, double mass, bool turned = false);
		virtual void run(jnr::JumpnRunData& data);
		void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
	};

	class PrologFujitsubo : public Effect
	{
	public:
		virtual void initialize();
		virtual void run(jnr::JumpnRunData& data);
		void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		bool to_turn;
	};

	class PrologHand : public Effect
	{
	public:
		virtual void initialize();
		virtual void run(jnr::JumpnRunData& data);
		void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		bool to_turn;
	};
};