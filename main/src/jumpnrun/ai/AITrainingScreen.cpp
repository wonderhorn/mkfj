#include"jumpnrun/ai/AITrainingScreen.h"
#include<iostream>
#include<float.h>
using namespace gfw;
using namespace jnr;
using namespace ai;

AITrainingScreen::AITrainingScreen(gfw::Game* game)
: Screen(game), training_scr(nullptr), ai(nullptr){}

AITrainingScreen::~AITrainingScreen(){}

bool AITrainingScreen::initialize()
{
	if (ai)
		delete ai;
	auto* aibt = new AI_BanditTree();
	aibt->initialize();
	ai = aibt;

	if (training_scr)
		delete training_scr;
	training_scr = new AIEvaluateScreen(game);
	training_scr->initialize(ai);
	training_scr->isRendered = true;
	score = FLT_MIN;
	return true;
}

bool AITrainingScreen::initialize(AI* ai)
{
	this->ai = ai;

	if (training_scr)
		delete training_scr;
	training_scr = new AIEvaluateScreen(game);
	training_scr->StageName(stage_name);
	training_scr->initialize(ai);
	training_scr->isRendered = true;
	score = FLT_MIN;
	return true;
}

void AITrainingScreen::finalize(){}

gfw::Screen::eScreenState AITrainingScreen::run(gfw::BaseMedia& media)
{
	//std::cout << "hey" << std::endl;

	eScreenState result = training_scr->run(media);
	if (result == eScreenState::Finished)
	{
		this->score = training_scr->ai_fitness;
		//training_scr->finalize();
		//this->initialize();
		return eScreenState::Finished;
	}

	return eScreenState::Continuing;
}

void AITrainingScreen::render(gfw::BaseMedia& media)
{
	BaseGraphics* graphics = media.getGraphics();
	graphics->clear(110, 110, 110, 255);
	training_scr->render(media);
}