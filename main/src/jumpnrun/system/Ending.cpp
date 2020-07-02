#include"jumpnrun/system/Ending.h"
#include"jumpnrun/system/Title.h"
#include"jumpnrun/system/JumpnRunScreen.h"
#include"jumpnrun/system/ConfigScreen.h"
#include"utils/string_util.h"
using namespace gfw;

using std::min;
using std::max;
//Title::Title(): b (false){}

bool Ending::initialize()
{
	cursor = 0;
	age = 0;
	timer_end = 128;
	return true;
}
void Ending::finalize()
{
}

Screen::eScreenState Ending::run(gfw::BaseMedia& media)
{
	const int num_menu = 3;

	if (age == 90)
	{
		$P.media.musics.resetBGM();
		$P.media.musics.startBGM("bgm\\stageselect.mp3");
		$P.media.musics.setAutoRepeat(true);
	}

	if (age > 450)
	{
		if (1)
		//if (timer_end > 0)
			timer_end--;
	}

	if ($P.players[0].vc->Z().pushed())
	{
		if (age < 300)
		{
			//age = 300;
		}
		else
		{
			/*
			$P.sound_stack.push_back($P.snd_enter);
			auto* jnrscr = new Title(game);
			jnrscr->initialize();
			game->push_back(jnrscr);
			auto* title = new Title(game);
			title->initialize();
			game->push_back(title);

			return eScreenState::Finished;*/
		}
	}

	if (timer_end == -150)
	{
		//$P.sound_stack.push_back($P.snd_enter);
		auto* title = new Title(game);
		title->initialize();
		game->push_back(title);
		$P.media.musics.stopBGM();

		return eScreenState::Finished;
	}
	//if (media.input.key_x.isDown())
	//	return eScreenState::Finished;

	if ($P.players[0].vc->Down().pushed())
	{
		cursor += 1;
		cursor %= num_menu;
		$P.sound_stack.push_back($P.snd_select);
	}
	if ($P.players[0].vc->Up().pushed())
	{
		cursor += num_menu - 1;
		cursor %= num_menu;
		$P.sound_stack.push_back($P.snd_select);
	}
	age++;
	return eScreenState::Continuing;
}
void Ending::render(BaseMedia& media)
{
	//if (graphics->startRendering())
	if (1)
	{
		BaseGraphics* graphics = media.getGraphics();
		if (!b)
		{
			graphics->draw($P.img_paper, 0, 0, 0, 0, 0, 640, 480, false, 0xffffffff);
			int fx = 240 +150 + max(96 * 2 - (age - 240) * 2 , 0);
			graphics->draw($P.img_fujitsubo, fx, 80, 0, 0, 0, 400, 400, false, 0xffffffff);
			//graphics->draw($P.img_logo, 0, -120, 0, 0, 0, 640, 480, false, 0xffffffff);
			int logo_y = age >= 80 ? -80 : -80 + age * 3 - 240;
			//graphics->drawSpinResize($P.img_logo, 0, logo_y, 0, 0, 0, 640, 480, false, 0xffffffff, 0.6, 0.6, 0);
			//graphics->drawString("タイトル画面", 0, 0, 200, 24);
			//graphics->drawString("タイトル画面", 0, 24, 200, 24, 1, 0, 0, 1);
			std::string txt = "Thank you for playing！";
			int txtl = (age - 30) / 6;
			int index = 0, counter = 0;
			while (index < txtl && counter < txt.size())
			{
				char c = txt[counter];
				int l = myutil::getMultiByteCharLength(c);
				index += l;
				counter++;
			}
			graphics->drawString(txt.substr(0, index)
					, 124, 128, 200, 50);

			if (age < 64)
				graphics->clear(0, 0, 0, 255 - age * 4);
			if (timer_end < 128)
			{
				int a = 255 - timer_end * 2;
				if (a > 255)
					a = 255;
				graphics->clear(0, 0, 0, a);
			}
		}

		//graphics->endRendering();
	}
}