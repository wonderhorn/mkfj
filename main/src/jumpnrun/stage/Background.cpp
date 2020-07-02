#include"jumpnrun/stage/Background.h"
#include"jumpnrun/system/Parmanent.h"
#include<math.h>
using namespace gfw;
using namespace jnr;

using std::max;
using std::min;

Background::Background() : translate_rate_x(1.0), translate_rate_y(1.0), offset_x(0), offset_y(32)
, img_width(640), img_height(480), img(-1)
, offset_x_min(-12800), offset_y_min(-720), offset_x_max(640), offset_y_max(480)
{}

void Background::run(jnr::JumpnRunData& data){}
void Background::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int x0 = offset_x - img_width - data.camera.X() * translate_rate_x;
	int y0 = offset_y - img_height - data.camera.Y() * translate_rate_y;
	x0 = max(offset_x_min, x0);
	y0 = max(offset_y_min, y0);
	x0 = min(offset_x_max, x0);
	y0 = min(offset_y_max, y0);
	//y0 = min(-480, y0); // forest‚Í‚±‚ê‚Å‚¤‚Ü‚­‚¢‚­
	y0 = (y0 % img_height);
	if (y0 > 0 )
		y0 -= img_height;
	//graphics->drawString("background", 0, 0, 16 * 10, 20);
	ImageHandler imgh = img >= 0 ? img : $P.img_school;
	for (int x = x0 % img_width; x < 640; x += img_width)
	{
		for (int y = y0; y < 480; y += img_height)
		//for (int y = y0 % img_height; y < 480; y += img_height)
		{
			graphics->draw(imgh, x, y, 0, 0, 0, 640, 480);
		}
	}

	for (BackgroundObject& obj : objects)
	{
		graphics->draw(obj.img, obj.x, obj.y, 0, 0, 0, obj.width, obj.height);
	}
}