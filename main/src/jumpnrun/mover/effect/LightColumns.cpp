#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/GRAPHICS.h"
#include"jumpnrun/system/Parmanent.h"
#define _USE_MATH_DEFINES
#include<math.h>
using namespace jnr;
using namespace gfw;
using namespace gmtr;
using std::max;
using std::min;
#define $V gmtr::Vector2D
RegisterMoverClass(LightColumns);

void LightColumns::run(jnr::JumpnRunData& data)
{
	if (age % 2 == 0)
	{
		for (int i = 0; i < 20; i++)
		{
			column_heights[i] = $P.rand.get() % 12;
		}
	}
	Effect::run(data);
}

void LightColumns::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	const int column_width = 8;
	for (int i = 0; i < this->width; i += column_width)
	{
		int rnd = column_heights[i / column_width];
		//int rnd1 = min(5, rnd);
		int rnd1 = min(5, rnd);
		int rnd2 = max(0, rnd - 6);
		graphics->draw($P.img_obj, P().x - data.camera.X() + i, P().y - data.camera.Y(), 0
			, REFPOINT_LIGHT_COLUMN_X + rnd2 * column_width, REFPOINT_LIGHT_COLUMN_Y, column_width, height, false, 0xccffffff);
		//if (rnd2 == 0)
		if (0)
		{
			graphics->draw($P.img_obj, P().x - data.camera.X() + i, P().y - data.camera.Y() - height + 30, 0
				, REFPOINT_LIGHT_COLUMN_X + rnd1 * column_width, REFPOINT_LIGHT_COLUMN_Y
				, column_width, height - 30, false, 0xccffffff);
		}
		//graphics->drawSpinResize($P.img_obj, P().x - data.camera.X() + i, P().y - data.camera.Y(), 0
		//	, REFPOINT_LIGHT_COLUMN_X + rnd * column_width, REFPOINT_LIGHT_COLUMN_Y, column_width, height, false
		//	, 0xaaffffff, 1.5, 1, 0);
	}
}