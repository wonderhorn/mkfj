#pragma once
#include"jumpnrun/ai/FeatureExtractor.h"
#include"jumpnrun/ai/AI.h"
#include<string>
#include<deque>
#include<map>
typedef std::vector<double> Feature;

namespace jnr
{
	namespace ai
	{
		class CharacterStatus;

		class ActionPlanReturn
		{
		public:
			ActionPlanReturn() :
				current_x_spell(0), current_z_spell(0){}
			ActionPlanReturn(const std::vector<eAction>& actions) :
				current_x_spell(0), current_z_spell(0), actions(actions) {}
			ActionPlanReturn(const std::initializer_list<eAction>& actions) :
				current_x_spell(0), current_z_spell(0), actions(actions) {}
			std::vector<eAction> actions;
			int current_x_spell;
			int current_z_spell;
		};

		class ActionPlan
		{
		public:
			ActionPlan() : finished(false){}
			~ActionPlan(){ for (auto* ptr : nexts){ if (ptr) delete ptr; } }
			virtual ActionPlanReturn run(const CharacterStatus& cs) = 0;
			bool finished;
			ActionPlan* pushNext(ActionPlan* n){ nexts.push_back(n); return this; }
			std::vector<ActionPlan*> nexts;
		};

		class Plan_Stop : public ActionPlan
		{
		public:
			virtual ActionPlanReturn run(const CharacterStatus& cs);
		};
		class Plan_TouchTurn : public ActionPlan
		{
		public:
			virtual ActionPlanReturn run(const CharacterStatus& cs);
		};
		class Plan_Climb : public ActionPlan
		{
		public:
			virtual ActionPlanReturn run(const CharacterStatus& cs);
		};
		class Plan_Chase: public ActionPlan
		{
		public:
			virtual ActionPlanReturn run(const CharacterStatus& cs);
		};
		class Plan_AttackIfFind : public ActionPlan
		{
		public:
 			Plan_AttackIfFind(): th(96), cursor(0) {}
			Plan_AttackIfFind(int th, int cursor): th(th), cursor(cursor){}
			virtual ActionPlanReturn run(const CharacterStatus& cs);
			int th, cursor;
		};
		class Plan_ChaseNearestEnemy : public ActionPlan
		{
		public:
			virtual ActionPlanReturn run(const CharacterStatus& cs);
		};
		class Plan_ChaseNearestEnemySwim : public ActionPlan
		{
		public:
			virtual ActionPlanReturn run(const CharacterStatus& cs);
		};

		class Plan_ChaseEscapeFrom : public ActionPlan
		{
		public:
			virtual ActionPlanReturn run(const CharacterStatus& cs);
		};
		class Plan_NearNoEnemy : public ActionPlan
		{
		public:
			Plan_NearNoEnemy() : th(160) {}
			Plan_NearNoEnemy(int th): th(th) {}
			virtual ActionPlanReturn run(const CharacterStatus& cs);
			int th;
		};
		class Plan_EnemyBehind : public ActionPlan
		{
		public:
			Plan_EnemyBehind() : th(96){}
			Plan_EnemyBehind(int th) : th(th){}
			virtual ActionPlanReturn run(const CharacterStatus& cs);
			int th;
		};
		class Plan_BulletGoingToHit : public ActionPlan
		{
		public:
			Plan_BulletGoingToHit() : dt(10) {}
			Plan_BulletGoingToHit(int dt) : dt(dt) {}
			virtual ActionPlanReturn run(const CharacterStatus& cs);
			int dt;
		};
		class Plan_NearFriend : public ActionPlan
		{
		public:
			Plan_NearFriend() : th(64) {}
			Plan_NearFriend(int th) : th(th) {}
			virtual ActionPlanReturn run(const CharacterStatus& cs);
			int th;
		};
		class Plan_Cast : public ActionPlan
		{
		public:
			Plan_Cast() :cursor(0) {}
			Plan_Cast(int cursor) : cursor(cursor) {}
			virtual ActionPlanReturn run(const CharacterStatus& cs);
			int cursor;
		};
		class Plan_Turn : public ActionPlan
		{
		public:
			virtual ActionPlanReturn run(const CharacterStatus& cs);
		};
		class Plan_Jump : public ActionPlan
		{
		public:
			virtual ActionPlanReturn run(const CharacterStatus& cs);
		};
		class Plan_HPGreaterThan : public ActionPlan
		{
		public:
			Plan_HPGreaterThan() :th(25) {}
			Plan_HPGreaterThan(int th) : th(th) {}
			virtual ActionPlanReturn run(const CharacterStatus& cs);
			int th;
		};
		class Plan_ManaGreaterThan : public ActionPlan
		{
		public:
			Plan_ManaGreaterThan() :th(25) {}
			Plan_ManaGreaterThan(int th): th(th) {}
			virtual ActionPlanReturn run(const CharacterStatus& cs);
			int th;
		};
		class Plan_NumOfFriendsGreaterThan : public ActionPlan
		{
		public:
			Plan_NumOfFriendsGreaterThan(int th) : th(th) {}
			virtual ActionPlanReturn run(const CharacterStatus& cs);
			int th;
		};
		class Plan_MarkovChain : public ActionPlan
		{
		public:
			Plan_MarkovChain():state(0)
			{
				probs[0][0] = 0;
				probs[0][1] = 1;
				probs[1][0] = 0.1;
				probs[1][1] = 0.9;
			}
			Plan_MarkovChain(const std::map<int, std::map<int, double>>& probs) :state(0)
			{
				this->probs = probs;
			}
			virtual ActionPlanReturn run(const CharacterStatus& cs);
			int state;
			std::map<int, std::map<int, double>> probs;
		};

		class AI_Planned : public AI
		{
		public:
			AI_Planned();

			~AI_Planned();
			void initialize();
			virtual void initialize(const std::string& data);
			Feature processMyStatus(CharacterStatus cs);
			void think(const CharacterStatus& cs);
			void generateNewPlan(){};
		private:
			Feature f_prev;
			//AICommand command;
			std::deque<ActionPlan*> plan;
			AICommand command;
		};
	};
};