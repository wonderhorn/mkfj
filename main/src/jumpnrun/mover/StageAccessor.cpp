#include"jumpnrun/mover/StageAccessor.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/mover/Mover.h"
using namespace gmtr;
using namespace gfw;
using namespace jnr;

StageAccessor::StageAccessor(const Stage& stg): stg(stg)
	, water_enterable(false)
	{}

double StageAccessor::distanceToWall(const gmtr::Vector2D& p, bool turned, const JumpnRunData& data)
{
	Vector2D dp = turned? Vector2D(- 12, 0): Vector2D(12, 0);
	double d = 12 * 24;
	for (int i = 0; i < 24; i++)
	{
		auto newp = p + dp * i;
		if (!stg.enterable(newp) || (newp.x < data.camera.X() + Constants::margin_camera_left
			|| newp.x >= data.camera.X() + data.camera.Width()))
		{
			d = 12 * i;
			break;
		}
	}
	return d;
}
double StageAccessor::distanceToFloor(const gmtr::Vector2D& p, const JumpnRunData& data)
{
	Vector2D dp = Vector2D(0, 12);
	double d = 12 * 50;
	for (int i = 0; i < 50; i++)
	{
		if (!stg.enterable(p + dp * i)
			|| (stg.getType(p + dp * i) == 'w' && !water_enterable))
		{
			d = 12 * i;
			break;
		}
	}
	return d;
}