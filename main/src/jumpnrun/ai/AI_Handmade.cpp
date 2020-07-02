#include"jumpnrun/ai/AI_Handmade.h"
//#include"FeatureExtractor.h"
using namespace gmtr;
using namespace gfw;
using namespace jnr;
using namespace ai;

AI_Handmade::AI_Handmade(){}

AI_Handmade::~AI_Handmade(){}
void AI_Handmade::initialize(){}
Feature AI_Handmade::processMyStatus(CharacterStatus cs)
{
	return FeatureExtractor().extract(cs);
}
void AI_Handmade::think(const CharacterStatus& cs)
{
	if (command.empty())
	{
		auto f = processMyStatus(cs);

		bool imdashing = f[3] >= 0.5;
		bool imstacked = f[5] >= 0.5;
		bool objectinback = f[13] >= 0.5;

		if (imstacked)
			command.pushAction(eAction::Turn, cs);
		else
		{
			if (!objectinback)
				command.pushAction(eAction::Dash, cs);
			else command.pushAction(eAction::Turn, cs);
		}
	}
	command.getCommand(this->mc, cs);
}