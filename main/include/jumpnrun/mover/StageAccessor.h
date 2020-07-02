#pragma once
#include"jumpnrun/stage/Stage.h"
#include"jumpnrun/system/JumpnRunData.h"

namespace jnr
{
	class StageAccessor
	{
	public:
		StageAccessor(const Stage& stg);
		double distanceToWall(const gmtr::Vector2D& p, bool turned, const JumpnRunData& data);
		double distanceToFloor(const gmtr::Vector2D& p, const JumpnRunData& data);

		bool water_enterable;
	private:
		const Stage& stg;
	};
}