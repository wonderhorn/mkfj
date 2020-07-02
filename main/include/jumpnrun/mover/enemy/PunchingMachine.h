#pragma once

#include"jumpnrun/mover/enemy/Enemy.h"
#include"framework/VirtualController.h"

namespace jnr {
	namespace enm
	{
		class PunchingMachine : public Enemy
		{
		public:
			void initialize(gmtr::Vector2D p, OWNERHANDLE owner, bool turned = true);
			virtual void run(jnr::JumpnRunData& data);
			virtual void think(jnr::JumpnRunData& data);
			virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
			virtual int damage(const Damage& d);
			int meter, meter_max;
		};
	};
};