#pragma once
#include<string>
#include"framework/Game.h"

class Mkfj {
public:
	Mkfj();
	bool initialize();
	void update();
	std::string render();

private:
	gfw::Game* game;
};