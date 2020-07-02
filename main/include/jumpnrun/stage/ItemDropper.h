#pragma once
#include"jumpnrun/system/JumpnRunData.h"
#include"jumpnrun/mover/enemy/Enemy.h"
namespace jnr
{
	class ItemDropper
	{
	public:
		void drop(gfw::Tasklist& tl
			, const gmtr::Vector2D& p, const std::string& dropper_name);
	};
};