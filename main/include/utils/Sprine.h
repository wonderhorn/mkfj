#pragma once
#include"utils/Geometry.h"

namespace gmtr
{
	class Sprine
	{
	public:
		Sprine();
		bool push_back(const Vector2D& p);

		static const int max_length = 32;
		int length;
		Vector2D points[max_length];
	};
};