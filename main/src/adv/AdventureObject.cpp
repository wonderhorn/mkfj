#include"adv/AdventureObject.h"
#include"jumpnrun/system/Parmanent.h"
using namespace gmtr;
using namespace gfw;
using namespace adv;

//AdvCharacter::AdvCharacter(){}
void AdvCharacter::initialize(const std::string& name, ImageHandler img, gmtr::Vector2D p)
{
	this->name = name;
	this -> phys.p = p;
	this->phys.v = Vector2D(0, 0);
	this->img = img;
	refx = refy = 0;
	width = 320;
	height = 480;
	width_coef = 1.0;
	height_coef = 1.0;
	e_refx = e_refy = e_x = e_y = -1;
}
void AdvCharacter::run()
{
	if (timer_move > 0)
	{
		phys.p += phys.v;
		width_coef += mag_x;
		height_coef += mag_y;
		timer_move--;
	}
	else if (timer_move == 0)
	{
		if (!motions.empty())
		{
			auto m = motions.front();
			setMotionDeformation(std::get<0>(m), std::get<1>(m)
				, std::get<2>(m), std::get<3>(m), std::get<4>(m));
			motions.pop_front();
		}
		else
		{
			phys.v = Vector2D(0.0, 0.0);
			mag_x = 0;
			mag_y = 0;
		}
	}
}
void AdvCharacter::render(gfw::BaseMedia& media)
{
	BaseGraphics* graphics = media.getGraphics();
	graphics->drawSpinResize(img, phys.p.x, phys.p.y, 0, refx, refy, width, height
		, false, 0xffffffff, width_coef, height_coef, 0);
	if (e_refx >= 0)
	{
		graphics->draw($P.img_effect, phys.p.x + e_x, phys.p.y + e_y, 0, e_refx, e_refy, 128, 128, false);
		//graphics->drawSpinResize($P.img_effect, phys.p.x + e_x, phys.p.y + e_y, 0
		//	, e_refx, e_refy, 96, 96, false, 0xffffffff, width_coef, height_coef, 0);
	}
}

void AdvCharacter::setMotion(double vx, double vy, int t)
{
	this->phys.v = Vector2D(vx, vy);
	this->timer_move = t;
}

void AdvCharacter::pushMotion(double vx, double vy, int t)
{
	motions.push_back(std::tuple<double, double, double, double, int>(vx, vy, 0, 0, t));
}

void AdvCharacter::setDeformation(double vx, double vy, int t)
{
	mag_x = vx;
	mag_y = vy;
	//this->phys.v = Vector2D(vx, vy);
	this->timer_move = t;
}
void AdvCharacter::pushDeformation(double vx, double vy, int t)
{
	motions.push_back(std::tuple<double, double, double, double, int>(0, 0, vx, vy, t));
}
void AdvCharacter::setMotionDeformation(double vx, double vy, double defvx, double defvy, int t)
{
	mag_x = defvx;
	mag_y = defvy;
	this->phys.v = Vector2D(vx, vy);
	this->timer_move = t;
}
void AdvCharacter::pushMotionDeformation(double vx, double vy, double defvx, double defvy, int t)
{
	motions.push_back(std::tuple<double, double, double, double, int>(vx, vy, defvx, defvy, t));
}

void AdvCharacter::setEffect(int x, int y, int refx, int refy)
{
	this->e_x = x;
	this->e_y = y;
	this->e_refx = refx;
	this->e_refy = refy;
}