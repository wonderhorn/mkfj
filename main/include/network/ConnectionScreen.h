#pragma once
#include"framework/Screen.h"

class ConnectionScreen : public gfw::Screen
{
public:
	ConnectionScreen(gfw::Game* game) : gfw::Screen(game) {}
	bool initialize();
	void finalize();
	eScreenState run(gfw::BaseMedia& media);
	void render(gfw::BaseMedia& media);

	enum ConnectionState {Preparing, WaitAsServer, WaitAsClient, Connected, Disconnected};

private:
	bool b;
	int cursor;
	int age;
	int finished_timer;
	ConnectionState state;
};