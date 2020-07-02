#pragma once
#include"framework/MediaAlternative.h"
#include"framework/Object.h"
#include<deque>
#include<tuple>

namespace adv
{
	class AdvCharacter: public gfw::Object
	{
	public:
		AdvCharacter(){}
		void initialize(const std::string& name, ImageHandler img, gmtr::Vector2D p);
		void run();
		void render(gfw::BaseMedia& media);
		void setMotion(double vx, double vy, int t);
		void pushMotion(double vx, double vy, int t);
		void setDeformation(double vx, double vy, int t);
		void pushDeformation(double vx, double vy, int t);
		void setMotionDeformation(double vx, double vy, double defvx, double defvy, int t);
		void pushMotionDeformation(double vx, double vy, double defvx, double defvy, int t);
		void setImage(ImageHandler img){ this->img = img; }
		void setEffect(int x, int y, int refx, int refy);
		void setCoef(double x, double y){ width_coef = x; height_coef = y; }
	private:
		std::string name;
		ImageHandler img;
		int refx, refy, width, height;
		int e_x, e_y, e_refx, e_refy;
		double width_coef, height_coef, mag_x, mag_y;
		int timer_move;
		std::deque<std::tuple<double, double, double, double, int>> motions; //speed_x, speed_y, deformation_x, def
	};
};