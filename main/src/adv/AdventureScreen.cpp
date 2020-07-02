#include"adv/AdventureScreen.h"
#include"adv/AdventureObject.h"
#include"jumpnrun/GRAPHICS.h"
#include"jumpnrun/system/Parmanent.h"
#include"utils/string_util.h"
#include<algorithm>
using namespace gfw;
using namespace adv;
using std::min;
using std::max;

#define $V gmtr::Vector2D


std::vector<std::string> AdventureSamples::samples =
{
	""
};


AdventureScreen::AdventureScreen(gfw::Game* game): Screen(game){}

AdventureScreen::~AdventureScreen(){}

bool AdventureScreen::initialize()
{
	text = "";
	speaker_name = "";
	counter = 0;
	timer_wait = 0;
	cursor_text = 0;
	waiting_key = false;
	sb.type = 2;
	speed_text = 2;
	finished = false;
	ask_cursor = -1;
	tl.initialize();

	name2images["chara1"].insert("normal", $P.img_stand);
	name2images["chara1"].insert("smile", $P.img_stand_s);
	name2images["chara1"].insert("astonished", $P.img_stand_h);

	name2images["chara2"].insert("normal", $P.img_stand2);
	name2images["chara2"].insert("smile", $P.img_stand2_s);
	name2images["chara2"].insert("astonished", $P.img_stand2_h);

	name2images["chara3"].insert("normal", $P.img_stand3);
	name2images["chara3"].insert("smile", $P.img_stand3_s);
	name2images["chara3"].insert("astonished", $P.img_stand3_h);

	name2sound["nyu"] = $P.snd_nyu;
	name2sound["nyu2"] = $P.snd_nyu2;
	name2sound["suck"] = $P.snd_suck;

	return true;
}

bool AdventureScreen::initialize(const std::string& str)
{
	text = "";
	speaker_name = "";
	counter = 0;
	timer_wait = 0;
	cursor_text = 0;
	waiting_key = false;
	sb.type = 2;
	speed_text = 2;
	finished = false;
	tl.initialize();
	script.parseFromString(str);
	ask_cursor = -1;
	item_board_visible = false;
	item_borad_refx = item_borad_refy = 0;

	name2images["chara1"].insert("normal", $P.img_stand);
	name2images["chara1"].insert("smile", $P.img_stand_s);
	name2images["chara1"].insert("astonished", $P.img_stand_h);
	name2images["chara1"].insert("hate", $P.img_stand_a);
	name2images["chara1"].insert("eyeclosed", $P.img_stand_e);
	name2images["chara1"].insert("open", $P.img_stand_o);
	name2images["chara1"].insert("disgusted", $P.img_stand_d);
	name2images["chara1"].insert("bashful", $P.img_stand_b);

	name2images["chara2"].insert("normal", $P.img_stand2);
	name2images["chara2"].insert("smile", $P.img_stand2_s);
	name2images["chara2"].insert("astonished", $P.img_stand2_h);
	name2images["chara2"].insert("open", $P.img_stand2_o);

	name2images["chara3"].insert("normal", $P.img_stand3);
	name2images["chara3"].insert("smile", $P.img_stand3_s);
	name2images["chara3"].insert("astonished", $P.img_stand3_h);
	name2images["chara3"].insert("hate", $P.img_stand3_a);
	name2images["chara3"].insert("open", $P.img_stand3_o);

	name2images["chara4"].insert("normal", $P.img_stand4);
	name2images["chara4"].insert("smile", $P.img_stand4_s);
	name2images["chara4"].insert("hate", $P.img_stand4_h);
	name2images["chara4"].insert("open", $P.img_stand4_o);
	name2images["chara4"].insert("astonished", $P.img_stand4_a);

	name2images["chara5"].insert("normal", $P.img_stand5);
	name2images["chara5"].insert("smile", $P.img_stand5_s);
	name2images["chara5"].insert("hate", $P.img_stand5_h);
	name2images["chara5"].insert("open", $P.img_stand5_o);
	name2images["chara5"].insert("astonished", $P.img_stand5_a);

	name2images["chara6"].insert("normal", $P.img_stand6);
	name2images["chara6"].insert("astonished", $P.img_stand6_a);
	name2images["chara6"].insert("astonished2", $P.img_stand6_a2);
	name2images["chara6"].insert("mouseopen", $P.img_stand6_o);
	name2images["chara6"].insert("cry", $P.img_stand6_o2);
	name2images["chara6"].insert("mouseclosed", $P.img_stand6_h);
	name2images["chara6"].insert("smile", $P.img_stand6_s1);
	name2images["chara6"].insert("smile_closed", $P.img_stand6_s2);
	name2images["chara6"].insert("meditating", $P.img_stand6_m);
	name2images["chara6"].insert("meditating_mouse_closed", $P.img_stand6_mo);

	name2images["chara7"].insert("normal", $P.img_stand7);
	name2images["chara7"].insert("smile", $P.img_stand7_s);
	name2images["chara7"].insert("hate", $P.img_stand7_h);
	name2images["chara7"].insert("open", $P.img_stand7_o);
	name2images["chara7"].insert("astonished", $P.img_stand7_a);
	name2images["chara7"].insert("disgusted", $P.img_stand7_d);

	name2images["chara8"].insert("normal", $P.img_stand8);
	name2images["chara8"].insert("smile", $P.img_stand8_s);
	name2images["chara8"].insert("hate", $P.img_stand8_h);
	name2images["chara8"].insert("open", $P.img_stand8_o);
	name2images["chara8"].insert("astonished", $P.img_stand8_a);

	name2images["chara9"].insert("normal", $P.img_stand9);
	name2images["chara9"].insert("open", $P.img_stand9_o);
	name2images["chara9"].insert("astonished", $P.img_stand9_a);
	name2images["chara9"].insert("hate", $P.img_stand9_h);
	name2images["chara9"].insert("eyeclosed", $P.img_stand9_c);
	name2images["chara9"].insert("smile", $P.img_stand9_s);
	//name2images["chara8"].insert("disgusted", $P.img_stand8_d);

	name2sound["nyu"] = $P.snd_nyu;
	name2sound["nyu2"] = $P.snd_nyu2;
	name2sound["suck"] = $P.snd_suck;

	return true;
}

void AdventureScreen::finalize(){}

gfw::Screen::eScreenState AdventureScreen::run(gfw::BaseMedia& media)
{
	sb.run();
	for (auto t = tl.begin(); t != tl.end();)
	{
		Object* obj = getObject(t);
		obj->run();

		if (obj->alive)
			t++;
		else
		{
			delete (Object*)t->workspace;
			t = tl.remove(t);
		}
	}

	if (cursor_text < text.size())
	{
		if (counter % speed_text == 0)
			cursor_text += myutil::getMultiByteCharLength(text[cursor_text]);
		cursor_text = min(cursor_text, (int)text.size());
	}

	if ($P.players[0].vc->Space().isDown())
	{
		//finished = true;

		waiting_key = false;
		timer_wait = 0;
		cursor_text = text.size();
		vars[ask_var_name] = 1; // neutralize questions to avoid battle-again
		ask_cursor = -1;
	}
	if (waiting_key)
	{
		if ($P.players[0].vc->Z().pushed())
		{
			if (cursor_text == text.size())
			{
				waiting_key = false;
				if (ask_cursor >= 0)
					vars[ask_var_name] = ask_cursor;
				ask_cursor = -1;
			}
			else
				cursor_text = text.size();
		}

		if (ask_cursor >= 0)
		{
			if ($P.players[0].vc->Up().pushed())
			{
				ask_cursor = !ask_cursor;
			}
			if ($P.players[0].vc->Down().pushed())
			{
				ask_cursor = !ask_cursor;
			}
			if ($P.players[0].vc->Left().pushed())
			{
				ask_cursor = !ask_cursor;
			}
			if ($P.players[0].vc->Right().pushed())
			{
				ask_cursor = !ask_cursor;
			}
		}
	}
	else if (timer_wait == 0)
	{
		auto command = script.getCurrent();
		std::cout << *command << std::endl;
		if (*command == "summon")
		{
			std::string name = *++command;
			double x = myutil::str2double(*++command);
			double y = myutil::str2double(*++command);
			AdvCharacter *p = generate<AdvCharacter>(tl);
			auto imgs = name2images[name];
			ImageHandler img = imgs["normal"];
			p->initialize(name, img, gmtr::Vector2D(x, y));
			name2chara.insert(std::map<std::string, gfw::Object::Pipe>::value_type(name, Object::Pipe(p, tl)));
			//p->setMotion(4, 0, 40);
		}
		else if (*command == "move")
		{
			std::string name = *++command;
			double x = myutil::str2double(*++command);
			double y = myutil::str2double(*++command);
			int t = myutil::str2int(*++command);
			AdvCharacter *p = static_cast<AdvCharacter *>(name2chara[name].Dst(tl));
			assert(p);
			p->pushMotion(x, y, t);
		}
		else if (*command == "deform")
		{
			std::string name = *++command;
			double x = myutil::str2double(*++command);
			double y = myutil::str2double(*++command);
			int t = myutil::str2int(*++command);
			AdvCharacter *p = static_cast<AdvCharacter *>(name2chara[name].Dst(tl));
			assert(p);
			p->pushDeformation(x, y, t);
		}
		else if (*command == "movedeform")
		{
			std::string name = *++command;
			double x = myutil::str2double(*++command);
			double y = myutil::str2double(*++command);
			double defx = myutil::str2double(*++command);
			double defy = myutil::str2double(*++command);
			int t = myutil::str2int(*++command);
			AdvCharacter *p = static_cast<AdvCharacter *>(name2chara[name].Dst(tl));
			assert(p);
			p->pushMotionDeformation(x, y, defx, defy, t);
		}
		else if (*command == "pos")
		{
			std::string name = *++command;
			double x = myutil::str2double(*++command);
			double y = myutil::str2double(*++command);
			double defx = myutil::str2double(*++command);
			double defy = myutil::str2double(*++command);
			int t = myutil::str2int(*++command);
			AdvCharacter *p = static_cast<AdvCharacter *>(name2chara[name].Dst(tl));
			assert(p);
			p->Phys().p = gmtr::Vector2D(x, y);
			p->setCoef(defx, defy);
		}
		else if (*command == "emote")
		{
			std::string name = *++command;
			std::string emotion = *++command;
			AdvCharacter *p = static_cast<AdvCharacter *>(name2chara[name].Dst(tl));
			assert(name2images.count(name));
			auto imgs = name2images[name];
			//assert(imgs.count(emotion));
			ImageHandler img = imgs[emotion];
			p->setImage(img);
		}
		else if (*command == "speak")
		{
			cursor_text = 0;
			text = *++command;
			if (text == "#")
				text = "";
			else waiting_key = true;
		}
		else if (*command == "ask")
		{
			ask_var_name = *++command;
			cursor_text = 0;
			ask_cursor = 0;
			text = *++command;
			if (text == "#")
				text = "";
			else waiting_key = true;
		}
		else if (*command == "speaker")
		{
			speaker_name = *++command;
			if (speaker_name == "#")
				speaker_name = "";
		}
		else if (*command == "effect")
		{
			std::string name = *++command;
			std::string x = *++command;
			std::string y = *++command;
			std::string refx = *++command;
			std::string refy = *++command;
			AdvCharacter *p = static_cast<AdvCharacter *>(name2chara[name].Dst(tl));
			p->setEffect(myutil::str2int(x), myutil::str2int(y)
				, myutil::str2int(refx), myutil::str2int(refy));

		}
		else if (*command == "itemboard_on")
		{
			std::string refx = *++command;
			std::string refy = *++command;
			this->item_borad_refx = myutil::str2int(refx);
			this->item_borad_refy = myutil::str2int(refy);
			this->item_board_visible = true;

		}
		else if (*command == "itemboard_off")
		{
			this->item_board_visible = false;
		}
		else if (*command == "turnbossdummy")
		{
			int b = myutil::str2int(*++command);
			$P.turn_boss_dummy = (bool)b;
		}
		else if (*command == "turnme")
		{
			int b = myutil::str2int(*++command);
			$P.turn_me = (bool)b;
		}
		else if (*command == "skipboss")
		{
			int b = myutil::str2int(*++command);
			$P.skip_boss = (bool)b;
		}
		else if (*command == "appearbossdummy")
		{
			int b = myutil::str2int(*++command);
			$P.appear_boss_dummy = (bool)b;
		}
		else if (*command == "wait")
		{
			int t = myutil::str2int(*++command);
			timer_wait = t;
		}
		else if (*command == "sound")
		{
			std::string snd = *++command;
			SoundHandler sh = name2sound[snd];
			$P.sound_stack.push_back(sh);
		}
		else if (*command == "bgm")
		{
			std::string snd = *++command;
			std::string intro = *++command;
			std::string start = *++command;
			$P.media.musics.resetBGM();
			$P.media.musics.startBGM(std::string("bgm\\") + snd, 
				myutil::str2double(intro), myutil::str2double(start));
		}
		else if (*command == "if")
		{
			std::string var_name = *++command;
			int val = vars[var_name];
			if (!val)
			{
				int counter = 0;
				while (1)
				{
					script.goNext();
					std::string c = *script.getCurrent();
					if (c == "if")
						counter++;
					else if (c == "else")
					{
						if (counter <= 0)
							break;
					}
					else if (c == "endif")
						counter--;
					if (counter < 0)
						break;
					assert(command != script.end());
				}
			}

		}
		else if (*command == "else")
		{
			int counter = 0;
			while (1)
			{
				script.goNext();
				std::string c = *script.getCurrent();
				if (c == "if")
					counter++;
				else if (c == "endif")
					counter--;
				if (counter < 0)
					break;
				assert(command != script.end());
			}
		}
		else if (*command == "end")
		{
			finished = true;
		}
		if (!script.goNext())
		{
			timer_wait = -1;
		}
	}
	else if (timer_wait > 0)
		timer_wait--;

	counter++;
	if (finished)
		return eScreenState::Finished;
	else return eScreenState::Continuing;
}

void AdventureScreen::render(gfw::BaseMedia& media)
{
	BaseGraphics* graphics = media.getGraphics();
	//graphics->drawString("ADV", 0, 0, 40, 20);

	for (auto t = tl.begin(); t != tl.end(); t++)
	{
		Object* obj = getObject(t);
		obj->render(media);
	}

	sb.render(media);
	auto lines = myutil::split(text.substr(0, cursor_text), "\n");
	graphics->drawString(speaker_name, 35, 363 + 2, 600, 65);
	for (int i = 0; i < lines.size(); i++)
		graphics->drawString(lines[i], 50, 386 + 2 + i * 21, 600, 65);

	if (waiting_key && cursor_text >= text.size())
	{
		if (counter % 60 < 40)
		graphics->drawString("¥", 600, 386 + 2 * 21 + 12, 600, 65, 0, 1, 0);

		if (ask_cursor >= 0)
		{
			//	graphics->drawString("¨", 65, 386 + 2 + (1 + ask_cursor) * 21, 600, 65);
			//	graphics->drawString("‚Í‚¢", 90, 386 + 2 + 1 * 21, 600, 65);
			//	graphics->drawString("‚¢‚¢‚¦", 90, 386 + 2 + 2 * 21, 600, 65);
			graphics->drawString("¨", 65 + ask_cursor * 120, 386 + 2 + 1 * 21 + 6, 600, 65);
			graphics->drawString("‚Í‚¢", 90, 386 + 2 + 1 * 21 + 6, 600, 65);
			graphics->drawString("‚¢‚¢‚¦", 90 + 120, 386 + 2 + 1 * 21 + 6, 600, 65);
		}
	}

	if (item_board_visible) 
	{
		graphics->draw($P.img_obj, 300, 100, 0, 
			REFPOINT_BOARD_X, REFPOINT_BOARD_Y, 96, 96, false);
		graphics->draw($P.img_obj, 300 + 32, 100 + 32, 0,
			item_borad_refx, item_borad_refy, 32, 32, false);
	}

}