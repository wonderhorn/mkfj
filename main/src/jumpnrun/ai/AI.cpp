#include"jumpnrun/ai/AI.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/mover/character/Character.h"
#include"utils/MyCppUtil.h"
#include"jumpnrun/ai/Radar.h"
#include"jumpnrun/ai/DecisionMaker.h"
using namespace jnr;
using namespace ai;


static double uniform()
{
	return (double)$P.rand.get() / RAND_MAX;
}

static int accumurate(const std::vector<double>& d)
{
	double sum = 0;
	for (int i = 0; i < d.size(); sum += d[i++]);
	double u = uniform();
	double acc = 0.0;
	for (int i = 0; i < d.size(); i++)
	{
		acc += d[i] / sum;
		if (acc >= u)
			return i;
	}
	return d.size() - 1;
}

AI::AI(){}


AI_TEST::Command::Command() : age(0), limit(1), button(0){}
AI_TEST::Command::Command(int limit, char button) : age(0), limit(limit), button(button){}

AI_TEST::ActionController::ActionController(AI_TEST& ai) : ai(ai){}
AI_TEST::Command AI_TEST::ActionController::act(eAction action, const CharacterStatus& cs)
{
	switch (action)
	{
	default:
		return AI_TEST::Command(20, 0);
	case eAction::Walk:
		return AI_TEST::Command(30, !cs.turned ? eButton::Right : eButton::Left);
	case eAction::Dash:
		return AI_TEST::Command(30, !cs.turned ? eButton::Right : eButton::Left);
	case eAction::Jump:
		return AI_TEST::Command(30, cs.isOnSomething ? eButton::Up : eButton::NoInput);
	case eAction::JumpWalk:
	{
		bool walk = !cs.turned ? eButton::Right : eButton::Left;
		return AI_TEST::Command(30, (cs.isOnSomething ? eButton::Up : eButton::NoInput) | walk);
	}
	case eAction::Turn:
		return AI_TEST::Command(5, cs.turned ? eButton::Right : eButton::Left);
	case eAction::Attack:
		return AI_TEST::Command(20, 0);
	};
}

AIParameters AIParameters::random()
{
	AIParameters param;
	for (int i = 0; i < 100; i++)
	{
		//wdl->list.push_back(WeightDecisionList::ListElement($P.rand.get() % 38, (eAction)($P.rand.get() % 8), uniform()));
		param.f.push_back($P.rand.get() % 38);
		param.action.push_back((jnr::ai::eAction)($P.rand.get() % 8));
		param.w.push_back(uniform());
	}
	return param;
}

AI_TEST::AI_TEST() : decide(NULL), action_cont(*this)
{
	status_prev.age = -1;

	/*Voting* v = new Voting();
	decisionmaker = v;
	decisionmaker_buf.push_back(decisionmaker);
	ConstDecision* d = new ConstDecision(eAction::Dash);
	ConstDecision* t = new ConstDecision(eAction::Turn);
	v->addChildren(d);
	v->addChildren(t);*/
}
AI_TEST::AI_TEST(const AIParameters& param) : decide(NULL), action_cont(*this)
{
	status_prev.age = -1;

	WeightDecisionList* wdl = new WeightDecisionList();
	decisionmaker_buf.push_back(wdl);
	decisionmaker = wdl;
	for (int i = 0; i < param.action.size(); i++)
	{
		WeightDecisionList::ListElement le(param.f[i], param.action[i], param.w[i]);
		wdl->list.push_back(le);
	}
}

AI_TEST::~AI_TEST()
{
	for (int i = 0; i < decisionmaker_buf.size(); i++)
	{
		delete decisionmaker_buf[i];
	}
}

void AI_TEST::initialize(const AIParameters& param)
{
	WeightDecisionList* wdl = new WeightDecisionList();
	decisionmaker_buf.push_back(wdl);
	decisionmaker = wdl;
	for (int i = 0; i < param.action.size(); i++)
	{
		WeightDecisionList::ListElement le(param.f[i], param.action[i], param.w[i]);
		wdl->list.push_back(le);
	}

	/*Voting* v = new Voting();
	ConstDecision* d = new ConstDecision(eAction::Dash);
	ConstDecision* t = new ConstDecision(eAction::Turn);
	decisionmaker_buf.push_back(d);
	decisionmaker_buf.push_back(t);
	v->addChildren(d);
	v->addChildren(t);
	decisionmaker = v;*/
}

Feature AI_TEST::processMyStatus(CharacterStatus cs)
{
	Feature f;
	Character* ch = (Character*)(cs.pipe.Dst(*cs.tl));
	if (ch)
	{
		/*f.push_back(ch->Status());
		f.push_back(ch->Turned());
		f.push_back(ch->V().x);
		f.push_back(ch->V().y);

		Radar radar;
		radar.myid = ch->ID();
		radar.myp = ch->P();
		radar.tl = cs.tl;
		auto vr = radar.scan(64 * 4);
		$for_i(vr.size())
		{
			Mover* mvr = vr[i];
			f.push_back(mvr == NULL);
		}*/
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
	}
	return f;
}

void AI_TEST::think(const CharacterStatus& cs)
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
		//commands.push_back(action_cont.act(eAction::Walk, cs));
		eAction a = decisionmaker->decide(f);
		commands.push_back(action_cont.act(a, cs));
	}

	if (status_prev.age >= 0)
	{
		decisionmaker->train(f, current_action, cs.hp - status_prev.hp);
		//$P.aitrainer.push(f, com, cs.hp - status_prev.hp - 0.05);
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

AI_TEST::Command AI_TEST::generateNextCommand(const Feature& f)
{
	std::vector<double> scores;
	/*for (int i = 0; i < 5; i++)
	{
		scores.push_back($P.aitrainer.predict(f, int2command(i), 15) + 0.001);
	}*/
	//int argmax = myutil::argmax<$V<double>>(scores);
	int argmax = accumurate(scores);

	Command rtn = randomCommand(f);
	rtn.button = int2command(argmax);
	return rtn;
}

AI_TEST::Command AI_TEST::randomCommand(const Feature& f)
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