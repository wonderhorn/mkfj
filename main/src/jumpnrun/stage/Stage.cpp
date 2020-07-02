#include"jumpnrun/stage/Stage.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/GRAPHICS.h"
#include"jumpnrun/mapchip.h"
#include"utils/string_util.h"
#include"framework/device_alternative_io.h"
#include<assert.h>
#include<iostream>
using namespace gfw;
using namespace jnr;
using std::max;
using std::min;

static double sign(double d)
{
	if (d > 0)
		return 1.0;
	else if (d < 0)
		return -1.0;
	else return 0;
}


LIST_OF_MAPCHIP Mapchip::Ref2Chip(int refx, int refy)
{
	for (int i = 0; i < NUM_MAPCHIP; i++)
	{
		if (refx == 64*3 && refy == 64*10)
		{
			int a = 0;
		}
		if (list_of_mapchips[i].x == refx &&  list_of_mapchips[i].y == refy)
			return list_of_mapchips[i];
	}
	return list_of_mapchips[4];
}


Stage::Stage(): width(0), height(0), chips(nullptr)
{
	dummy.type = 'f';
}

Stage::~Stage()
{
	if (chips)
	{
		delete[] chips;
	}
}

bool Stage::initialize()
{
	age = 0;
	curtainr = curtaing = curtainb = curtaina = 0;
	clouds.clear();
	return true;
}

void Stage::generateClouds()
{
	clouds.clear();
	for (int i = 0; i < 5; i++)
	{
		int t = $P.rand.get() % 4;
		Billboard b;
		b.translate_rate_x = 0;
		b.translate_rate_y = 0;
		switch (t)
		{
		default:
			break;
		case 0:
			b.refx = 0;
			b.refy = 0;
			b.w = 128;
			b.h = 64;
			break;
		case 1:
			b.refx = 128;
			b.refy = 0;
			b.w = 64;
			b.h = 32;
			break;
		case 2:
			b.refx = 128;
			b.refy = 32;
			b.w = 64;
			b.h = 32;
			break;
		case 3:
			b.refx = 192;
			b.refy = 0;
			b.w = 128;
			b.h = 64;
			break;
		}
		b.x = $P.rand.get() % 640;
		b.y = ($P.rand.get() % 128) - 32;
		clouds.push_back(b);
	}
}
void Stage::generateRainDrops()
{
	raindrops.clear();
	for (int i = 0; i < 200; i++)
	{
		Billboard b;
		b.translate_rate_x = 0;
		b.translate_rate_y = 1;

		b.refx = 0;
		b.refy = 64;
		b.w = 16;
		b.h = 16;

		b.x = $P.rand.get() % 640;
		b.y = $P.rand.get() % 480;
		raindrops.push_back(b);
	}
}
void Stage::generateBirds()
{
	raindrops.clear();
	for (int i = 0; i < 1; i++)
	{
		Billboard b;
		b.translate_rate_x = 0;
		b.translate_rate_y = 1;

		b.refx = REFPOINT_MINIBIRD_X;
		b.refy = REFPOINT_MINIBIRD_Y;
		b.w = 32;
		b.h = 32;

		b.x = 700;
		b.y = $P.rand.get() % 160;
		birds.push_back(b);
	}
	for (int i = 0; i < 1; i++)
	{
		Billboard b;
		b.translate_rate_x = 0;
		b.translate_rate_y = 1;

		b.refx = REFPOINT_MINIBIRD_X + 64;
		b.refy = REFPOINT_MINIBIRD_Y;
		b.w = 32;
		b.h = 32;

		b.x = 700 + $P.camera.X();
		b.y = $P.rand.get() % 160 + $P.camera.Y();
		birds.push_back(b);
	}
}

void Stage::finalize(){}

const Mapchip& Stage::ChipAt(const gmtr::Vector2D& v)const
{
	int index_x = v.x / Mapchip::WIDTH;
	int index_y = v.y / Mapchip::HEIGHT;
	if (0 > index_x || index_x >= width
		|| 0 > index_y || index_y >= height)
		return dummy;
	assert(index_x < width);
	assert(index_y < height);
	return chips[index_x + index_y * width];
}

const Mapchip& Stage::Chip(int index_x, int index_y)const
{
	if (0 > index_x || index_x >= width
		|| 0 > index_y || index_y >= height)
		return dummy;
	assert(index_x < width);
	assert(index_y < height);
	return chips[index_x + index_y * width];
}

Mapchip& Stage::ChipMutable(int index_x, int index_y)
{
	if (0 > index_x || index_x >= width
		|| 0 > index_y || index_y >= height)
		return dummy;
	assert(index_x < width);
	assert(index_y < height);
	return chips[index_x + index_y * width];
}

bool Stage::enterable(const gmtr::Vector2D& v_prev, const gmtr::Vector2D& v_next)const
{
	bool rtn = true;
	int index_x = v_next.x / Mapchip::WIDTH;
	int index_y = v_next.y / Mapchip::HEIGHT;
	if (0 > index_x || index_x >= width
		|| 0 > index_y || index_y >= height)
		return false;
	char t = Chip(index_x, index_y).type;
	if (t == 'f')
		rtn = false;
	if (t == 'h')
		rtn = v_next.y - index_y * Mapchip::HEIGHT
		< Mapchip::HEIGHT / 2;
	if (t == 'l')
		rtn = v_next.y - index_y * Mapchip::HEIGHT
		> Mapchip::HEIGHT / 2;
	return rtn;
}

bool Stage::enterable(const gmtr::Vector2D& v)const
{
	bool rtn = true;
	int index_x = v.x / Mapchip::WIDTH;
	int index_y = v.y / Mapchip::HEIGHT;
	if (0 > index_x || index_x >= width
		|| 0 > index_y || index_y >= height)
		return false;
	if (v.x < 0 || v.y < 0)
		return false;

	char t = Chip(index_x, index_y).type;
	if (t == 'f')
		rtn = false;
	if (t == 'h')
		rtn = v.y - index_y * Mapchip::HEIGHT
		< Mapchip::HEIGHT / 2;
	if (t == 'l')
		rtn = v.y - index_y * Mapchip::HEIGHT
		> Mapchip::HEIGHT / 2;

	return rtn;
}

static double abs_add01(double d)
{
	if (d > 0)
		return (d + .0);
	else if (d < 0)
		return (d - .0);
	else return 0.0;
}
static double abs_add1(double d)
{
	if (d > 0)
		return (d + .5);
	else if (d < 0)
		return (d - .5);
	else return 0.0;
}
gmtr::Vector2D Stage::react(const gmtr::Vector2D& v_prev, const gmtr::Vector2D& v_next, bool up)
{
	gmtr::Vector2D rtn(0.0, 0.0);
	gmtr::Vector2D v = v_next - v_prev;
	int index_x = (v_next.x) / Mapchip::WIDTH;
	int index_y = (v_next.y) / Mapchip::HEIGHT;
	int index_x_prev = v_prev.x / Mapchip::WIDTH;
	int index_y_prev = (v_prev.y) / Mapchip::HEIGHT;

	char t = Chip(index_x, index_y).type;
	char t_prev = Chip(index_x_prev, index_y_prev).type;

	//if (t == 'n')
	if (enterable(v_next))
	{
		return rtn;
	}
	//if (t == 'f')
	else
	{
		/*if (index_y != index_y_prev)
			rtn.y = -v.y;
		else
		{
			double offset_y = v_prev.y - index_y_prev * Mapchip::HEIGHT;
			if (offset_y < 0.1 * Mapchip::HEIGHT
				|| offset_y > 0.9 * Mapchip::HEIGHT)
			//if (offset_y > 0.9 * Mapchip::HEIGHT)
				rtn.y = -v.y;
			else rtn.x = -v.x;

			int index_y_true = (v_next.y - 2) / Mapchip::HEIGHT;
			if (Chip(index_x, index_y_true).type == 'f')
			{
				rtn.x = -abs_add01(v.x);
			}
		}*/
		gmtr::Vector2D vv(0.0, v.y);
		gmtr::Vector2D vh(v.x, 0.0);
		int index_x_nexth = (v_prev + vh).x / Mapchip::WIDTH;
		int index_y_nexth = ((v_prev + vh).y - 2) / Mapchip::HEIGHT;
		int index_x_nextv = ((v_prev + vv).x - 2 * sign(vv.x)) / Mapchip::WIDTH;
		int index_y_nextv = (v_prev + vv).y / Mapchip::HEIGHT;
		//if (Chip(index_x_nexth, index_y_nexth).type == 'f')
		if (!enterable(v_prev + vh - gmtr::Vector2D(0.0, 2.0)))
		{
			rtn.x = -abs_add1(v.x);
			if (index_x_nexth != index_x_nextv
				|| index_y_nexth != index_y_nextv
				|| v.y < 0)
			//if (index_x_nexth != index_x_nextv)
			{
				//if (Chip(index_x_nextv, index_y_nextv).type == 'f')
				if (!enterable(v_prev + vv + rtn - gmtr::Vector2D(2.0 * sign(vv.x), 0.0)))
				{
					if ((!up && v.y > 0) || (up && v.y < 0))
						rtn.y = -abs_add01(v.y);
				}
			}
		}
		//else if (Chip(index_x_nextv, index_y_nextv).type == 'f')
		else if (!enterable(v_prev + vv - gmtr::Vector2D(2.0 * sign(vv.x), 0.0)))
		{
			if ((!up && v.y > 0) || (up && v.y < 0))
				rtn.y = -abs_add01(v.y);
		}
	}
	return rtn;
}

char Stage::getType(const gmtr::Vector2D& v)const
{
	int index_x = v.x / Mapchip::WIDTH;
	int index_y = v.y / Mapchip::HEIGHT;
	if (0 > index_x || index_x >= width
		|| 0 > index_y || index_y >= height)
		return 'f';
	if (v.x < 0 || v.y < 0)
		return 'f';

	return Chip(index_x, index_y).type;
}

void Stage::run(jnr::JumpnRunData& data)
{
	int x_ori = max(0, (data.camera.X() / Mapchip::WIDTH));
	int y_ori = max(0, data.camera.Y() / Mapchip::HEIGHT);
	int x_ter = min(width - 1, (data.camera.X() + data.camera.Width()) / Mapchip::WIDTH);
	int y_ter = min(height - 1, (data.camera.Y() + data.camera.Height()) / Mapchip::HEIGHT);
	for (int x = x_ori; x <= x_ter; x++)
	{
		for (int y = y_ori; y <= y_ter; y++)
		{
			Mapchip& c = ChipMutable(x, y);

			const Mapchip& c1 = Chip(x + 1, y);
			const Mapchip& c2 = Chip(x - 1, y);
			if (c.type == 'n' &&
				c1.type == 'w' && c2.type == 'w' &&
				!(c.type == 'w'))
			{
				c.type = 'w';
			}
		}
	}

	for (Billboard &b : raindrops)
	{
		b.y += 6;
		if (b.y > data.camera.Y() + 500)
		{
			b.x = $P.rand.get() % 640;
			b.y = data.camera.Y() - 64;
		}
	}
	for (Billboard &b : birds)
	{
		b.x -= 1;
		if (b.x < data.camera.X() - 120)
		{
			b.x = $P.rand.get() % 400 +940 + data.camera.X();
			b.y = $P.rand.get() % 160 + data.camera.Y();
		}
	}
	if (age % 12 == 0)
	{
		for (Billboard &b : clouds)
		{
			b.x -= 1;
			if (b.x < -b.w)
				b.x = 640;
		}
	}
	age++;
}

void Stage::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int x_ori = max(0, (data.camera.X() / Mapchip::WIDTH));
	int y_ori = max(0, data.camera.Y() / Mapchip::HEIGHT);
	int x_ter = min(width - 1, (data.camera.X() + data.camera.Width()) / Mapchip::WIDTH);
	int y_ter = min(height - 1, (data.camera.Y() + data.camera.Height()) / Mapchip::HEIGHT);

	if (1)
	{
		for (auto b : clouds)
		{
			if (b.x + b.w >= x_ori || b.y + b.h >= y_ori
				|| b.x <= x_ter || b.y <= y_ter)
			{
				//graphics->draw($P.name2img[n], b.x - data.camera.X(), b.y - data.camera.Y(), 0
				//graphics->draw($P.img_clouds, b.x - (data.camera.X()) * b.translate_rate_x, b.y, 1.0
				graphics->draw($P.img_clouds, b.x, b.y, .0
					, b.refx, b.refy, b.w, b.h);
			}
		}
		for (auto b : birds)
		{
			if (b.x + b.w >= x_ori || b.y + b.h >= y_ori
				|| b.x <= x_ter || b.y <= y_ter)
			{
				graphics->draw($P.img_obj, b.x - data.camera.X(), b.y - data.camera.Y(), .0
					, b.refx + ((age / 12) % 2) * 32, b.refy, b.w, b.h);
			}
		}
		for (auto b : raindrops)
		{
			if (b.x + b.w >= x_ori || b.y + b.h >= y_ori
				|| b.x <= x_ter || b.y <= y_ter)
			{
				graphics->draw($P.img_clouds, b.x, b.y - data.camera.Y(), .0
					, b.refx, b.refy, b.w, b.h);
			}
		}
	}

	for (auto b : billboards)
	{
		int bx = - (data.camera.X() + 320 - b.x) * b.translate_rate_x + 320;
		//int bx = b.x - 320 - (data.camera.X() - 320) * b.translate_rate_x;
		if (bx + b.w >= x_ori || b.y + b.h >= y_ori
			|| bx <= x_ter || b.y <= y_ter)
		{
			auto v = myutil::split(b.file_name, "\\");
			std::string n = v[v.size() - 1];
			//graphics->draw($P.name2img[n], b.x - data.camera.X(), b.y - data.camera.Y(), 0
			graphics->draw($P.name2img[n], bx, b.y - data.camera.Y(), 0
				, b.refx, b.refy, b.w, b.h);
		}
	}

	for (int x = x_ori; x <= x_ter; x++)
	{
		for (int y = y_ori; y <= y_ter; y++)
		{
			const Mapchip& c = Chip(x, y);

			const Mapchip& c1 = Chip(x + 1, y);
			const Mapchip& c2 = Chip(x - 1, y);
			/*
			if (//c.type != 'n' && 
				c1.type == 'w' && c2.type == 'w')
			{
				if(!(c.refx == 64 * 3 && c.refy == 64 * 3)
					&& !(c.refx == 64 * 3 && c.refy == 64 * 4))
					graphics->draw($P.img_map, x * Mapchip::WIDTH - data.camera.X()
						, y * Mapchip::HEIGHT - data.camera.Y(), 0
						, 64 * 3, 64 * 4, Mapchip::WIDTH, Mapchip::HEIGHT, false, water_alpha);
			}*/

			int offset_refx = 0;

			// kyo's father's portrait
			if (c.refx == 832 && c.refy == 256) 
			{
				if ($P.getFlag("turnport"))
					offset_refx = 64;
			}

			if (!c.foreground)
			{
				if (c.refx >= 0)
				{
					/*if (c.refx == 64 * 4 && c.refy == 64 * 7)
					{
						graphics->draw($P.img_map, x * Mapchip::WIDTH - data.camera.X(), y * Mapchip::HEIGHT - data.camera.Y(), 0
							, 64 * 3, 64 * 3, Mapchip::WIDTH, Mapchip::HEIGHT, false, 0xaaffffff);
					}*/

					graphics->draw($P.img_map, x * Mapchip::WIDTH - data.camera.X(), y * Mapchip::HEIGHT - data.camera.Y(), 0
						, c.refx + offset_refx, c.refy, Mapchip::WIDTH, Mapchip::HEIGHT);
				}
			}
		}
	}
}

void Stage::renderForeground(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int x_ori = max(0, (data.camera.X() / Mapchip::WIDTH));
	int y_ori = max(0, data.camera.Y() / Mapchip::HEIGHT);
	int x_ter = min(width - 1, (data.camera.X() + data.camera.Width()) / Mapchip::WIDTH);
	int y_ter = min(height - 1, (data.camera.Y() + data.camera.Height()) / Mapchip::HEIGHT);

	for (int x = x_ori; x <= x_ter; x++)
	{
		for (int y = y_ori; y <= y_ter; y++)
		{
			const Mapchip& c = Chip(x, y);

			//check water
			
			const Mapchip& c1 = Chip(x + 1, y);
			const Mapchip& c2 = Chip(x - 1, y);
			
			if(//c.type != 'n' && 
				c1.type == 'w' && c2.type == 'w' && !c.waterproof)
			{
				if (!(c.refx == 64 * 3 && c.refy == 64 * 3)
					&& !(c.refx == 64 * 3 && c.refy == 64 * 4))
					graphics->draw($P.img_map, x * Mapchip::WIDTH - data.camera.X()
						, y * Mapchip::HEIGHT - data.camera.Y(), 0
						, 64 * 3, 64 * 4, Mapchip::WIDTH, Mapchip::HEIGHT, false, water_alpha);
			}
			

			if (c.foreground)
			{
				if (c.refx >= 0)
				{
					graphics->draw($P.img_map, x * Mapchip::WIDTH - data.camera.X(), y * Mapchip::HEIGHT - data.camera.Y(), 0
						, c.refx, c.refy, Mapchip::WIDTH, Mapchip::HEIGHT, false, c.alpha);
				}
			}
		}
	}

	if (curtaina != 0) {
		graphics->clear(curtainr, curtaing, curtainb, curtaina);
	}
}


void Stage::initializeTestStage()
{
	if (!chips)
		delete[] chips;
	width = 10;
	height = 6;
	startp = gmtr::Vector2D(32, 64 * 4);
	if (chips)
		delete[] chips;
	chips = new Mapchip[width * height];
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			Mapchip& c = chips[x + y * width];
			c.x = x * Mapchip::WIDTH;
			c.y = y * Mapchip::HEIGHT;
			c.refx = -1;
			c.refy = -1;
			c.type = 'n';
			c.w = Mapchip::WIDTH;
			c.h = Mapchip::HEIGHT;
		}
	}

	for (int x = 0; x < width; x++)
	{
		for (int y = height - 1; y < height; y++)
		{
			Mapchip& c = chips[x + y * width];
			c.x = x * Mapchip::WIDTH;
			c.y = y * Mapchip::HEIGHT;
			c.refx = 0;
			c.refy = 0;
			c.type = 'f';
			c.w = Mapchip::WIDTH;
			c.h = Mapchip::HEIGHT;
		}
	}

	/*Mapchip& c = chips[6 + 4 * width];
	c.x = 6 * Mapchip::WIDTH;
	c.y = 4 * Mapchip::HEIGHT;
	c.refx = 0;
	c.refy = 0;
	c.type = 'f';
	c.w = Mapchip::WIDTH;
	c.h = Mapchip::HEIGHT;

	chips[7 + 3 * width].x = 7 * Mapchip::WIDTH;
	chips[7 + 3 * width].y = 7 * Mapchip::HEIGHT;
	chips[7 + 3 * width].refx = 0;
	chips[7 + 3 * width].refy = 0;
	chips[7 + 3 * width].type = 'f';

	chips[4 + 2 * width].x = 4 * Mapchip::WIDTH;
	chips[4 + 2 * width].y = 2 * Mapchip::HEIGHT;
	chips[4 + 2 * width].refx = 0;
	chips[4 + 2 * width].refy = 0;
	chips[4 + 2 * width].type = 'f';*/
}

static char getChipType(int refx, int refy)
{
	for (int i = 0; i < NUM_MAPCHIP; i++)
	{
		if (refx == list_of_mapchips[i].x && refy == list_of_mapchips[i].y)
			return list_of_mapchips[i].type;
	}
	return 'n';
}

Stage Stage::readFile(const std::string& filename)
{
	Stage rtn;
	FILE* fp;
	myfopen(fp, filename.c_str(), "r");
	char buf[10240] = {};
	char *cont;
	while (fgets(buf, 10240, fp))
	{
		char* s = my_strtok(buf, " ", &cont);
		if (strcmp(s, "width") == 0)
		{
			rtn.width = atoi(my_strtok(NULL, " ", &cont));
		}
		else if (strcmp(s, "height") == 0)
		{
			rtn.height = atoi(my_strtok(NULL, " ", &cont));
		}
		else if (strcmp(s, "chips") == 0)
		{
			if (rtn.chips)
				delete[] rtn.chips;
			rtn.chips = new Mapchip[rtn.width * rtn.height];
			for (int y = 0; y < rtn.height; y++)
			{
				for (int x = 0; x < rtn.width; x++)
				{
					int refx = atoi(my_strtok(NULL, " ", &cont));
					int refy = atoi(my_strtok(NULL, " ", &cont));
					Mapchip& c = rtn.chips[x + y * rtn.width];
					c.x = x * Mapchip::WIDTH;
					c.y = y * Mapchip::HEIGHT;
					c.refx = refx;
					c.refy = refy;
					c.type = getChipType(refx, refy);
					c.w = Mapchip::WIDTH;
					c.h = Mapchip::HEIGHT;
				}
			}
		}
		else if (strcmp(s, "object") == 0)
		{
			/*int x = atoi(strtok(NULL, " "));
			int y = atoi(strtok(NULL, " "));
			int refx = atoi(strtok(NULL, " "));
			int refy = atoi(strtok(NULL, " "));
			char* s = strtok(NULL, " ");
			objputter->push(x, y, refx, refy, s);*/
		}
	}

	fclose(fp);
	return rtn;
}

void Stage::Curtain(int r, int g, int b, int a) 
{
	this->curtainr = r;
	this->curtaing = g;
	this->curtainb = b;
	this->curtaina = a;
}