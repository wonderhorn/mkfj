#include"jumpnrun/system/ObjectManager.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/mover/enemy/Enemy.h"
#include"jumpnrun/mover/Item.h"
#include"utils/string_util.h"
#include"jumpnrun/GRAPHICS.h"
#include<limits.h>
using namespace gfw;
#define $V(x, y) gmtr::Vector2D((x), (y))
ObjectManager::ObjectManager()
: active_x(INT_MIN), active_y(INT_MIN), active_w(INT_MAX), active_h(INT_MAX), margin(96), tl(nullptr)
{}

ObjectManager::ObjectManager(Tasklist* tl)
: active_x(INT_MIN), active_y(INT_MIN), active_w(INT_MAX), active_h(INT_MAX), margin(96), tl(tl)
{}

void ObjectManager::pushObject(int x, int y, std::string str, std::vector<double> args)
{
	pipes.push_back(Object::Pipe());
	objects.push_back(std::tuple<gmtr::Vector2D, std::string, std::vector<double>>
		(gmtr::Vector2D(x, y), str, args));
}

static void generateFromName(const std::string& str
	, const gmtr::Vector2D& v, Tasklist* tl, Object::Pipe& pipe
	, const std::vector<double>& args)
{
	using namespace jnr;
	if (str == "boy")
	{
		enm::Boy* ptr = generate<enm::Boy>(*tl);
		ptr->initialize(REFPOINT_BOY_X, REFPOINT_BOY_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "salaryman")
	{
		enm::Boy* ptr = generate<enm::Boy>(*tl);
		ptr->initialize(REFPOINT_SALARYMAN_X, REFPOINT_SALARYMAN_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "girl1")
	{
		enm::Boy* ptr = generate<enm::Boy>(*tl);
		ptr->initialize(REFPOINT_GIRL1_X, REFPOINT_GIRL1_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "girl2")
	{
		enm::Boy* ptr = generate<enm::Boy>(*tl);
		ptr->initialize(REFPOINT_GIRL2_X, REFPOINT_GIRL2_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "girl3")
	{
		enm::Boy* ptr = generate<enm::Boy>(*tl);
		ptr->initialize(REFPOINT_GIRL3_X, REFPOINT_GIRL3_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "girl4")
	{
		enm::Boy* ptr = generate<enm::Boy>(*tl);
		ptr->initialize(REFPOINT_GIRL4_X, REFPOINT_GIRL4_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "girl5")
	{
		enm::Boy* ptr = generate<enm::Boy>(*tl);
		ptr->initialize(REFPOINT_GIRL5_X, REFPOINT_GIRL5_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}

	if (str == "kendo")
	{
		enm::Kendo* ptr = generate<enm::Kendo>(*tl);
		ptr->initialize(REFPOINT_KENDO_X, REFPOINT_KENDO_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "volley")
	{
		enm::Volley* ptr = generate<enm::Volley>(*tl);
		ptr->initialize(REFPOINT_VOLLEYGIRL_X, REFPOINT_VOLLEYGIRL_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "bicycle")
	{
		enm::Bicycle* ptr = generate<enm::Bicycle>(*tl);
		ptr->initialize(REFPOINT_BICYCLE_X, REFPOINT_BICYCLE_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "car")
	{
		enm::Car* ptr = generate<enm::Car>(*tl);
		ptr->initialize(REFPOINT_CAR_X, REFPOINT_CAR_Y, 96, 64, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "car2")
	{
		enm::Car* ptr = generate<enm::Car>(*tl);
		ptr->initialize(REFPOINT_CAR_X + 96, REFPOINT_CAR_Y, 96, 64, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "longcar")
	{
		enm::Car* ptr = generate<enm::Car>(*tl);
		ptr->initialize(REFPOINT_LONGCAR_X, REFPOINT_LONGCAR_Y, 160, 64, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "bee")
	{
		enm::Bee* ptr = generate<enm::Bee>(*tl);
		ptr->initialize(REFPOINT_BEE_X, REFPOINT_BEE_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "mushroom")
	{
		enm::Mushroom* ptr = generate<enm::Mushroom>(*tl);
		ptr->initialize(REFPOINT_MUSHROOM_X, REFPOINT_MUSHROOM_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "frog")
	{
		enm::Frog* ptr = generate<enm::Frog>(*tl);
		ptr->initialize(REFPOINT_FROG_X, REFPOINT_FROG_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "snail")
	{
		enm::Snail* ptr = generate<enm::Snail>(*tl);
		ptr->initialize(REFPOINT_SNAIL_X, REFPOINT_SNAIL_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "bug")
	{
		enm::Bug* ptr = generate<enm::Bug>(*tl);
		ptr->initialize(REFPOINT_BUG_BLACK_X, REFPOINT_BUG_BLACK_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "dustboxman")
	{
		enm::Dustboxman* ptr = generate<enm::Dustboxman>(*tl);
		ptr->initialize(REFPOINT_DUSTBOXMAN_X, REFPOINT_DUSTBOXMAN_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "pylonman")
	{
		enm::Pylonman* ptr = generate<enm::Pylonman>(*tl);
		ptr->initialize(REFPOINT_PYLONMAN_X, REFPOINT_PYLONMAN_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "cannon")
	{
		enm::Cannon* ptr = generate<enm::Cannon>(*tl);
		ptr->initialize(REFPOINT_CANNON_X, REFPOINT_CANNON_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "mandrake")
	{
		enm::Mandragora* ptr = generate<enm::Mandragora>(*tl);
		ptr->initialize(REFPOINT_MANDRAKE_X, REFPOINT_MANDRAKE_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "goblin")
	{
		enm::Goblin* ptr = generate<enm::Goblin>(*tl);
		ptr->initialize(REFPOINT_GOBLIN_X, REFPOINT_GOBLIN_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "researcher")
	{
		enm::Researcher* ptr = generate<enm::Researcher>(*tl);
		ptr->initialize(REFPOINT_RESEARCHER_X, REFPOINT_RESEARCHER_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "elf")
	{
		enm::Elf* ptr = generate<enm::Elf>(*tl);
		ptr->initialize(REFPOINT_ELF_X, REFPOINT_ELF_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "gargoyle")
	{
		enm::Gargoyle* ptr = generate<enm::Gargoyle>(*tl);
		ptr->initialize(REFPOINT_GARGOYLE_X, REFPOINT_GARGOYLE_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "pumpkin")
	{
		enm::Pumpkin* ptr = generate<enm::Pumpkin>(*tl);
		ptr->initialize(REFPOINT_PUMPKIN_X, REFPOINT_PUMPKIN_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "candle")
	{
		enm::Candle* ptr = generate<enm::Candle>(*tl);
		ptr->initialize(REFPOINT_CANDLE_X, REFPOINT_CANDLE_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "hand")
	{
		enm::Hand* ptr = generate<enm::Hand>(*tl);
		ptr->initialize(REFPOINT_HAND_X, REFPOINT_HAND_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "fairy_blue")
	{
		enm::Fairy* ptr = generate<enm::Fairy>(*tl);
		ptr->initialize(REFPOINT_FAIRY_BLUE_X, REFPOINT_FAIRY_BLUE_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "fish")
	{
		enm::Fish* ptr = generate<enm::Fish>(*tl);
		ptr->initialize(REFPOINT_FISH_X, REFPOINT_FISH_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "jellyfish")
	{
		enm::JellyFish* ptr = generate<enm::JellyFish>(*tl);
		ptr->initialize(REFPOINT_JELLYFISH_X, REFPOINT_JELLYFISH_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str == "urchin")
	{
		enm::Urchin* ptr = generate<enm::Urchin>(*tl);
		ptr->initialize(REFPOINT_URCHIN_X, REFPOINT_URCHIN_Y, v, 1);
		pipe = Object::Pipe(ptr, *tl);
	}
	if (str.size() == 4 && str.substr(0, 4) == "trap") // not used now
	{
		int refx = args[0];
		int refy = args[1];
		int period = args[2];
		//auto t = myutil::split(str, " ");
		if (period >= 0)
		{

			enm::TrapTimered* ptr = generate<enm::TrapTimered>(*tl);
			ptr->initialize(refx, refy, v, 1, 10, period);
			pipe = Object::Pipe(ptr, *tl);
		}
		else
		{
			enm::Trap* ptr = generate<enm::Trap>(*tl);
			ptr->initialize(refx, refy, v, 1, 10);
			pipe = Object::Pipe(ptr, *tl);
		}
	}
	if (str.substr(0, 8) == "trap_red") // not used now
	{
		int refx = args[0];
		int refy = args[1];
		int period = args[2];
		//auto t = myutil::split(str, " ");
		if (period >= 0)
		{

			enm::TrapTimered* ptr = generate<enm::TrapTimered>(*tl);
			ptr->initialize(refx, refy, v, 1, 25, period);
			pipe = Object::Pipe(ptr, *tl);
		}
		else
		{
			enm::Trap* ptr = generate<enm::Trap>(*tl);
			ptr->initialize(refx, refy, v, 1, 25);
			pipe = Object::Pipe(ptr, *tl);
		}
	}
	if (str == "jewel")
	{
		Item* ptr = generate<Item>(*tl);
		ptr->initialize(REFPOINT_JEWEL_X, REFPOINT_JEWEL_Y + 32, v, -1, 20, $V(0, 0), 0);
		pipe = Object::Pipe(ptr, *tl);
	}

}

void ObjectManager::generateNear()
{
	for (int i = 0; i < objects.size(); i++)
	{
		auto ptr = pipes[i].Dst(*tl);
		if (ptr)
		{
			if (ptr->alive)
			{
				auto v = ptr->Phys().p;
				//bool b = v.x >= active_x - margin && v.x <= active_x + active_w + margin
				//	&&  v.y >= active_y - margin && v.y <= active_y + active_h + margin;
				bool b = v.x >= active_x - margin*2 && v.x <= active_x + active_w + margin*2
					&&  v.y >= active_y - margin*2 && v.y <= active_y + active_h + margin*2;

				if (!b)
					ptr->alive = false;
			}
			else
				pipes[i] = Object::Pipe();
		}
		else
		{
			auto v = std::get<0>(objects[i]);
			std::string str = std::get<1>(objects[i]);
			bool b = v.x >= active_x && v.x <= active_x + active_w
				&&  v.y >= active_y && v.y <= active_y + active_h;
			bool b2 = v.x >= active_x - margin * 2 && v.x <= active_x + active_w + margin * 2
				&& v.y >= active_y - margin * 2 && v.y <= active_y + active_h + margin * 2;
			if (b2)
			{
				using namespace jnr;
				generateFromName(str, v, tl, pipes[i], std::get<2>(objects[i]));
			}
		}
	}
}

void ObjectManager::run()
{
	for (int i = 0; i < objects.size(); i++)
	{
		auto ptr = pipes[i].Dst(*tl);
		if (ptr)
		{
			if (ptr->alive)
			{
				auto v = ptr->Phys().p;
				bool b = v.x >= active_x - margin*2 && v.x <= active_x + active_w + margin*2
					&&  v.y >= active_y - margin*2 && v.y <= active_y + active_h + margin*2;
				if (!b)
					ptr->alive = false;
			}
			else
				pipes[i] = Object::Pipe();
		}
		else
		{
			auto v = std::get<0>(objects[i]);
			std::string str = std::get<1>(objects[i]);
			/*bool b = v.x >= active_x && v.x <= active_x + active_w
				&&  v.y >= active_y && v.y <= active_y + active_h;
			bool b2 = v.x >= active_x && v.x <= active_x + active_w + margin
				&& v.y >= active_y - margin && v.y <= active_y + active_h + margin ;*/
			bool b = v.x > active_x - margin && v.x < active_x + active_w + margin
				&&  v.y > active_y - margin  && v.y < active_y + active_h + margin;
			bool b2 = v.x >= active_x - margin*2 && v.x <= active_x + active_w + margin*2
				&&  v.y >= active_y - margin*2 && v.y <= active_y + active_h + margin*2;
			if (!b && b2)
			{
				using namespace jnr;
				generateFromName(str, v, tl, pipes[i], std::get<2>(objects[i]));
			}
		}
	}
}
void ObjectManager::setActiveWindow(int x, int y, int w, int h)
{
	active_x = x;
	active_y = y;
	active_w = w;
	active_h = h;
}