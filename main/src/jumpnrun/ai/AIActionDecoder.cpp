#include"jumpnrun/ai/AIActionDecoder.h"
#include"jumpnrun/ai/AI.h"
using namespace jnr;
using namespace ai;

jnr::ai::AIActionDecoder::AIActionDecoder()
{
	symbols = { eAction::Walk, eAction::Turn, eAction::Jump, eAction::JumpWalk, eAction::Attack};
}

bool jnr::ai::AIActionDecoder::act(gfw::ManualController& mc)
{
	if (!commands.empty())
	{
		auto& c = commands.front();
		char com = c.button;
		mc.push_up = com & eButton::Up; mc.push_right = com & eButton::Right; mc.push_down = com & eButton::Down;
		mc.push_left = com & eButton::Left; mc.push_z = com & eButton::Z; mc.push_x = com & eButton::X;
		c.limit--;
		if (c.limit <= 0)
			commands.pop_front();
	}
	return commands.empty();
	//command.getCommand(this->mc, cs);
}

void jnr::ai::AIActionDecoder::push(int code, gfw::ManualController& mc, const CharacterStatus& cs)
{
	Command com;
	eAction action = (eAction)this->symbols[code];

	switch (action)
	{
	default:
		com = Command(20, 0);
		break;
	case eAction::Walk:
		com = Command(30, !cs.turned ? eButton::Right : eButton::Left);
		break;
	case eAction::Dash:
		com = Command(30, !cs.turned ? eButton::Right : eButton::Left);
		break;
	case eAction::Jump:
		com = Command(30, cs.isOnSomething ? eButton::Up : eButton::NoInput);
		break;
	case eAction::JumpWalk:
		if (1)
		{
			bool walk = !cs.turned ? eButton::Right : eButton::Left;
			com = Command(30, (cs.isOnSomething ? eButton::Up : eButton::NoInput) | walk);
			break;
		}
	case eAction::Turn:
		com = Command(12, cs.turned ? eButton::Right : eButton::Left);
		break;
	case eAction::Attack:
		com = Command(20, 0);
		break;
	};
	commands.push_back(com);
}