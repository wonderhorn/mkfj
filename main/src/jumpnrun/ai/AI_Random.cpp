#include"jumpnrun/ai/AI_Random.h"
#include"jumpnrun/ai/FeatureExtractor.h"
#include"jumpnrun/system/Parmanent.h"
using namespace gmtr;
using namespace gfw;
using namespace jnr;
using namespace ai;

AI_Random::AI_Random(){}

AI_Random::~AI_Random(){}
void AI_Random::initialize()
{
	$P.aisuperviser.setHeader(RichFeatureExtractor().format());
}
Feature AI_Random::processMyStatus(CharacterStatus cs)
{
	return RichFeatureExtractor().extract(cs);
}
void AI_Random::think(const CharacterStatus& cs)
{
	eAction action = eAction::Keep;
	auto f = processMyStatus(cs);
	assert(f_prev.size() == 0 || f.size() == f_prev.size());
	if (command.empty())
	{
		action = (eAction)($P.rand.get() % 8);
		command.pushAction(action, cs);
	}
	command.getCommand(this->mc, cs);

	$P.aisuperviser.push(f, cs.hp, action);
	f_prev = f;
}