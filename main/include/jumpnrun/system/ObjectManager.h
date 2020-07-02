#pragma once
#include"framework/Object.h"
#include"jumpnrun/stage/StageDataLoader.h"
#include<vector>
#include<tuple>
#include<string>
namespace gfw
{
	class ObjectManager
	{
		friend class StageDataLoader;
	public:
		ObjectManager();
		ObjectManager(Tasklist* tl);
		void run();
		void generateNear();
		void setActiveWindow(int x, int y, int w, int h);
		void pushObject(int x, int y, std::string str, std::vector<double> args = {});

	private:
		Tasklist* tl;
		std::vector<Object::Pipe> pipes;
		std::vector < std::tuple < gmtr::Vector2D, std::string, std::vector<double> >> objects;
		int active_x, active_y, active_w, active_h, margin;
	};
};