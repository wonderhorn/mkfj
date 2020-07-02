#include"utils/Geometry.h"

namespace phsc
{
	class Physics
	{
	public:
		Physics():p(0, 0), v(0, 0), a(0, 0){}
		gmtr::Vector2D p, v, a;
		double mass;
	};
};