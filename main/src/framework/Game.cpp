#include"framework/Game.h"
#include"jumpnrun/system/Endroll.h"
#include"jumpnrun/system/Title.h"
#include"framework/TaskTest.h"
#include"jumpnrun/system/JumpnRunScreen.h"
#include"jumpnrun/ai/AITestScreen.h"
#include"adv/AdventureScreen.h"
#include"jumpnrun/ai/AITrainingScreen.h"
#include"jumpnrun/system/Parmanent.h"
using namespace gfw;

using std::min;
using std::max;

Game::Game(){}

bool Game::initialize()
{
	bool result = true;
	//screens.push_back(new Title());
	//screens.push_back(new TaskTest());

	//jnr::ai::AITrainingScreen* jnrscr = new jnr::ai::AITrainingScreen(this);
	/*jnr::AITestScreen* jnrscr = new jnr::AITestScreen(this);
	jnr::ai::AIParameters param = jnr::ai::AIParameters::random();
	jnrscr->initialize(param);
	$P.ai.initialize();*/

	Title* jnrscr = new Title(this);
	//Endroll* jnrscr = new Endroll(this);
	//jnr::JumpnRunScreen* jnrscr = new jnr::JumpnRunScreen(this);
	//adv::AdventureScreen* jnrscr = new adv::AdventureScreen(this);
	jnrscr->initialize();

 	screens.push_back(jnrscr);

	$P.recorder.clear();
	//$P.save(std::string("replay\\") + $P.start_time + "\\savedata.txt");
	initialized = true;
	age = 0;
	return result;
}

void Game::finalize()
{
	if ($P.to_save)
	{
		$P.recorder.writeFile(std::string("replay\\") + $P.start_time + "\\controller.txt");
	}
	for (auto scr : screens)
		scr->finalize();
}

Game::eGameState Game::run(BaseMedia& media)
{
	age++;

	if(!initialized)
		return eGameState::Continuing;
	if (screens.empty())
		return eGameState::Continuing;
	Screen& screen = *(screens.front());
	Screen::eScreenState state = screen.run(media);
	jnr::Parmanent::Instance().keycont.poll();
#ifdef GFW_WIN32
	jnr::Parmanent::Instance().joycont.poll();
#endif
	jnr::Parmanent::Instance().andcont.poll();
	$P.replaycont.poll();
	$P.recorder.push(
	{
		$P.andcont.Z().isDown(),
		$P.andcont.X().isDown(),
		$P.andcont.C().isDown(),
		$P.andcont.A().isDown(),
		$P.andcont.S().isDown(),
		$P.andcont.Up().isDown(),
		$P.andcont.Right().isDown(),
		$P.andcont.Down().isDown(),
		$P.andcont.Left().isDown(),
		$P.andcont.Space().isDown(),
	});

	if ($P.sound_on)
	{
		while (!$P.sound_stack.empty())
		{
			$P.media.sounds.play($P.sound_stack.front());
			$P.sound_stack.pop_front();
		}
	}

	if (state == Screen::eScreenState::Finished)
	{
		screen.finalize();
		delete screens.front();
		screens.pop_front();
		if (screens.empty())
			return eGameState::Finished;
	}

	/*while (1)
	{
		jnr::JumpnRunScreen* jnrscr = new jnr::JumpnRunScreen(this);
		jnrscr->initialize();
		Screen::eScreenState state = jnrscr->run(media);
		//delete jnrscr;
	}*/

	return eGameState::Continuing;
}
void Game::render(BaseMedia& media)
{
	if (screens.empty())
		return;
	Screen& screen = *(screens.front());
	BaseGraphics* graphics = media.getGraphics();
	if (1)
	{
		graphics->setRenderingTarget($P.img_screen);
		//graphics->clear(0, 0, 0, 255);
		screen.render(media);

		graphics->setRenderingTargetToBackBuffer();
		graphics->clear(0, 0, 0, 255);
		//graphics->draw($P.img_screen, 0, 0, .0, 0, 0, 640, 480
		//	, false, 0xffffffff);
		//graphics->clear(255, 255, 255, 255);
		int w = $P.window_w;
		int h = $P.window_h;
		int cw = $P.current_window_w;
		int ch = $P.current_window_h;
		double rx = (double)cw / w;
		double ry = (double)ch / h;
		double r = min(rx, ry);
		double ar = rx / ry;
		int margin_x = 0;
		int margin_y = 0;
		//int margin_x = $P.window_w * (rx - r) / 4;
		//int margin_y = $P.window_h * (ry - r) / 4;
		//if (1)
		if (r > 1.0)
		{
			//double magx = rx < ry ? 1.0 : ry / rx;
			//double magy = ry < rx ? 1.0 : rx / ry;
			double magx = r;
			double magy = r;
			if (rx > ry)
				margin_x = w * (rx - ry) / 2;
			if (ry > rx)
				margin_y = w * (ry - rx) / 2;
			/*graphics->drawSpinResizeNoAlpha($P.img_screen, ($P.current_window_w - $P.window_w) / 2 - margin_x
				, ($P.current_window_h - $P.window_h) / 2 - margin_y, 0
				, 0, 0, $P.window_w, $P.window_h, true, 0xffffffff, 1.0, 1.0, 0);*/
			graphics->drawSpinResizeNoAlpha($P.img_screen
				, w * (r - 1) / 2 + margin_x
				, h * (r - 1) / 2 + margin_y
				, 0
				, 0, 0, w, h, true, 0xffffffff, magx, magy, 0);

			int mx = $P.mouse_x;
			int my = $P.mouse_y;
			graphics->drawString("+", mx, my, 12, 12,
				1, 0, 0, 1);
		}
		else
		{
			graphics->drawSpinResizeNoAlpha($P.img_screen, - margin_x
				, - margin_y, 0
				, 0, 0, $P.window_w, $P.window_h, true, 0xffffffff, 1.0, 1.0, 0);
		}
	}
	else
	{
		graphics->clear(0, 0, 0, 255);
		screen.render(media);
	}
	//graphics->clear(0, 0, 0, 255);
	//graphics->clear(255, 0, 0, 128);
}
