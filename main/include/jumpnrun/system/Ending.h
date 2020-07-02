#include"framework/Screen.h"

class Ending : public gfw::Screen
{
public:
	Ending(gfw::Game* game) : gfw::Screen(game){}
	bool initialize();
	void finalize();
	eScreenState run(gfw::BaseMedia& media);
	void render(gfw::BaseMedia& media);

private:
	bool b;
	int cursor;
	int age;
	int timer_end;
};