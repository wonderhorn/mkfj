#pragma once
namespace gfw
{
	class Camera
	{
	public:
		Camera() : x(0), y(0), w(640), h(480), dx(0), dy(0), speed(2)
			, isFixed(false), outOfStage(false), quake_timer(0), quake_mag(0){}
		void setPos(int x, int y);
		void chase(int x, int y);
		void setQuake(int period, int magnitude);

		int X()const{ return x; }
		int Y()const{ return y; }
		int Width()const{ return w; }
		int Height()const { return h; }
		void setMax(int w, int h){ this->max_x = w; this->max_y = h; }

		bool isFixed, outOfStage;
	private:
		int x, y, w, h, dx, dy, speed, max_x, max_y;
		int quake_timer, quake_mag;
	};
};