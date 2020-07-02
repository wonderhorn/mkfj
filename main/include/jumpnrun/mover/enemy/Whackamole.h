#pragma once

#include"jumpnrun/mover/enemy/Enemy.h"
#include"framework/VirtualController.h"
#include"utils/Sprine.h"
namespace jnr {
	namespace enm
	{
		class Whackamole : public Enemy
		{
		public:
			void initialize(gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true, 
				const std::string& flag_id = "");
			virtual void run(jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			virtual int damage(const Damage& d);
			enm::Enemy* genEnemy(gmtr::Vector2D p);
			int meter, meter_max;
			char flag_id[JNR_FLAG_SIZE];

		private:
			const static int NUM_MOLE = 6;
			Object::Pipe pipes[NUM_MOLE];
			bool is_started;
			int timer_game;
		};
	};
};