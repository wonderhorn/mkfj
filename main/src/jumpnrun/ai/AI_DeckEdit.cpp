#include"jumpnrun/ai/FeatureExtractor.h"
#include"jumpnrun/ai/AI_DeckEdit.h"
using namespace gmtr;
using namespace gfw;
using namespace jnr;
using namespace ai;

AI_DeckEdit::AI_DeckEdit(){}

AI_DeckEdit::~AI_DeckEdit(){}
void AI_DeckEdit::initialize()
{
	target_x = 64;
	command.commands.clear();
	command.actions.clear();
}
Feature AI_DeckEdit::processMyStatus(CharacterStatus cs)
{
	return FeatureExtractor().extract(cs);
}
void AI_DeckEdit::think(const CharacterStatus& cs)
{
	if (command.empty())
	{
		auto f = processMyStatus(cs);

		bool imdashing = f[3] >= 0.5;
		bool imstacked = f[5] >= 0.5;
		bool objectinback = f[13] >= 0.5;


		if (cs.p.x >= target_x + 32)
		{
			if (!cs.turned)
				command.pushAction(eAction::Turn, cs);
			command.pushAction(eAction::Walk, cs);

		}
		else if (cs.p.x < target_x - 32)
		{
			if (cs.turned)
				command.pushAction(eAction::Turn, cs);
			command.pushAction(eAction::Walk, cs);
		}
		else if (cs.turned)
		{
			command.pushAction(eAction::Turn, cs);
		}
		else
		{
			command.pushAction(eAction::Wait, cs);
			command.pushAction(eAction::Cast, cs);
			command.pushAction(eAction::Wait, cs);
			command.pushAction(eAction::Wait, cs);
			command.pushAction(eAction::Wait, cs);
			command.pushAction(eAction::Wait, cs);
			command.pushAction(eAction::Wait, cs);
			//command.pushAction(eAction::Wait, cs);
			//command.pushAction(eAction::Wait, cs);
		}

	}
	command.getCommand(this->mc, cs);
}