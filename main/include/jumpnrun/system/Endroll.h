#pragma once
#include<map>
#include<string>
#include"framework/Screen.h"

class EndrollObject 
{
public:
	EndrollObject(){}
	EndrollObject(int x, int y, int refx,
		int  refy, int  w, int h, int dx, int dy);
	void setAnimation(int anim_num, int anim_interval);
	float x, y, refx, refy, w, h, dx, dy, ddx, ddy;
	float scale, dscale;
	bool turned;
	int anim_num, anim_interval;
	int anim_counter;
};

class Endroll : public gfw::Screen
{
public:
	Endroll(gfw::Game* game) : gfw::Screen(game) {}
	bool initialize();
	void finalize();
	eScreenState run(gfw::BaseMedia& media);

	void render(gfw::BaseMedia& media);

private:
	void play(int t, std::map<std::string, EndrollObject>& objects);

	bool b;
	int cursor;
	int age;

	std::map<std::string, EndrollObject> objects;
	std::map<std::string, EndrollObject> mapchips;
	std::vector<std::string> texts;

	int bgx, bgy;
	ImageHandler* background;
};
