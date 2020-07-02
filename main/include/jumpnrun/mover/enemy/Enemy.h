#pragma once
#include"jumpnrun/mover/character/Character.h"
#include"framework/VirtualController.h"
#include"utils/Sprine.h"
namespace jnr{
	namespace enm
	{
		class Enemy : public jnr::Character
		{
		public:
			virtual void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner);
			virtual void run(jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data) = 0;
			virtual const gfw::VirtualController& Controller(){ return mc; }
			virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
			//virtual int damage(int power, int stun_time, int invinc_time);
			virtual int damage(const Damage& d);
			virtual void die(jnr::JumpnRunData& data);
		protected:
			gfw::ManualController mc;
			int power;
			int lifetime;
			int timer_deactivate_damage;
			bool is_damaging, is_weak_to_water;
		};

		class Boy : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			int ai_timer;
			bool where_to_go, stopping;
		};

		class Volley : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void actz(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			//virtual void render();
			int ai_timer;
			bool where_to_go;
		};

		class Kendo : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void actz(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			//virtual void render();
			int ai_timer;
			bool where_to_go;
		};

		class Bicycle : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void actz(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			int ai_timer;
			bool where_to_go;
		};
		class Car : public Enemy
		{
		public:
			void initialize(int refx, int refy, int width, int height
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void actz(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			int ai_timer;
			bool where_to_go;
		};

		class Trap : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, int power, char direction = 'u');
			virtual void run(jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			char direction;
		};

		class TrapTimered : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, int power, int delay = 0);
			virtual void run(jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);

		private:
			int interval_on, interval_off, interval_transit, delay;
		};

		class Goblin : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void actz(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			//virtual void render();
			int ai_timer;
			bool where_to_go;
		};

		class Researcher : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void actz(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			//virtual void render();
			int ai_timer;
			bool where_to_go;
		};

		class Elf : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void actz(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void actx(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			//virtual void render();
			int ai_timer;
			bool where_to_go;
		};

		class Knight : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void actz(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void actx(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual int damage(const Damage& d);
			//virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			int ai_timer;
			bool where_to_go;
			gfw::Timer timer_guard;
			bool isGuarding;
		};

		class Cannon : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void actz(phsc::Physics& phys, jnr::JumpnRunData& data);
		protected:
			char direction;
		};
		class RotaryCannon : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void actz(phsc::Physics& phys, jnr::JumpnRunData& data);
		protected:
			char direction;
		};

		class Frog : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			//virtual void render();
			int ai_timer;
			bool where_to_go;
		};

		class Mushroom : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void actz(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			//virtual void render();
			int ai_timer;
			bool where_to_go;
		};

		class Slime : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, int rank, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void die(jnr::JumpnRunData& data);
			//virtual void render();
			int ai_timer;
			bool where_to_go;
			int rank;
		};

		class Bee : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			int ai_timer;
			char direction;
		};

		class Bug : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			//virtual void render();
			int ai_timer;
			bool where_to_go;
		};

		class Bat : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			int ai_timer;
			char direction;
		};

		class Bird : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			int ai_timer;
			char direction;
			bool where_to_go;
			double speed_up, speed_down;
			int favorite_height;
		};

		class Snail : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			//virtual int damage(int power, int stun_time, int invinc_time);
			virtual int damage(const Damage& d);
			//virtual void render();
			int ai_timer;
			bool where_to_go;
			gfw::Timer timer_guard;
			bool isGuarding;
		};

		class Pylonman : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			//virtual int damage(int power, int stun_time, int invinc_time);
			virtual int damage(const Damage& d);
			//virtual void render();
			int ai_timer, ai_fin_guard_timer;
			bool where_to_go;
			gfw::Timer timer_guard;
			bool isGuarding;
		};

		class Dustboxman : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			//virtual int damage(int power, int stun_time, int invinc_time);
			virtual int damage(const Damage& d);
			//virtual void render();
			int ai_timer;
			bool where_to_go;
			gfw::Timer timer_guard;
			bool isGuarding;
		};

		class Mandragora: public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void actz(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			//virtual int damage(int power, int stun_time, int invinc_time);
			virtual int damage(const Damage& d);
			//virtual void render();
			int ai_timer;
			bool where_to_go;
		};

		class Ghost : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			int ai_timer;
			char direction;
		};

		class Hand : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void die(jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void actz(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void actx(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			int ai_timer;
			char direction;

			enum eHandItem{None, Bomb, Block};
			eHandItem handitem;
		};

		class Gargoyle : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void actz(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			//virtual int damage(int power, int stun_time, int invinc_time);
			virtual int damage(const Damage& d);
			//virtual void render();
			int ai_timer;
			bool where_to_go;
			gfw::Timer timer_guard;
			bool isGuarding;
		};

		class Fairy : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void actz(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			int ai_timer;
			char direction;
		};

		class Fish : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			int ai_timer;
			char direction;
		};

		class Angel : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void actz(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void run(jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			int ai_timer, barrier_timer;
			char direction;

			Object::Pipe  pipe;
		};

		class BigEye : public Enemy
		{
		public:
			void initialize(gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			virtual void die(jnr::JumpnRunData& data);

			int ai_timer;
			char direction;

		private:
			static const int max_num_tentacles = 6;
			Object::Pipe tentacles[max_num_tentacles];
			gmtr::Vector2D eyev;
		};

		class BigFish : public Enemy
		{
		public:
			void initialize(gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void actz(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void run(jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			virtual void die(jnr::JumpnRunData& data);

			int ai_timer;
			char direction;

			const static int length_body = 12;
			gmtr::Vector2D points_body[length_body];

		private:
			//gmtr::Vector2D eyev;
		};

		class ManaFlower : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner);
			virtual void run(jnr::JumpnRunData& data);
			virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);

		};

		class Pumpkin : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void actz(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			int ai_timer;
			char direction, attack_direction;
		};
		class Candle : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void actz(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			int ai_timer;
			char direction, attack_direction;
		};

		class Mole : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned);
			virtual void run(jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			virtual int damage(const Damage& d);
			//virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);

		protected:
			//int lifetime;
			int ai_timer;
			char visible_height;
			bool hiding;
		};

		class Urchin : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner
				, bool turned = true, double mass = 0.0, int lifetime = 999999, int power=5);
			virtual void run(jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);

		protected:
			//int lifetime;
		};

		class JellyFish : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void actz(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			int ai_timer;
			char direction, attack_direction;
		};

		class Lamp : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void actz(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void actx(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			int ai_timer;
			char direction, attack_direction;
			Object::Pipe slime;
		};

		class BigSlime : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void die(jnr::JumpnRunData& data);
			void addCell(gmtr::Vector2D v);
			//virtual void render();
			int ai_timer;
			bool where_to_go;

			static const int num_cell = 24;
			Object::Pipe cells[num_cell];
		};

		class BigSlimeCell : public Enemy
		{
		public:
			void initialize(int refx, int refy
				, gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void die(jnr::JumpnRunData& data);

			static const int num_neighbors = 4;
			Object::Pipe neighbors[num_neighbors];
			Object::Pipe parent;

			int lifetime;
			int ai_timer;
			bool where_to_go;
			double radius;
		};
	};
};