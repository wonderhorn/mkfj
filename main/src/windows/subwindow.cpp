#include<windows.h>
#include"windows/subwindow.h"
using namespace space_subwindow;

SUBWINDOW SUBWINDOW::instance;

SUBWINDOW::SUBWINDOW(){}

SUBWINDOW::~SUBWINDOW()
{
	std::list<space_subwindow::GRAPH*>::iterator it = graphlist.begin();
	while(it != graphlist.end())
	{
		GRAPH* deletee = (*it);
		it = graphlist.erase(it);
	}
}

void SUBWINDOW::init()
{
	instance = SUBWINDOW();
	instance.Letter("test", 1, 1);
	instance.Box(0, 0, 40, 20);
	instance.Line(50, 20, 250, 20);
}

void SUBWINDOW::paint(HDC hDC)
{
	std::list<space_subwindow::GRAPH*>::iterator it = graphlist.begin();
	while(it != graphlist.end())
	{
		(*it)->paint(hDC);
		it++;
	}
	//MoveToEx( hDC, 100, 100, NULL );  // 始点を設定する
	//LineTo( hDC, 200, 200 );          // 直線を描く
}

void SUBWINDOW::Letter(const char* str, int x, int y)
{
	graphlist.push_front(new LETTERBOX(str, x, y));
}
void SUBWINDOW::Line(int orix, int oriy, int terx, int tery)
{
	graphlist.push_front(new LINE(orix, oriy, terx, tery));
}
void SUBWINDOW::Box(int x1, int y1, int x2, int y2)
{
	graphlist.push_front(new LINE(x1, y1, x2, y1));
	graphlist.push_front(new LINE(x2, y1, x2, y2));
	graphlist.push_front(new LINE(x2, y2, x1, y2));
	graphlist.push_front(new LINE(x1, y2, x1, y1));
}

LETTERBOX::LETTERBOX(const char *str, int x, int y): str(str), x(x), y(y)
{}
void LETTERBOX::paint(HDC hDC)
{
	TextOut(hDC, x, y, str.c_str(), strlen(str.c_str()));
}

LINE::LINE(int orix, int oriy, int terx, int tery)
: orix(orix), oriy(oriy), terx(terx), tery(tery)
{}
void LINE::paint(HDC hDC)
{
	MoveToEx( hDC, orix, oriy, NULL );  // 始点を設定する
	LineTo( hDC, terx, tery);          // 直線を描く
}