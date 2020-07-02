#include"network/ConnectionScreen.h"
#include"jumpnrun/system/Title.h"
#include"jumpnrun/system/JumpnRunScreen.h"
#include"jumpnrun/stage/StageSelectionScreen.h"
#include"jumpnrun/system/ConfigScreen.h"
using namespace gfw;

//Title::Title(): b (false){}

bool ConnectionScreen::initialize()
{
	cursor = 0;
	age = 0;
	state = Preparing;
	finished_timer = -1;
	return true;
}
void ConnectionScreen::finalize()
{
}

Screen::eScreenState ConnectionScreen::run(gfw::BaseMedia& media)
{
	/*if (age == 60)
	{
	$P.media.musics.startBGM("bgm\\begin.mp3");
	$P.media.musics.setAutoRepeat(false);
	}*/
	if (age == 60)
	{
		$P.sound_stack.push_back($P.snd_card);
	}

	if (finished_timer >= 0)
		finished_timer--;
	if (finished_timer == 0)
	{
		return eScreenState::Finished;
	}

	auto& udpcont = $P.udpcont;
	int port = 20000;
	if(state == ConnectionState::WaitAsServer)
	{
		bool connected = udpcont.try_accept();
		if (connected)
		{
			state = ConnectionState::Connected;
			finished_timer = 120;
		}
	}
	if (state == ConnectionState::WaitAsClient)
	{
		bool connected = udpcont.try_connect("127.0.0.1", port);
		if (connected)
		{
			state = ConnectionState::Connected;
			finished_timer = 120;
		}
	}

	const int num_menu = 3;
	if ($P.players[0].vc->Z().pushed())
	{
		if (age < 96)
		{
			age = 96;
		}
		else
		{
			if (state == ConnectionState::Preparing)
			{
				if (cursor == 0) //サーバ
				{
					udpcont.initializeSocket(20000, 40000, "127.0.0.1");
					state = ConnectionState::WaitAsServer;
				}
				else if (cursor == 1)// クライアント
				{
					udpcont.initializeSocket(40000, 20000, "127.0.0.1");
					state = ConnectionState::WaitAsClient;
				}
				else if (cursor == 2)
				{
				}
			}
			//$P.media.musics.stopBGM();
			//$P.media.musics.resetBGM();
			//return eScreenState::Finished;
		}
	}

	if ($P.players[0].vc->X().pushed())
	{
		udpcont.endConnection();
		return eScreenState::Finished;
	}

	if ($P.players[0].vc->Down().pushed())
	{
		cursor += 1;
		cursor %= num_menu;
		$P.sound_stack.push_back($P.snd_select);
		//$P.sound_stack.push_back($P.snd_mydamaged);
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
void ConnectionScreen::render(BaseMedia& media)
{
	//if (graphics->startRendering())
	if (1)
	{
		BaseGraphics* graphics = media.getGraphics();
		if (!b)
		{
			graphics->draw($P.img_paper, 0, 0, 0, 0, 0, 640, 480, false, 0xffffffff);
			//graphics->draw($P.img_logo, 0, -120, 0, 0, 0, 640, 480, false, 0xffffffff);
			int logo_y = age >= 80 ? 40 : 40 + age * 4 - 320;
			//graphics->drawSpinResize($P.img_logo, 0, logo_y, 0, 0, 0, 512, 221, false, 0xffffffff, 0.6, 0.6, 0);
			//graphics->draw($P.img_logo, (640 - 512) / 2, logo_y, 0, 0, 0, 512, 221, false, 0xffffffff);

			if (state == ConnectionState::Preparing)
			//if (age > 96)
			{
				graphics->drawString("サーバとして待ち受け", 300, 300, 200, 24);
				graphics->drawString("クライアントとして接続", 300, 300 + 40, 200, 24);
				graphics->drawString("タイトルにもどる", 300, 300 + 40 * 2, 200, 24);
				graphics->draw($P.img_cursor, 267, 300 - 6 + 40 * cursor, 0, 0, 0, 40, 80, false);
			}
			else if (state == ConnectionState::WaitAsServer)
			{
				graphics->drawString("接続待ち", 300, 300, 200, 24);
			}
			else if (state == ConnectionState::WaitAsClient)
			{
				graphics->drawString("接続中", 300, 300, 200, 24);
			}
			else if (state == ConnectionState::Connected)
			{
				graphics->drawString("接続できました", 300, 300, 200, 24);
			}
		}

		//graphics->endRendering();
	}
}