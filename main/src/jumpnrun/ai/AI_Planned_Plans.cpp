#include"jumpnrun/ai/FeatureExtractor.h"
#include"jumpnrun/ai/AI_Planned.h"
#include"jumpnrun/mover/StageAccessor.h"
#include"jumpnrun/ai/Radar.h"
using namespace gmtr;
using namespace gfw;
using namespace jnr;
using namespace ai;
#define $V(x, y) gmtr::Vector2D((x), (y))
static int sample(const std::map<int, double>& v)
{
	double sum = 0;
	for (auto p : v)
	{
		sum += p.second;
	}
	if (sum == 0.0)
	{
		return 0;
	}
	double u = (double)$P.rand.get() / RAND_MAX;
	double accum = 0;
	for (auto p : v)
	{
		accum += p.second / sum;
		if (u < accum)
			return p.first;
	}
	return -1;
}

ActionPlanReturn Plan_Stop::run(const CharacterStatus& cs)
{
	return { eAction::Wait };
}

ActionPlanReturn Plan_TouchTurn::run(const CharacterStatus& cs)
{
	double d2wall = StageAccessor(*cs.stg).distanceToWall(cs.p, cs.turned, *cs.data);
	bool imstacked = d2wall < 32;
	if (imstacked)
		return{ eAction::Turn, eAction::Dash, eAction::Dash };
	else
		return{ eAction::Dash };
}
ActionPlanReturn Plan_Climb::run(const CharacterStatus& cs)
{
	double d2wall = StageAccessor(*cs.stg).distanceToWall(cs.p, cs.turned, *cs.data);
	double d2wall2 = StageAccessor(*cs.stg).distanceToWall(cs.p - $V(0, 64), cs.turned, *cs.data);
	bool imstacked = d2wall < 32 && d2wall2 > d2wall;
	if (imstacked)
		return{ eAction::JumpWalk, eAction::Dash };
	else
		return{ eAction::Dash };
}

ActionPlanReturn Plan_AttackIfFind::run(const CharacterStatus& cs)
{
	Radar radar;
	radar.myid = cs.id;
	radar.myp = cs.p;
	radar.tl = cs.tl;
	radar.turned = cs.turned;
	auto vr = radar.scan(th);
	for (int i : {0, 1, 14, 15})
	{
		if (auto ptr = vr[i])
		{
			double d = (ptr->Center() - cs.p).l2();
			if (ptr->Name().substr(0, 9) == "character" && d < th
				&& ptr->Owner() != cs.owner)
			{
				ActionPlanReturn rtn;
				rtn.actions = { eAction::Attack, eAction::Wait };
				rtn.current_z_spell = cursor;
				return rtn;
			}
		}
	}

	if (nexts.size() > 0)
		return nexts[0]->run(cs);
	else
	{
		double d2wall = StageAccessor(*cs.stg).distanceToWall(cs.p, cs.turned, *cs.data);
		bool imstacked = d2wall < 32;
		if (imstacked)
			return{ eAction::Turn };
		else
			return{ eAction::Dash };
	}
}
ActionPlanReturn Plan_ChaseNearestEnemy::run(const CharacterStatus& cs)
{
	Radar radar;
	radar.myid = cs.id;
	radar.myp = cs.p;
	radar.tl = cs.tl;
	radar.turned = cs.turned;
	auto vr = radar.scan(64 * 3);
	double min_d2 = FLT_MAX;
	Mover* argmin = nullptr;
	for (auto ptr : vr)
	{
		if (ptr && ptr->Name().substr(0, 9) == "character"
			&& ptr->Owner() != cs.owner
			&& (ptr->Center() - cs.p).l2() < min_d2)
		{
			min_d2 = (ptr->Center() - cs.p).l2();
			argmin = ptr;
		}
	}

	if (argmin)
	{
		auto dp = argmin->Center() - cs.p;
		if (dp.l2() > 192 || dp.x < 0 != cs.turned)
		{
			if ((dp.x > 0 && !cs.turned) || (dp.x < 0 && cs.turned))
				return{ eAction::Dash };
			else
				return{ eAction::Turn };
		}

		if (nexts.size() > 0)
			return nexts[0]->run(cs);
		return Plan_TouchTurn().run(cs);
	}
	else
	{
		double d2wall = StageAccessor(*cs.stg).distanceToWall(cs.p, cs.turned, *cs.data);
		bool imstacked = d2wall < 32;
		if (imstacked)
			return{ eAction::Turn };
		else
			return{ eAction::Dash };
	}
}

ActionPlanReturn Plan_ChaseNearestEnemySwim::run(const CharacterStatus& cs)
{
	Radar radar;
	radar.myid = cs.id;
	radar.myp = cs.p;
	radar.tl = cs.tl;
	radar.turned = cs.turned;
	auto vr = radar.scan(64 * 3);
	double min_d2 = FLT_MAX;
	Mover* argmin = nullptr;
	for (auto ptr : vr)
	{
		if (ptr && ptr->Name().substr(0, 9) == "character"
			&& ptr->Owner() != cs.owner
			&& (ptr->Center() - cs.p).l2() < min_d2)
		{
			min_d2 = (ptr->Center() - cs.p).l2();
			argmin = ptr;
		}
	}

	if (argmin)
	{
		auto dp = argmin->Center() - cs.p;
		if (dp.l2() > 192 || dp.x < 0 != cs.turned)
		{
			if ((dp.x > 0 && !cs.turned) || (dp.x < 0 && cs.turned))
				return{ eAction::Dash };
			else
				return{ eAction::Turn };
		}

		if (nexts.size() > 0)
			return nexts[0]->run(cs);
		return Plan_TouchTurn().run(cs);
	}
	else
	{
		double d2wall = StageAccessor(*cs.stg).distanceToWall(cs.p, cs.turned, *cs.data);
		bool imstacked = d2wall < 32;
		if (imstacked)
			return{ eAction::Turn };
		else
		{
			if(cs.location == 'w')
				return{ eAction::JumpWalk };
			else
				return{ eAction::Dash };
		}
	}
}


ActionPlanReturn Plan_ChaseEscapeFrom::run(const CharacterStatus& cs)
{
	Radar radar;
	radar.myid = cs.id;
	radar.myp = cs.p;
	radar.tl = cs.tl;
	radar.turned = cs.turned;
	auto vr = radar.scan(64 * 3);
	double min_d2 = FLT_MAX;
	Mover* argmin = nullptr;
	for (auto ptr : vr)
	{
		if (ptr && ptr->Name().substr(0, 9) == "character"
			&& ptr->Owner() != cs.owner
			&& (ptr->Center() - cs.p).l2() < min_d2)
		{
			min_d2 = (ptr->Center() - cs.p).l2();
			argmin = ptr;
		}
	}

	if (argmin)
	{
		auto dp = argmin->Center() - cs.p;
		if (dp.l2() > 192 || dp.x < 0 != cs.turned)
		{
			if ((dp.x > 0 && cs.turned) || (dp.x < 0 && !cs.turned))
				return{ eAction::Dash };
			else
				return{ eAction::Turn };
		}

		if (nexts.size() > 0)
			return nexts[0]->run(cs);
		return Plan_TouchTurn().run(cs);
	}
	else
	{
		double d2wall = StageAccessor(*cs.stg).distanceToWall(cs.p, cs.turned, *cs.data);
		bool imstacked = d2wall < 32;
		if (imstacked)
			return{ eAction::Turn };
		else
			return{ eAction::Dash };
	}
}


ActionPlanReturn Plan_NearNoEnemy::run(const CharacterStatus& cs)
{
	Radar radar;
	radar.myid = cs.id;
	radar.myp = cs.p;
	radar.tl = cs.tl;
	radar.turned = cs.turned;
	auto vr = radar.scan(64 * 3);
	double min_d2 = FLT_MAX;
	Mover* argmin = nullptr;
	for (auto ptr : vr)
	{
		if (ptr && ptr->Name().substr(0, 9) == "character"
			&& (ptr->Center() - cs.p).l2() < min_d2)
		{
			min_d2 = (ptr->Center() - cs.p).l2();
			argmin = ptr;
		}
	}

	if (argmin)
	{
		auto dp = argmin->Center() - cs.p;
		if (dp.l2() < 192)
		{
			if (nexts.size() > 0)
				return nexts[1]->run(cs);
			if ((dp.x < 0 && !cs.turned) || (dp.x > 0 && cs.turned))
			{
				return Plan_TouchTurn().run(cs);
				//return{ eAction::Dash };
			}
			else
			{
				if (dp.l2() < 40)
					return{ eAction::Turn , eAction::Walk };
				else
					return{ eAction::Turn , eAction::Dash };
			}
		}

		if (nexts.size() > 0)
			return nexts[0]->run(cs);
		return{ eAction::Cast };

	}
	else
	{
		if (nexts.size() > 1)
			return nexts[0]->run(cs);
		return{ eAction::Cast };
	}
}

ActionPlanReturn Plan_EnemyBehind::run(const CharacterStatus& cs)
{
	Radar radar;
	radar.myid = cs.id;
	radar.myp = cs.p;
	radar.tl = cs.tl;
	radar.turned = cs.turned;
	auto vr = radar.scan(th);
	for (int i : {7, 8, 9})
	{
		if (auto ptr = vr[i])
		{
			double d = (ptr->Center() - cs.p).l2();
			if (ptr->Name().substr(0, 9) == "character" && d < th)
				return nexts[0]->run(cs);;
		}
	}

	if (nexts.size() > 1)
		return nexts[1]->run(cs);
	else
	{
		double d2wall = StageAccessor(*cs.stg).distanceToWall(cs.p, cs.turned, *cs.data);
		bool imstacked = d2wall < 32;
		if (imstacked)
			return{ eAction::Turn };
		else
			return{ eAction::Dash };
	}
}

ActionPlanReturn Plan_BulletGoingToHit::run(const CharacterStatus& cs)
{
	Radar radar;
	radar.myid = cs.id;
	radar.myp = cs.p;
	radar.tl = cs.tl;
	radar.turned = cs.turned;
	const int th = 128;
	auto vr = radar.scan(th);
	for (int i : {15, 0, 1, 7, 8, 9})
	{
		if (auto ptr = vr[i])
		{
			double d = (ptr->Center() - cs.p).l2();
			if (ptr->Name().substr(0, 6) == "bullet"
				&& d < th
				&& ptr->Owner() != cs.owner) {
 				return nexts[0]->run(cs);
			}
		}
	}

	if (nexts.size() > 1)
		return nexts[1]->run(cs);
	else
	{
		double d2wall = StageAccessor(*cs.stg).distanceToWall(cs.p, cs.turned, *cs.data);
		bool imstacked = d2wall < 32;
		if (imstacked)
			return{ eAction::Turn };
		else
			return{ eAction::Dash };
	}
}



ActionPlanReturn Plan_NearFriend::run(const CharacterStatus& cs)
{
	ActionPlanReturn rtn_wait;
	rtn_wait.actions = { eAction::Wait };
	Radar radar;
	radar.myid = cs.id;
	radar.myp = cs.p;
	radar.tl = cs.tl;
	radar.turned = cs.turned;
	auto vr = radar.scan(th);
	double min_d2 = FLT_MAX;
	Mover* argmin = nullptr;
	for (auto ptr : vr)
	{
		if (ptr 
			&& (ptr->Name().substr(0, 9) == "character" || ptr->Name().substr(0, 6) == "bubble")
			&& ptr->Owner() == cs.owner
			&& (ptr->Center() - cs.p).l2() < min_d2)
		{
			min_d2 = (ptr->Center() - cs.p).l2();
			argmin = ptr;
		}
	}

	if (argmin)
	{
		auto dp = argmin->Center() - cs.p;
		if (dp.l2() < th)
		{
			if (nexts.size() > 1)
			{
				return nexts[1]->run(cs);
			}
			
				return rtn_wait;
		}
	}

	if (nexts.size() > 0)
	{
		auto rtn = nexts[0]->run(cs);
		if (rtn.actions.size() == 0)
		{
			int a = 0;
		}
		return rtn;
	}
	return rtn_wait;
	
}


ActionPlanReturn Plan_Cast::run(const CharacterStatus& cs)
{

	//else
	{
		if (nexts.size() > 1)
			return nexts[0]->run(cs);
		ActionPlanReturn rtn;
		rtn.actions = { eAction::Cast };
		rtn.current_x_spell = cursor;
		return rtn;
	}
}

ActionPlanReturn Plan_Turn::run(const CharacterStatus& cs)
{
	//else
	{
		if (nexts.size() > 1)
			return nexts[0]->run(cs);
		return{ eAction::Turn };
	}
}

ActionPlanReturn Plan_Jump::run(const CharacterStatus& cs)
{
	//else
	{
		if (nexts.size() > 1)
			return nexts[0]->run(cs);
		return{ eAction::JumpWalk };
	}
}


ActionPlanReturn Plan_ManaGreaterThan::run(const CharacterStatus& cs)
{
	if (cs.mp > th)
		return nexts[0]->run(cs);
	else return nexts[1]->run(cs);
}

ActionPlanReturn Plan_HPGreaterThan::run(const CharacterStatus& cs)
{
	if (cs.hp > th)
		return nexts[0]->run(cs);
	else return nexts[1]->run(cs);
}

ActionPlanReturn Plan_NumOfFriendsGreaterThan::run(const CharacterStatus& cs)
{
	if (cs.num_of_friends > th)
		return nexts[0]->run(cs);
	else return nexts[1]->run(cs);
}

ActionPlanReturn Plan_MarkovChain::run(const CharacterStatus& cs)
{
	double u = (double)$P.rand.get() / RAND_MAX;
	state = sample(probs[state]);
	/*if (u < 0.1)
	state = !state;*/
	auto rtn = nexts[state % nexts.size()]->run(cs);
	if (rtn.actions.size() == 0) 
	{
		int a = 0;
	}
	return rtn;
}