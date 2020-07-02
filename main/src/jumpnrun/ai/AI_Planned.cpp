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

AI_Planned::AI_Planned() {}

AI_Planned::~AI_Planned() {}
void AI_Planned::initialize()
{
	//plan.push_back((new Plan_ChaseNearestEnemy())->pushNext(new Plan_AttackIfFind()));
	//plan.push_back((new Plan_AttackIfFind())
	//	->pushNext(new Plan_ChaseNearestEnemy()));
	//plan.push_back(new Plan_NearNoEnemy());
	plan.push_back(
		(new Plan_MarkovChain())
		->pushNext(new Plan_NearNoEnemy())
		->pushNext((new Plan_ChaseNearestEnemy())
			->pushNext(new Plan_AttackIfFind(128, 0))
		)
	);
}

void AI_Planned::initialize(const std::string& data)
{
	//plan.push_back((new Plan_ChaseNearestEnemy())->pushNext(new Plan_AttackIfFind()));
	//plan.push_back((new Plan_AttackIfFind())
	//	->pushNext(new Plan_ChaseNearestEnemy()));
	//plan.push_back(new Plan_NearNoEnemy());
	plan.clear();
	if (data == "maka")
	{
		std::map<int, std::map<int, double>> probs;
		probs[0][0] = 0.5;
		probs[0][1] = 0.5;
		probs[1][0] = 0.5;
		probs[1][1] = 0.5;
		probs[0][2] = 0.2;
		probs[2][0] = 0.2;
		probs[2][1] = 0.8;
		probs[2][2] = 0.2;

		plan.push_back((new Plan_MarkovChain(probs))
			->pushNext(new Plan_NearNoEnemy())
			//->pushNext((new Plan_ChaseNearestEnemy())
			->pushNext((new Plan_Stop())
				->pushNext(new Plan_AttackIfFind(48, 0)))
			->pushNext((new Plan_Stop())));

		CharacterStatus cs;
		command.pushAction(Cast, cs);
	}
	if (data == "kyo1")
	{
		std::map<int, std::map<int, double>> probs;
		probs[0][0] = 0.8;
		probs[0][1] = 0.2;
		probs[1][0] = 0.2;
		probs[1][1] = 0.8;

		std::map<int, std::map<int, double>> probs2;
		probs2[0][0] = 0.5;
		probs2[0][1] = 0.5;
		probs2[1][0] = 0.5;
		probs2[1][1] = 0.5;

		plan.push_back(
			(new Plan_NearNoEnemy())
			->pushNext((new Plan_MarkovChain(probs2))
				->pushNext((new Plan_Stop())
					->pushNext((new Plan_Cast())))
			)

			->pushNext((new Plan_ChaseNearestEnemy())
				->pushNext((new Plan_AttackIfFind(160, 0)))
			)
		);

		CharacterStatus cs;
		command.pushAction(Cast, cs);
	}
	if (data == "kyo2")
	{
		std::map<int, std::map<int, double>> probs;
		probs[0][0] = 0.8;
		probs[0][1] = 0.2;
		probs[1][0] = 0.2;
		probs[1][1] = 0.8;

		std::map<int, std::map<int, double>> probs2;
		probs2[0][0] = 0.90;
		probs2[0][1] = 0.10;
		probs2[1][0] = 1.0;
		probs2[1][1] = 0.0;

		plan.push_back(
			(new Plan_NearNoEnemy())
				->pushNext((new Plan_HPGreaterThan(110))
					->pushNext((new Plan_Stop()))
					->pushNext((new Plan_MarkovChain(probs2))
						->pushNext((new Plan_Stop()))
						->pushNext((new Plan_Cast(2)))
					)
					
				)

			->pushNext((new Plan_ChaseNearestEnemy())
				->pushNext((new Plan_MarkovChain(probs2))
					->pushNext((new Plan_Jump()))
					->pushNext((new Plan_AttackIfFind(128, 0)))
				)
			)
		);

		this->current_z_spell = 4;
		CharacterStatus cs;
		command.pushAction(Attack, cs);

	}
	if (data == "rika")
	{
		/*plan.push_back((new Plan_EnemyBehind(48))
			->pushNext((new Plan_Turn()))
			->pushNext((new Plan_TouchTurn()))
			);*/
		std::map<int, std::map<int, double>> probs;
		probs[0][0] = 0.4;
		probs[0][1] = 0.6;
		probs[1][0] = 0.6;
		probs[1][1] = 0.4;
		//probs[0][2] = 0.2;
		//probs[2][0] = 0.2;
		//probs[2][1] = 0.8;
		//probs[2][2] = 0.2;

		std::map<int, std::map<int, double>> probs2;
		probs[0][0] = 0.1;
		probs[0][1] = 0.9;
		probs[1][0] = 0.1;
		probs[1][1] = 0.9;

		plan.push_back((new Plan_EnemyBehind(48))
			->pushNext((new Plan_Turn()))
			->pushNext((new Plan_MarkovChain(probs))
				->pushNext((new Plan_NearNoEnemy())
					->pushNext((new Plan_MarkovChain(probs2))
						->pushNext((new Plan_Cast()))
						->pushNext((new Plan_ChaseNearestEnemy()))
					)
					->pushNext((new Plan_AttackIfFind(64, 0))))
				->pushNext(new Plan_AttackIfFind(64, 0)))
		);
		/*plan.push_back((new Plan_MarkovChain(probs))
			->pushNext((new Plan_NearNoEnemy())
				->pushNext((new Plan_MarkovChain(probs2))
					->pushNext((new Plan_Cast())))
					->pushNext((new Plan_ChaseNearestEnemy()))
				->pushNext(new Plan_AttackIfFind(64)))
				->pushNext((new Plan_AttackIfFind())));*/

		CharacterStatus cs;
		command.pushAction(Cast, cs);
	}
	if (data == "tamao")
	{
		plan.push_back((new Plan_MarkovChain())
			->pushNext(new Plan_NearNoEnemy())
			->pushNext((new Plan_ChaseNearestEnemy())
				->pushNext((new Plan_ManaGreaterThan(20))
					->pushNext(new Plan_AttackIfFind(128, 0))
					->pushNext(new Plan_AttackIfFind(128, 4))
				)
			)
		);
	}
	if (data == "wakaba")
	{
		std::map<int, std::map<int, double>> probs;
		probs[0][0] = 0;
		probs[0][1] = 0.7;
		probs[0][2] = 0.3;
		probs[1][0] = 0.1;
		probs[1][1] = 0.9;
		probs[1][2] = 0.0;
		probs[2][0] = 1;
		probs[2][1] = 0;
		probs[2][2] = 0;
		plan.push_back((new Plan_MarkovChain(probs))
			->pushNext(new Plan_NearNoEnemy())
			->pushNext((new Plan_ChaseNearestEnemySwim())
				->pushNext((new Plan_ManaGreaterThan(30))
					->pushNext(new Plan_AttackIfFind(160, 0))
					->pushNext(new Plan_AttackIfFind(70, 4))
				)
			)
			->pushNext(new Plan_Cast())
		);
	}
	if (data == "mari")
	{
		/*plan.push_back((new Plan_EnemyBehind(48))
		->pushNext((new Plan_Turn()))
		->pushNext((new Plan_TouchTurn()))
		);*/
		std::map<int, std::map<int, double>> probs;
		probs[0][0] = 0.3;
		probs[0][1] = 0.7;
		probs[1][0] = 0.7;
		probs[1][1] = 0.3;
		//probs[0][2] = 0.2;
		//probs[2][0] = 0.2;
		//probs[2][1] = 0.8;
		//probs[2][2] = 0.2;

		std::map<int, std::map<int, double>> probs2;
		probs2[0][0] = 0.3;
		probs2[0][1] = 0.7;
		probs2[1][0] = 0.8;
		probs2[1][1] = 0.2;

		std::map<int, std::map<int, double>> probs3;
		probs3[0][0] = 0.0;
		probs3[0][1] = 1.0;
		probs3[1][0] = 0.1;
		probs3[1][1] = 0.9;

		plan.push_back((new Plan_MarkovChain(probs2))
			->pushNext((new Plan_BulletGoingToHit(20))
				->pushNext((new Plan_Jump()))
				->pushNext((new Plan_NearFriend())
					->pushNext((new Plan_MarkovChain(probs))
						->pushNext(new Plan_Climb())
						->pushNext(new Plan_TouchTurn())
					)
				)
			)
			->pushNext((new Plan_ManaGreaterThan(30))
				->pushNext((new Plan_AttackIfFind(280, 0))
					->pushNext((new Plan_NumOfFriendsGreaterThan(0))
						->pushNext(new Plan_TouchTurn())
						->pushNext((new Plan_MarkovChain(probs3))
							->pushNext(new Plan_Cast(0))
							->pushNext(new Plan_TouchTurn())
						)
					)
				)

				->pushNext((new Plan_AttackIfFind(280, 1))
					->pushNext((new Plan_NumOfFriendsGreaterThan(0))
						->pushNext(new Plan_TouchTurn())
						->pushNext((new Plan_MarkovChain(probs3))
							->pushNext(new Plan_Cast(0))
							->pushNext(new Plan_TouchTurn())
						)
					)
				)
			)

		);

		/*
		plan.push_back(
			(new Plan_MarkovChain(probs2))
				->pushNext((new Plan_MarkovChain(probs))
					->pushNext(new Plan_Climb())
					->pushNext(new Plan_TouchTurn())
				)
				->pushNext((new Plan_AttackIfFind(192, 0))
					->pushNext((new Plan_MarkovChain(probs3))
						->pushNext(new Plan_Cast())
						->pushNext(new Plan_TouchTurn())
					)
				)
		);*/

		CharacterStatus cs;
		command.pushAction(Turn, cs);
		command.pushAction(Dash, cs);
		command.pushAction(Cast, cs);
	}
	else
	{
		plan.push_back((new Plan_MarkovChain())
			->pushNext(new Plan_NearNoEnemy())
			->pushNext((new Plan_ChaseNearestEnemy())
				->pushNext(new Plan_AttackIfFind(192, 0))));
	}
}

Feature AI_Planned::processMyStatus(CharacterStatus cs)
{
	return FeatureExtractor().extract(cs);
}
void AI_Planned::think(const CharacterStatus& cs)
{
	if (command.empty())
	{
		auto f = processMyStatus(cs);

		if (!plan.empty())
		{
			auto *p = plan.front();
			ActionPlanReturn apr = p->run(cs);
			std::vector<eAction> action = apr.actions;
			this->current_x_spell = apr.current_x_spell;
			this->current_z_spell = apr.current_z_spell;
			if(action.size() == 0)
			{
				ActionPlanReturn apr2 = p->run(cs);
			}
			assert(action.size() > 0);
			if (p->finished)
			{
				plan.pop_front();
				delete p;
			}
			for (auto a : action)
				command.pushAction(a, cs);
		}
		else
		{
			generateNewPlan();
		}
	}
	command.getCommand(this->mc, cs);
}