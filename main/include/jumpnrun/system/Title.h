#include"framework/Screen.h"

class Title : public gfw::Screen
{
public:
	Title(gfw::Game* game) : gfw::Screen(game){}
	bool initialize();
	void finalize();
	eScreenState run(gfw::BaseMedia& media);
	void render(gfw::BaseMedia& media);

private:
	bool b;
	int cursor;
	int age;
};