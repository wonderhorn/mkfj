#include"jumpnrun/GRAPHICS.h"
#include"jumpnrun/system/Title.h"
#include"jumpnrun/system/JumpnRunScreen.h"
#include"jumpnrun/stage/StageSelectionScreen.h"
#include"jumpnrun/system/ConfigScreen.h"
using namespace gfw;

//Title::Title(): b (false){}

bool Title::initialize()
{
	cursor = 0;
	age = 0;
	return true;
}
void Title::finalize()
{
}

Screen::eScreenState Title::run(gfw::BaseMedia& media)
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

	const int num_menu = 3;
	if ($P.players[0].vc->Z().pushed())
	{
		if (age < 96)
		{
			age = 96;
		}
		else{
			if (cursor == 0 || cursor == 1) //‚Â‚Ã‚«‚©‚çE‚Í‚¶‚ß‚©‚ç
			{
				$P.sound_stack.push_back($P.snd_enter);
				$P.reset_bgm = true;
				if (cursor == 0) //‚Â‚Ã‚«‚©‚½
				{
					$P.load($P.save_file_name);
					$P.players[0].hp_current = $P.players[0].hp_max;
					$P.players[0].mp_current = $P.players[0].mp_max;
				}
				if (cursor == 1) //‚Í‚¶‚ß‚©‚ç
				{
					$P.renameSaveData();
					$P.reset();
				}
				//if (0)
				if (!$P.in_stage)
				{
					auto* jnrscr = new gfw::StageSelectionScreen(game);
					jnrscr->initialize();
					game->push_back(jnrscr);
				}
				else
				{
					auto* jnrscr = new jnr::JumpnRunScreen(game);
					jnrscr->initialize();
					game->push_back(jnrscr);
				}

				//$P.media.musics.stopBGM();

			}
			else if (cursor == 2)
			{
				$P.sound_stack.push_back($P.snd_enter);
				auto* jnrscr = new jnr::ConfigScreen(game);
				jnrscr->initialize();
				game->push_back(jnrscr);
				auto* title = new Title(game);
				title->initialize();
				game->push_back(title);
				//$P.media.musics.stopBGM();
			}
			$P.media.musics.stopBGM();
			$P.media.musics.resetBGM();
			return eScreenState::Finished;
		}
	}
	//if (media.input.key_x.isDown())
	//	return eScreenState::Finished;

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
void Title::render(BaseMedia& media)
{
	//if (graphics->startRendering())
	if (1)
	{
		BaseGraphics* graphics = media.getGraphics();
		if (!b)
		{
			graphics->draw($P.img_paper, 0, 0, 0, 0, 0, 640, 480, false, 0xffffffff);
			//graphics->draw($P.img_logo, 0, -120, 0, 0, 0, 640, 480, false, 0xffffffff);
			//int logo_y = age >= 80 ? 40 : 40 + age * 4 - 320;
			int logo_x = (640 - 320) / 2;
			int logo_y = 60;
			int logo_w = min(320, max(0, age * 6 - 120));
			//graphics->drawSpinResize($P.img_logo, 0, logo_y, 0, 0, 0, 512, 221, false, 0xffffffff, 0.6, 0.6, 0);
			graphics->draw($P.img_logo, logo_x, logo_y, 0, 0, 0, logo_w, 141, false, 0xffffffff);

			if (logo_w > 0 && logo_w < 320)
			{
				for (int i = 0; i < 30; i++) 
				{
					graphics->draw($P.img_obj, 
						logo_x + logo_w - 2 + $P.rand.get() % 10, logo_y + $P.rand.get() % 141, 0,
						REFPOINT_MINISTAR_X, REFPOINT_MINISTAR_Y, 16, 16);
				}
			}

			if (age > 96)
			{
				graphics->drawString("‚Â‚Ã‚«‚©‚ç", 300, 300, 200, 24);
				graphics->drawString("‚Í‚¶‚ß‚©‚ç", 300, 300 + 40, 200, 24);
				graphics->drawString("Ý’è", 300, 300 + 40 * 2, 200, 24);
				graphics->draw($P.img_cursor, 267, 300 - 6 + 40 * cursor, 0, 0, 0, 40, 80, false);
			}
			if (age > 180)
			{
				graphics->drawString("Press Z to start", 277, 440, 200, 24);
			}
			if (age < 64)
				graphics->clear(255, 255, 255, 255 - age * 4);

		}

		//graphics->endRendering();
	}
}