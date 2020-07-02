#include"jumpnrun/ai/Radar.h"
#define _USE_MATH_DEFINES
#include<math.h>
#include"utils/MyCppUtil.h"
using namespace jnr;
using namespace ai;
using namespace gfw;

Radar::Radar() : num_bin(16), tl(NULL), turned(false){}

std::vector<jnr::Mover*> Radar::scan(double R)
{
	using namespace gmtr;
	std::vector<jnr::Mover*> rtn(num_bin);
	std::vector<double> r(num_bin);
	$for_i(num_bin)
	{
		rtn[i] = NULL;
		r[i] = FLT_MAX;
	}
	for (auto i = tl->begin(); i != tl->end(); i++)
	{
		Object* obj = getObject(i);
		if (!obj->alive || !obj->isMover)
			continue;
		Mover* mvr = static_cast<Mover*>(obj);
		if (mvr->ID() != myid)
		{
			Vector2D p = mvr->P();
			Vector2D dp = p - myp;
			if (turned)
				dp.x = -dp.x;
			double l2 = dp.l2();
			if (isnan(l2))
				continue;
			if (l2 != 0.0)
			{
				double cos_theta = dp.x / dp.l2();
				if (cos_theta > 1.0)
					cos_theta = 1.0;
				else if (cos_theta < -1.0)
					cos_theta = -1.0;
				double theta = acos(cos_theta);
				double dtheta = 2.0*M_PI / num_bin;
				if (dp.y < 0)
					theta = 2.0 * M_PI - theta;
				int index = theta / dtheta;
				if (index >= num_bin)
					index = num_bin - 1;

				if (index < 0)
				{
					volatile int a = 0;
				}
				if (l2 < r[index])
				{
					r[index] = l2;
					rtn[index] = mvr;
				}
			}
		}
	}
	return rtn;
}