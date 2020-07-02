#define _USE_MATH_DEFINES
#include"jumpnrun/ai/AI_Base.h"
#include"jumpnrun/ai/AIModules.h"
#include"jumpnrun/ai/Radar.h"
#include"jumpnrun/system/Parmanent.h"
#include"utils/MyCppUtil.h"

#include<math.h>

using namespace gfw;
using namespace jnr;
using namespace ai;


AI_Base::Command::Command() : age(0), limit(1), button(0){}
AI_Base::Command::Command(int limit, char button) : age(0), limit(limit), button(button){}

AI_Base::ActionController::ActionController(AI_Base& ai) : ai(ai){}
AI_Base::Command AI_Base::ActionController::act(eAction action, const CharacterStatus& cs)
{
	switch (action)
	{
	default:
		return AI_Base::Command(20, 0);
	case eAction::Walk:
		return AI_Base::Command(30, !cs.turned ? eButton::Right : eButton::Left);
	case eAction::Dash:
		return AI_Base::Command(30, !cs.turned ? eButton::Right : eButton::Left);
	case eAction::Jump:
		return AI_Base::Command(30, cs.isOnSomething ? eButton::Up : eButton::NoInput);
	case eAction::JumpWalk:
	{
		bool walk = !cs.turned ? eButton::Right : eButton::Left;
		return AI_Base::Command(30, (cs.isOnSomething ? eButton::Up : eButton::NoInput) | walk);
	}
	case eAction::Turn:
		return AI_Base::Command(16, cs.turned ? eButton::Right : eButton::Left);
	case eAction::Attack:
		return AI_Base::Command(20, 0);
	};
}


AI_Base::AI_Base() : action_cont(*this), mods_root(NULL)
{
	status_prev.age = -1;
	mods.push_back(new ConstModule(eAction::Dash));
	mods.push_back(new ConstModule(eAction::Turn));
	mods.push_back(new Alternate(mods[0], mods[1]));
	mods_root = mods[2];
}

AI_Base::~AI_Base()
{
	$for_i(mods.size())
	{
		delete mods[i];
	}
	mods.clear();
}

void AI_Base::initialize()
{}

Feature AI_Base::processMyStatus(CharacterStatus cs)
{
	Feature f;
	Character* ch = (Character*)(cs.pipe.Dst(*cs.tl));
	if (ch)
	{
		f.push_back(ch->Turned());
		f.push_back(ch->Status() == Character::eStatus::Standing);
		f.push_back(ch->Status() == Character::eStatus::Walking);
		f.push_back(ch->Status() == Character::eStatus::Dashing);
		f.push_back(ch->Status() == Character::eStatus::Flying);
		f.push_back((ch->Status() == Character::eStatus::Walking || ch->Status() == Character::eStatus::Dashing)
			&& ch->V().l2() <= 0.05);

		Radar radar;
		radar.myid = ch->ID();
		radar.myp = ch->P();
		radar.tl = cs.tl;
		auto vr = radar.scan(64 * 4);
		$for_i(vr.size())
		{
			Mover* mvr = vr[i];
			f.push_back(mvr != NULL);
		}
		vr = radar.scan(64 * 2);
		$for_i(vr.size())
		{
			Mover* mvr = vr[i];
			f.push_back(mvr != NULL);
		}

		if (cs.stg)
		{
			const int max_i = 12;
			$for_i(max_i)
			{
				const double theta = M_PI * i / max_i;
				const double r = 48;
				gmtr::Vector2D dp(r * cos(theta),  r * sin(theta));
				bool b = cs.stg->enterable(cs.p, cs.p + dp);
				f.push_back(b);
			}
		}
	}
	return f;
}

void AI_Base::think(const CharacterStatus& cs)
{
	Feature f = processMyStatus(cs);
	char com = eButton::NoInput;
	if (!commands.empty())
	{
		Command& command = commands.front();
		mc.push_up = command.button & eButton::Up; mc.push_right = command.button & eButton::Right; mc.push_down = command.button & eButton::Down; mc.push_left = command.button & eButton::Left; mc.push_z = command.button & eButton::Z; mc.push_x = command.button & eButton::X;

		com = command.button;
		command.age++;
		if (command.age >= command.limit)
			commands.pop_front();
	}
	else if (!actions.empty())
	{
		current_action = actions.front();
		commands.push_back(action_cont.act(actions.front(), cs));
		actions.pop_front();
	}
	else
	{
		//commands.push_back(action_cont.act(eAction::Walk, cs));
		//commands.push_back(action_cont.act(eAction::Turn, cs));
		commands.push_back(action_cont.act((eAction)mods_root->decide(cs), cs));
	}

	if (status_prev.age >= 0)
	{

	}
	//mc.push_right = true;
	status_prev = cs;
}

static char int2command(int r)
{
	char c = 0;
	switch (r)
	{
	case 0:
		c |= eButton::Left;
		break;
	case 1:
		c |= eButton::Right;
		break;
	case 2:
		c |= eButton::Right;
		c |= eButton::Up;
		break;
	case 3:
		c |= eButton::Right;
		c |= eButton::Up;
		break;
	case 4:
		break;
	}
	return c;
}

AI_Base::Command AI_Base::randomCommand(const Feature& f)
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