#include"jumpnrun/ai/AI.h"
#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/system/Parmanent.h"
#include<iostream>
using namespace gmtr;
using namespace gfw;
using namespace jnr;
using namespace ai;

static std::string button2str(char b)
{
	std::string rtn;
	if (b & eButton::Down)
		rtn += "«";
	if (b & eButton::Left)
		rtn += "©";
	if (b & eButton::Right)
		rtn += "¨";
	if (b & eButton::Up)
		rtn += "ª";
	return rtn;
}

AICommand::AICommand() : com_prev(eButton::NoInput){}

bool AICommand::empty()const
{
	return commands.empty() && actions.empty();
}

char AICommand::getCommand(gfw::ManualController& mc, const CharacterStatus& cs)
{
	char com = com_prev;
	if (!commands.empty())
	{
		Command& command = commands.front();

		com = command.button;
		command.age++;
		if (command.age >= command.limit)
			commands.pop_front();

		mc.push_up = com & eButton::Up; mc.push_right = com & eButton::Right; mc.push_down = com & eButton::Down;
		mc.push_left = com & eButton::Left; mc.push_z = com & eButton::Z; mc.push_x = com & eButton::X;
	}
	else if (!actions.empty())
	{
		current_action = actions.front();
		action_cont.act(commands, actions.front(), cs);
		actions.pop_front();

		com = commands.front().button;
		mc.push_up = com & eButton::Up; mc.push_right = com & eButton::Right; mc.push_down = com & eButton::Down;
		mc.push_left = com & eButton::Left; mc.push_z = com & eButton::Z; mc.push_x = com & eButton::X;
	}
	else
	{
		int a = 0;
	}

	if (!(com & eButton::Right))
	{
		int a = 0;
	}

	com_prev = com;
	static double vx_prev = cs.v.x;
	if (vx_prev == 2.4 && cs.v.x < vx_prev)
	{
		int a = 0;
	}
	vx_prev = cs.v.x;
	//std::cout << button2str(com) << std::endl;

	return com;
}

void AICommand::pushAction(eAction a, const CharacterStatus& cs)
{
	actions.push_back(a);
}

AICommand::ActionController::ActionController(){}
void AICommand::ActionController::act(std::deque<Command>& commands, eAction action, const CharacterStatus& cs)
{
	switch (action)
	{
	default:
		commands.push_back(AICommand::Command(20, 0));
		break;
	case eAction::Walk:
		commands.push_back(AICommand::Command(12, !cs.turned ? eButton::Right : eButton::Left));
		break;
	case eAction::Dash:
		if (cs.status == Character::Dashing)
			commands.push_back(AICommand::Command(6, !cs.turned ? eButton::Right : eButton::Left));
		else
		{
			commands.push_back(AICommand::Command(3, eButton::NoInput));
			commands.push_back(AICommand::Command(3, !cs.turned ? eButton::Right : eButton::Left));
			commands.push_back(AICommand::Command(3, eButton::NoInput));
			commands.push_back(AICommand::Command(6, !cs.turned ? eButton::Right : eButton::Left));
		}
		break;
	case eAction::Jump:
		commands.push_back(AICommand::Command(15, cs.isOnSomething ? eButton::Up : eButton::NoInput));
		break;
	case eAction::JumpWalk:
	{

		if (cs.status == Character::Dashing)
			commands.push_back(AICommand::Command(30, eButton::Up | (!cs.turned ? eButton::Right : eButton::Left)));
		else
		{
			commands.push_back(AICommand::Command(3, eButton::NoInput));
			commands.push_back(AICommand::Command(3, !cs.turned ? eButton::Right : eButton::Left));
			commands.push_back(AICommand::Command(3, eButton::NoInput));
			commands.push_back(AICommand::Command(6, eButton::Up | (!cs.turned ? eButton::Right : eButton::Left)));
		}
		break;
	}

	case eAction::Turn:
		commands.push_back(AICommand::Command(12, cs.turned ? eButton::Right : eButton::Left));
		break;
	case eAction::Attack:
		commands.push_back(AICommand::Command(20, eButton::Z));
		break;
	case eAction::Cast:
		commands.push_back(AICommand::Command(20, eButton::X));
		break;
	};
}

AICommand::Command AICommand::randomCommand(const Feature& f)
{
	Command c;
	c.age = 0;
	c.limit = $P.rand.get() % 300;
	int r = $P.rand.get() % 5;
	c.button = eButton::NoInput;
	switch (r)
	{
	case 0:
		c.button |= eButton::Left;
		break;
	case 1:
		c.button |= eButton::Right;
		break;
	case 2:
		c.button |= eButton::Right;
		c.button |= eButton::Up;
		break;
	case 3:
		c.button |= eButton::Right;
		c.button |= eButton::Up;
		break;
	case 4:
		break;
	}
	return c;
}

AICommand::Command::Command() : age(0), limit(1), button(0){}
AICommand::Command::Command(int limit, char button) : age(0), limit(limit), button(button){}