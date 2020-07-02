#include"jumpnrun/stage/StageSelectionScreen.h"
#include"jumpnrun/system/JumpnRunScreen.h"
#include"jumpnrun/system/Title.h"
#include"jumpnrun/spell/DeckEditScreen.h"
#include"jumpnrun/system/ConfigScreen.h"
#include"jumpnrun/system/Achievement.h"
#include<future>
using namespace gfw;
using namespace jnr;
using std::max;
using std::min;

bool StageSelectionScreen::initialize()
{
	jnrscr = nullptr;
	cursor = 0;
	age = 0;
	p_current = p_prev = $P.stageselect_cursor;
	timer_move = -1;
	timer_end = -1;

	StagePoint stg1;
	stg1.x = 70;
	stg1.y = 320;
	stg1.stg_file_name = "classroom_start.txt";
	stg1.name = "äwçZÇÃíÜ";
	stg1.start_x = 64;
	stg1.start_y = 64*6;
	stg1.status = Cleared;
	stg1.neighbors.push_back(1);
	stg1.status = $P.getFlag("stg1c") ? Cleared : Uncleared;
	stg1.stage_prefixes = {
		"rouka",
		"class",
		"step",
		"boss_room1",
	};
	stage_point.push_back(stg1);

	StagePoint stg2;
	stg2.x = 180;
	stg2.y = 320;
	stg2.stg_file_name = "ground1.txt";
	stg2.name = "çZíÎÇ©ÇÁí äwòH";
	stg2.start_x = 0;
	stg2.start_y = 64*7;
	stg2.status = Uncleared;
	stg2.neighbors.push_back(5);
	stg2.neighbors.push_back(0);
	stg2.neighbors.push_back(2);
	stg2.neighbors.push_back(0);
	stg2.status = $P.getFlag("stg2c") ? Cleared : Uncleared;
	stg2.status = $P.getFlag("stg1c") ? stg2.status : Invisible;
	stg2.stage_prefixes = {
		"load",
		"ground",
		"building1",
		"underground1",
		"boss_room2",
	};
	stage_point.push_back(stg2);

	StagePoint stg3;
	stg3.x = 300;
	stg3.y = 260;
	stg3.stg_file_name = "forest2.txt";
	stg3.name = "ãﬂèäÇÃêX";
	stg3.start_x = 0;
	stg3.start_y = 64*10;
	stg3.status = Uncleared;
	stg3.neighbors.push_back(3);
	stg3.neighbors.push_back(1);
	stg3.neighbors.push_back(3);
	stg3.neighbors.push_back(1);
	stg3.status = $P.getFlag("stg3c") ? Cleared : Uncleared;
	stg3.status = $P.getFlag("stg2c") ? stg3.status : Invisible;
	stg3.stage_prefixes = {
		"forest",
		"boss_room3",
	};
	stage_point.push_back(stg3);


	StagePoint stg4;
	stg4.x = 346;
	stg4.y = 200;
	stg4.stg_file_name = "downtown1.txt";
	stg4.name = "É_ÉEÉìÉ^ÉEÉì";
	stg4.start_x = 0;
	stg4.start_y = 64 * 8;
	stg4.status = Uncleared;
	stg4.neighbors.push_back(4);
	stg4.neighbors.push_back(2);
	stg4.neighbors.push_back(4);
	stg4.neighbors.push_back(5);
	stg4.status = $P.getFlag("stg4c") ? Cleared : Uncleared;
	stg4.status = $P.getFlag("stg3c") || $P.getFlag("stg32c") ? stg4.status : Invisible;
	stg4.stage_prefixes = {
		"building2",
		"building3",
		"downtown",
		"boss_room4",
	};
	stage_point.push_back(stg4);

	StagePoint stg5;
	stg5.x = 445;
	stg5.y = 165;
	stg5.stg_file_name = "garden1.txt";
	stg5.name = "óHóÏâÆï~";
	stg5.start_x = 0;
	stg5.start_y = 64 * 12;
	stg5.status = Uncleared;
	stg5.neighbors.push_back(6);  // r
	stg5.neighbors.push_back(3);  // l
	stg5.neighbors.push_back(6);  // u
	stg5.neighbors.push_back(3);  //d
	stg5.status = $P.getFlag("stg5c") ? Cleared : Uncleared;
	stg5.status = $P.getFlag("stg4c") ? stg5.status : Invisible;
	stg5.stage_prefixes = {
		"garden",
		"mansion",
		"boss_room5",
	};
	stage_point.push_back(stg5);

	StagePoint stg32;
	stg32.x = 375;
	stg32.y = 305;
	stg32.stg_file_name = "river1.txt";
	stg32.name = "êÏâàÇ¢";
	stg32.start_x = 0;
	stg32.start_y = 64 * 6;
	stg32.status = Uncleared;
	stg32.neighbors.push_back(-1);
	stg32.neighbors.push_back(1);
	stg32.neighbors.push_back(3);
	stg32.neighbors.push_back(1);
	stg32.status = $P.getFlag("stg32c") ? Cleared : Uncleared;
	stg32.status = $P.getFlag("stg2c") ? stg32.status : Invisible;
	stg32.stage_prefixes = {
		"river",
		"boss_room6",
	}; stage_point.push_back(stg32);

	StagePoint stg6;
	stg6.x = 445 + 24;
	stg6.y = 165 - 50;
	stg6.stg_file_name = "boss_room5.txt";
	stg6.name = "óHóÏâÆï~ÇÃç≈âú";
	stg6.start_x = 64 * 2;
	stg6.start_y = 64 * 7;
	stg6.status = Uncleared;
	stg6.neighbors.push_back(-1);
	stg6.neighbors.push_back(7);
	stg6.neighbors.push_back(-1);
	stg6.neighbors.push_back(4);
	stg6.status = $P.getFlag("endwatched") || $P.getFlag("gameclear")
		? Cleared : Uncleared;
	stg6.status = $P.getFlag("m35") ? stg6.status : Invisible;
	stg6.stage_prefixes = {
		"boss_room5.txt",
		"mansion4",
		"mansion5",
		"mansion6",
	};
	stage_point.push_back(stg6);

	StagePoint stg7;
	stg7.x = 270;
	stg7.y = 120;
	stg7.stg_file_name = "fjtbroom.txt";
	stg7.name = "Ç”Ç∂Ç¬Ç⁄ÇÃâ∆";
	stg7.start_x = 64 * 5;
	stg7.start_y = 64 * 3;
	stg7.status = Uncleared;
	stg7.neighbors.push_back(6);  // r
	stg7.neighbors.push_back(7);  // l
	stg7.neighbors.push_back(7);  // u
	stg7.neighbors.push_back(6);  // d
	stg7.status = $P.getFlag("endwatched") || $P.getFlag("gameclear") ? Cleared : Invisible;
	stg7.stage_prefixes = {
		"fjtbroom",
	};
	stage_point.push_back(stg7);


	$P.in_stage = false;
	if($P.to_save)
		$P.save($P.save_file_name);

	return true;
}
void StageSelectionScreen::finalize()
{
	//if (!jnrscr)
	//	jnrscr = future_jnrscr.get();
	//delete jnrscr;
}

Screen::eScreenState StageSelectionScreen::run(gfw::BaseMedia& media)
{
	age++;
	if (age == 90 && timer_end < 0)
	{
		$P.media.musics.setAutoRepeat(true);
		$P.reset_bgm = true;
		$P.start_bgm = true;
		$P.bgm_name = "stageselect.mp3";
	}
	if (!controls.empty())
	{
		auto ctrl = controls.front();
		eControl e = ctrl->run();
		if (!ctrl->Active())
		{
			delete ctrl;
			controls.pop_front();
		}
		switch (e)
		{
		default:
			break;
		case eControl::ToTitle:
			if (true)
			{
				Title* title = new Title(game);
				title->initialize();
				game->push_back(title);
			}
			return eScreenState::Finished;
			break;
		case eControl::ToDeckEdit:
			if (true)
			{
				spl::DeckEditScreen* title = new spl::DeckEditScreen(game);
				title->initialize();
				controls.push_back(new OtherScreen(media, title));
				controls.push_back(new PAUSE($P.players[0].vc));
			}
			return eScreenState::Continuing;
			break;
		case eControl::ToConfig:
			if (true)
			{
				ConfigScreen* title = new ConfigScreen(game);
				title->initialize();
				controls.push_back(new OtherScreen(media, title));
				controls.push_back(new PAUSE($P.players[0].vc));
			}
			return eScreenState::Continuing;
			break;
		case eControl::ToAchievement:
			if (true)
			{
				AchievementScreen* title = new AchievementScreen(game);
				title->initialize();
				controls.push_back(new OtherScreen(media, title));
				controls.push_back(new PAUSE($P.players[0].vc));
			}
			return eScreenState::Continuing;
			break;
		}
	}
	else
	{
		if (timer_move >= 0)
			timer_move--;
		if (timer_end >= 0)
			timer_end--;
		if (timer_wink >= 0)
			timer_wink--;
		if (($P.rand.get() % (60 * 40)) == 0)
			timer_wink = 45;

		$P.in_stage = false;
		if (timer_end == 1)
		{
			//jnrscr = future_jnrscr.get();
			jnrscr = new JumpnRunScreen(game);
			jnrscr->initialize();

			game->push_back(jnrscr);
			$P.in_stage = true;
			$P.reset_bgm = true;
			return eScreenState::Finished;
		}

		if ($P.players[0].vc->Space().pushed())
		{
			controls.push_back(new PAUSE($P.players[0].vc));
		}

		if (timer_move < 0)
		{
			if ($P.players[0].vc->Z().pushed())
			{
				if (true)
				{

					//$P.players[0].hp_current = p->HP();
					// if the previous stage is in the node, keep staying the stage
					bool stay = false;
					for(auto& prefix: stage_point[p_current].stage_prefixes)
					{
						if ($P.next_stage_name.substr(0, prefix.length()) == prefix) 
						{
							stay = true;
							break;
						}
					}
					if (!stay)
					{
						$P.next_stage_name = stage_point[p_current].stg_file_name;
						$P.next_start_p = gmtr::Vector2D(stage_point[p_current].start_x, stage_point[p_current].start_y);
					}

					$P.players[0].hp_current = $P.players[0].hp_max;
					$P.players[0].mp_current = $P.players[0].mp_max;

					/*future_jnrscr = std::async(std::launch::async, [&]
					{
						auto* rtn = new JumpnRunScreen(game);
						rtn->initialize();
						return rtn;
					});*/
					timer_end = 60;
				}
				//return eScreenState::Finished;
			}

			if ($P.players[0].vc->Right().isDown())
			{
				if (stage_point[p_current].neighbors.size() >= 1
					&& stage_point[p_current].neighbors[0] >= 0
					&& stage_point[stage_point[p_current].neighbors[0]].status != Invisible)
				{
					p_prev = p_current;
					p_current = stage_point[p_current].neighbors[0];
					timer_move = 60;
				}
			}
			if ($P.players[0].vc->Left().isDown())
			{
				if (stage_point[p_current].neighbors.size() >= 2
					&& stage_point[p_current].neighbors[1] >= 0
					&& stage_point[stage_point[p_current].neighbors[1]].status != Invisible)
				{
					p_prev = p_current;
					p_current = stage_point[p_current].neighbors[1];
					timer_move = 60;
				}
			}
			if ($P.players[0].vc->Up().isDown())
			{
				if (stage_point[p_current].neighbors.size() >= 3
					&& stage_point[p_current].neighbors[2] >= 0
					&& stage_point[stage_point[p_current].neighbors[2]].status != Invisible)
				{
					p_prev = p_current;
					p_current = stage_point[p_current].neighbors[2];
					timer_move = 60;
				}
			}
			if ($P.players[0].vc->Down().isDown())
			{
				if (stage_point[p_current].neighbors.size() >= 4
					&& stage_point[p_current].neighbors[3] >= 0
					&& stage_point[stage_point[p_current].neighbors[3]].status != Invisible)
				{
					p_prev = p_current;
					p_current = stage_point[p_current].neighbors[3];
					timer_move = 60;
				}
			}

			$P.stageselect_cursor = p_current;
		}
	}

	return eScreenState::Continuing;
}
void StageSelectionScreen::render(BaseMedia& media)
{
	//if (graphics->startRendering())
	if (1)
	{
		BaseGraphics* graphics = media.getGraphics();
		graphics->draw($P.img_stageselectmap, 0, 0, 0, 0, 0, 640, 480, false, 0xffffffff);
		//graphics->draw($P.img_logo, 0, -120, 0, 0, 0, 640, 480, false, 0xffffffff);
	
		for (int i = 0; i < stage_point.size(); i++)
		{
			for (int n : stage_point[i].neighbors)
			{
				if (n > i && stage_point[i].status != Invisible  && stage_point[n].status != Invisible)
				{
					graphics->drawLine(stage_point[i].x + 24 - 2, stage_point[i].y + 13
						, stage_point[n].x + 24 - 2, stage_point[n].y + 13
						, 64, 64, 64, 255, 4);
				}
			}
			if (stage_point[i].status != Invisible)
			{
				int refx = stage_point[i].status == Cleared ? 0: 48;
				graphics->draw($P.img_stageselectpanel, stage_point[i].x, stage_point[i].y, 0
					, refx, 0, 48, 26);
			}
		}

		double a = timer_move < 0 ? 0 : (double)timer_move / 60;
		double px = stage_point[p_current].x * (1 - a) + stage_point[p_prev].x * a - 160 + 28 + 2;
		double py = stage_point[p_current].y * (1 - a) + stage_point[p_prev].y * a - 240 - 48 -4
			+ 1 * sin(((double)(age % 90) / 90 - 0.5) * 3.14 * 2);
		ImageHandler ih = timer_wink > 0 ? $P.img_stand_e : $P.img_stand;
		graphics->drawSpinResize(ih
			, px, py, 0
			, 0, 0, 320, 480, false, 0xffffffff, 0.4, 0.4, 0);

		//graphics->drawSpinResize($P.img_stand, stage_point[p_current].x - 160*0.33, stage_point[p_current].y - 480*0.33, 0
		//	, 0, 0, 320, 480, false, 0xffffffff, 0.33, 0.33, 0);

		graphics->drawString(stage_point[p_current].name, 20, 450, 200, 20);

		if (age < 64)
			graphics->clear(255, 255, 255, 255 - age * 4);

		if (!controls.empty())
		{
			controls.front()->render(media);
			//graphics->clear(0, 0, 0, 64);
		}

		if (timer_end >= 0)
		{
			graphics->clear(0, 0, 0, 255 - min(timer_end * 255 / 60, 255));
		}
	}
}
