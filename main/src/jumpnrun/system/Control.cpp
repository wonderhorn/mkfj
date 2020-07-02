#pragma once
#include"framework/Control.h"
//#include"Media.h"
#include"jumpnrun/system/Parmanent.h"
#include"framework/Screen.h"
using namespace gfw;
using namespace jnr;
using std::max;
using std::min;
CONTROL::CONTROL() : isActive(true){}

PAUSE::PAUSE(gfw::VirtualController* vc)
	: CONTROL(), vc(vc), age(0), cursory(0), for_stageselect(false)
{
	 cursory = $P.menu_cursor;
	 timer_exit = -1;
	 selected = Continue;

}
eControl PAUSE::run()
{
	if (timer_exit == 0)
		return selected;
	if (timer_exit > 0)
	{
		timer_exit--;
		return Continue;
	}
	//for_stageselect = true;
	int max_cursor = for_stageselect ? 6: 6;
	if (age > 0 && vc->Space().pushed())
		isActive = false;
	if (age > 5 && vc->X().pushed())
	{
		isActive = false;
		$P.sound_stack.push_back($P.snd_enter);

		return Continue;
	}
	if (timer_exit == -1 && age > 5 && vc->Z().pushed())
	{
		$P.sound_stack.push_back($P.snd_enter);

		switch (cursory)
		{
		case 0:
			isActive = false;
			return ToDeckEdit;
		case 1:
			isActive = false;
			return ToCardGallery;
		case 2:
			isActive = false;
			return ToAchievement;
		case 3:
			isActive = false;
			return ToConfig;
		case 4:
			$P.media.musics.stopBGM();
			timer_exit = 60;
			selected = ToStageSelect;
			break;
			//return ToStageSelect;
		case 5:
			$P.media.musics.stopBGM();
			timer_exit = 60;
			selected = ToTitle;
			break;
			//return ToTitle;
		}
	}
	if (vc->Down().pushed())
	{
		cursory++;
		cursory %= max_cursor;
		$P.sound_stack.push_back($P.snd_select);

	}
	if (vc->Up().pushed())
	{
		cursory += max_cursor - 1;
		cursory %= max_cursor;
		$P.sound_stack.push_back($P.snd_select);

	}
	$P.menu_cursor = cursory;
	age++;

	return Continue;
}
void PAUSE::render(gfw::BaseMedia& media)
{
	BaseGraphics* graphics = media.getGraphics();
	graphics->clear(0, 0, 0, 128);
	graphics->drawString("デッキ編集", 280, 160, 400, 20, 1, 1, 1, 1);
	graphics->drawString("カード一覧", 280, 160 + 41 * 1, 400, 20, 1, 1, 1, 1);
	graphics->drawString("実績", 280, 160 + 41 * 2, 400, 20, 1, 1, 1, 1);
	graphics->drawString("設定", 280, 160 + 41 * 3, 400, 20, 1, 1, 1, 1);
	graphics->drawString("マップへ", 280, 160 + 41 * 4, 400, 20, 1, 1, 1, 1);
	graphics->drawString("タイトルへ", 280, 160 + 41 * 5, 400, 20, 1, 1, 1, 1);
	//if (!for_stageselect)
	//	graphics->drawString("マップへ", 280, 160 + 41 * 4, 400, 20, 1, 1, 1, 1);
	graphics->draw($P.img_cursor, 220, 155 + cursory * 41, 0, 0, 0, 40, 40);

	if (timer_exit >= 0)
	{
		int a = (90 - timer_exit) * 6;
		if (a > 255)
			a = 255;
		graphics->clear(255, 255, 255, a);
	}
}


Wait::Wait(int t) : timer(t), age(0){}
eControl Wait::run()
{
	if (age >= timer)
	{
		isActive = false;
		return eControl::Continue;
	}

	age++;
	return eControl::RunJNR;
}



CHANGE_STAGE::CHANGE_STAGE()
	: CONTROL(), age(0), x(-1), y(-1)
{}

CHANGE_STAGE::CHANGE_STAGE(int x, int y)
: CONTROL(), age(0), x(x), y(y)
{}
eControl CHANGE_STAGE::run()
{
	if (age >= 48 + 30)
	{
		isActive = false;
		return eControl::ChangeStage;
	}

	age++;
	return eControl::Continue;
}
void CHANGE_STAGE::render(gfw::BaseMedia& media)
{
	BaseGraphics* graphics = media.getGraphics();
	int a = age * 8;
	if(a > 255)
		a = 255;
	if(x < 0)
		graphics->clear(0, 0, 0, a);
	else
		graphics->Whipe(x, y, max(15 * 48 - age * 15, 0));
	//Cd3d9->Whipe(ME::mypoint.getx() - CAMERA::getx(), ME::mypoint.gety() -  CAMERA::gety(), 1024 - age * 32);
}

END_JNR::END_JNR(eControl e)
	: CONTROL(), age(0), e(e)
{}
eControl END_JNR::run()
{
	if (age >= 32)
	{
		isActive = false;
		return e;
	}

	age++;
	return eControl::Continue;
}
void END_JNR::render(gfw::BaseMedia& media)
{
	BaseGraphics* graphics = media.getGraphics();
	int a = age * 8;
	if (a > 255)
		a = 255;
	graphics->clear(0, 0, 0, a);
	//Cd3d9->Whipe(ME::mypoint.getx() - CAMERA::getx(), ME::mypoint.gety() -  CAMERA::gety(), 1024 - age * 32);
}


START_STAGE::START_STAGE(int x, int y)
	: CONTROL(), age(0), x(x), y(y)
{}

START_STAGE::START_STAGE()
: CONTROL(), age(0), x(-1), y(-1)
{}
eControl START_STAGE::run()
{
	if (age >= 32)
	{
		isActive = false;
		//scr->finish = true;
	}

	age++;
	return eControl::Continue;
}
void START_STAGE::render(gfw::BaseMedia& media)
{
	BaseGraphics* graphics = media.getGraphics();
	int a = 255 - age * 8;
	if(a < 0)
	a = 0;
	if (x < 0)
		graphics->clear(0, 0, 0, a);
	else
		graphics->Whipe(x, y, age * 20);
	//Cd3d9->Whipe(ME::mypoint.getx() - CAMERA::getx(), ME::mypoint.gety() -  CAMERA::gety(), age * 32);
	//Cd3d9->Whipe(window_width / 2, window_height / 2, age * 32);
}


OtherScreen::OtherScreen(gfw::BaseMedia& media, gfw::Screen* scr) : media(media), scr(scr){}
eControl OtherScreen::run()
{
	gfw::Screen::eScreenState s = scr->run(media);
	if (s == gfw::Screen::eScreenState::Finished)
	{
		isActive = false;
		scr->finalize();
		delete scr;
		scr = nullptr;
	}
	age++;
	return Continue;
}
void OtherScreen::render(gfw::BaseMedia& media)
{
	BaseGraphics* graphics = media.getGraphics();
	graphics->clear(0, 0, 0, 128);
	scr->render(media);
}