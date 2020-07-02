#include"jumpnrun/ai/AIEvaluateScreen.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/mover/enemy/Enemy.h"
#include"jumpnrun/mover/Item.h"
#include"jumpnrun/mover/SystemObject.h"
#include"jumpnrun/stage/StageDataLoader.h"
#include"jumpnrun/GRAPHICS.h"
#include"utils/string_util.h"
#include<iostream>
#include<time.h>
#include<limits.h>
using namespace jnr;
using namespace gfw;
#define $V gmtr::Vector2D

AIEvaluateScreen::AIEvaluateScreen(gfw::Game* game)
: Screen(game), ai_fitness(INT_MIN), time_limit(60 * 180), name("noname"){}

AIEvaluateScreen::~AIEvaluateScreen(){}

bool AIEvaluateScreen::initialize(ai::AI* ai)
{
	isRendered = false;
	paused = false;
	bool result;
	data.pullFromParmanent();
	result = tl.initialize();
	objmng = gfw::ObjectManager(&tl);
	if (stg_name.size() == 0)
		stg.initializeTestStage();
	else
	{
		StageDataLoader().load(stg_name, &tl, stg, objmng);
		//stg.StartPoint($P.next_start_p);
	}
	Mover::stg = &stg;
	Object::tl = &tl;

	//generate<Player>(tl)->initialize(REFPOINT_ME_X, REFPOINT_ME_Y, $V(0, 0), 0);
	AIPlayer* aip = generate<AIPlayer>(tl);
	//aip->initialize(REFPOINT_ME_X, REFPOINT_ME_Y, $V(0, 0), 2, &ai_test);
	aip->initialize(REFPOINT_ME_X, REFPOINT_ME_Y, stg.StartPoint(), 2, false, ai);
	player = Object::Pipe(aip, tl);
	generate<Item>(tl)->initialize(REFPOINT_JEWEL_X, REFPOINT_JEWEL_Y + 32, $V(64 * 6, 0), -1, 10, $V(0, 0), 1);
	generate<ItemPutter>(tl)->initialize();
	$P.camera.setPos(aip->P().x - 320, aip->P().y - 200);

	std::cout << "ai test started" << std::endl;

	return result;
}

bool AIEvaluateScreen::initialize()
{
	isRendered = true;
	paused = false;
	bool result;
	result = tl.initialize();
	if (stg_name.size() == 0)
		stg.initializeTestStage();
	else
	{
		StageDataLoader().load(stg_name, &tl, stg, objmng);
		stg.StartPoint($P.next_start_p);
	}
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

void AIEvaluateScreen::finalize()
{
	int t = ::time(nullptr);
	//$P.aisuperviser.save("AISupervision\\" + myutil::int2str(t) + ".csv");
	//$P.aisuperviser.clear();
	std::cout << "ai test finished" << std::endl;
	std::cout << "ai score: " << ai_fitness << std::endl;

	/*jnr::AITestScreen* jnrscr = new jnr::AITestScreen(this->game);
	jnr::ai::AIParameters param = jnr::ai::AIParameters::random();
	jnrscr->initialize(param);
	$P.ai.initialize();
	game->push_back(jnrscr);*/
}

gfw::Screen::eScreenState AIEvaluateScreen::run(gfw::BaseMedia& media)
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

		stg.run(data);

		if ($P.keycont.Right().isDown())
			$P.camera.setPos($P.camera.X() + 1, $P.camera.Y());
		if ($P.keycont.Left().isDown())
			$P.camera.setPos($P.camera.X() - 1, $P.camera.Y());


		for (auto t = tl.begin(); t != tl.end();)
		{
			Object* obj = getObject(t);
			if (obj->isMover)
				(static_cast<Mover*>(obj))->run(data);
			else
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
		{
			AIPlayer* ptr = (AIPlayer*)player.Dst(tl);
			ai_fitness = ptr->HP() + ptr->Score();
			ptr->setXSpell($P.players[0].deck.current_card);

			auto myp = ptr->P();
			$P.camera.chase(myp.x - 320, myp.y - 200);
		}
	}

	if (counter >= time_limit)
	{
		OutputDebugString(std::string("ai test finished").c_str());
		OutputDebugString((std::string("ai fitness: ") + myutil::int2str(ai_fitness)).c_str());
		return eScreenState::Finished;
	}
	else return eScreenState::Continuing;
}

void AIEvaluateScreen::render(gfw::BaseMedia& media)
{
	if (!isRendered)
		return;
	BaseGraphics* graphics = media.getGraphics();
	//graphics->draw(Parmanent::Instance().img_map, 0, 0, 0, 0, 0, 960, 320);
	//bg.render(media);
	stg.render(media, data);
	for (auto t = tl.begin(); t != tl.end(); t++)
	{
		Object* obj = getObject(t);
		if (obj->isMover)
			(static_cast<Mover*>(obj))->render(media, data);
	}

	if (paused)
	{
		graphics->clear(0, 0, 0, 64);
	}
	//graphics->clear(0, 255, 0, 64);
}