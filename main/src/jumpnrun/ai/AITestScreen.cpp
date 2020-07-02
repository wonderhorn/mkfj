#include"jumpnrun/ai/AITestScreen.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/mover/enemy/Enemy.h"
#include"jumpnrun/mover/Item.h"
#include"jumpnrun/mover/SystemObject.h"
#include"jumpnrun/GRAPHICS.h"
#include"utils/string_util.h"
#include<iostream>
#include<time.h>
#include<limits.h>
using namespace jnr;
using namespace gfw;
#define $V gmtr::Vector2D

AITestScreen::AITestScreen(gfw::Game* game) : Screen(game), ai_fitness(INT_MIN), ai_test(), time_limit(60 * 60 * 10), name("noname"){}

AITestScreen::~AITestScreen(){}

bool AITestScreen::initialize()
{
	isRendered = true;
	paused = false;
	bool result;
	result = tl.initialize();
	stg.initializeTestStage();
	Mover::stg = &stg;
	Object::tl = &tl;

	//generate<Player>(tl)->initialize(REFPOINT_ME_X, REFPOINT_ME_Y, $V(0, 0), 0);
	AIPlayer* aip = generate<AIPlayer>(tl);
	//aip->initialize(REFPOINT_ME_X, REFPOINT_ME_Y, $V(0, 0), 2, &ai_test);
	//aip->initialize(REFPOINT_ME_X, REFPOINT_ME_Y, $V(0, 0), 2, &$P.ai);
	player = Object::Pipe(aip, tl);
	generate<Item>(tl)->initialize(REFPOINT_JEWEL_X, REFPOINT_JEWEL_Y + 32, $V(64 * 6, 0), -1, 10, $V(0, 0), 1);
	generate<ItemPutter>(tl)->initialize();

	std::cout << "ai test started" << std::endl;

	return result;
}
bool AITestScreen::initialize(const ai::AIParameters& param)
{
	bool result = initialize();
	ai_test.initialize();
	return result;
}

void AITestScreen::finalize()
{
	int t = ::time(nullptr);
	$P.aisuperviser.save("AISupervision\\" + myutil::int2str(t) + ".csv");
	$P.aisuperviser.clear();
	std::cout << "ai test finished" << std::endl;

	jnr::AITestScreen* jnrscr = new jnr::AITestScreen(this->game);
	jnr::ai::AIParameters param = jnr::ai::AIParameters::random();
	jnrscr->initialize(param);
	$P.ai.initialize();
	game->push_back(jnrscr);
}

gfw::Screen::eScreenState AITestScreen::run(gfw::Media& media)
{
	using namespace gmtr;
	Parmanent::Instance().keycont.poll();
	if (paused)
	{
		if ($P.keycont.Space().pushed())
		{
			paused = false;
			if ($P.keycont.Z().pushed())
			{
				/*if (game)
					game->push_back(new JumpnRunScreen(game));*/
				return eScreenState::Finished;
			}
			if ($P.keycont.X().pushed())
			{
				return eScreenState::Finished;
			}
		}
	}
	else
	{
		if ($P.keycont.Space().pushed())
		{
			paused = true;
		}

		stg.run();

		if ($P.keycont.Right().isDown())
		$P.camera.setPos($P.camera.X() + 1, $P.camera.Y());
		if ($P.keycont.Left().isDown())
		$P.camera.setPos($P.camera.X() - 1, $P.camera.Y());


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
		counter++;
		if (player.Dst(tl))
			ai_fitness = ((AIPlayer*)player.Dst(tl))->HP();
	}

	if (counter >= time_limit)
	{
		OutputDebugString(std::string("ai test finished").c_str());
		OutputDebugString((std::string("ai fitness: ") + myutil::int2str(ai_fitness)).c_str());
		return eScreenState::Finished;
	}
	else return eScreenState::Continuing;
}

void AITestScreen::render(gfw::Media& media)
{
	if (!isRendered)
		return;
	//media.graphics.draw(Parmanent::Instance().img_map, 0, 0, 0, 0, 0, 960, 320);
	//bg.render(media);
	stg.render(media);
	for (auto t = tl.begin(); t != tl.end(); t++)
	{
		Object* obj = getObject(t);
		obj->render(media);
	}

	if (paused)
	{
		media.graphics.clear(0, 0, 0, 64);
	}
}