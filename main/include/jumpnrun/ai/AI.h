#pragma once
#include"framework/VirtualController.h"
#include"jumpnrun/stage/Stage.h"
//#include"CharacterStatus.h"
//#include"Character.h"
#include"jumpnrun/ai/FeatureExtractor.h"
#include<deque>
typedef std::vector<double> Feature;

namespace jnr
{
	namespace ai
	{
		enum eButton{NoInput = 0, Up = 1, Right = 2, Down = 4, Left = 8, Z =16, X = 32};
		enum eAction{Walk, Dash, Turn, Jump, JumpWalk, Attack, Cast, Keep, Wait};
		class DecisionMaker;

		class AIParameters
		{
		public:
			std::vector<int> f;
			std::vector<eAction> action;
			std::vector<double> w;
			static AIParameters random();
		};

		class AI
		{
		public:
			AI();
			virtual void initialize(const std::string& data){}
			virtual void think(const CharacterStatus& cs) = 0;
			virtual gfw::VirtualController& Controller(){ return mc; };
			int CurrentZSpell() { return current_z_spell; }
			int CurrentXSpell() { return current_x_spell;}
		protected:
			gfw::ManualController mc;
			int current_z_spell, current_x_spell;
		};

		class AICommand
		{
		public:
			AICommand();
			class Command
			{
			public:
				Command();
				Command(int limit, char button);
				int age, limit;
				char button;
			};
			class ActionController
			{
			public:
				ActionController();
				//Command act(eAction action, const CharacterStatus& cs);
				void act(std::deque<Command>& commands, eAction action, const CharacterStatus& cs);
			private:
				bool turned;
			};
			ActionController action_cont;
			std::deque<Command> commands;
			std::deque<eAction> actions;
			eAction current_action;
			char com_prev;
			static Command randomCommand(const Feature& f);
			char getCommand(gfw::ManualController& mc, const CharacterStatus& cs);
			void pushAction(eAction a, const CharacterStatus& cs);
			bool empty()const;
		};

		class AI_TEST : public AI
		{
		public:
			AI_TEST();
			AI_TEST(const AIParameters& param);
			~AI_TEST();
			void initialize(const AIParameters& params);
			Feature processMyStatus(CharacterStatus cs);
			void think(const CharacterStatus& cs);

			class StatusMessage
			{
			public:
				char mestype;
				std::vector<double> params;
			};
			class Command
			{
			public:
				Command();
				Command(int limit, char button);
				int age, limit;
				char button;
			};
			Command generateNextCommand(const Feature& f);
			static Command randomCommand(const Feature& f);

			std::deque<Command> commands;
			std::deque<eAction> actions;
			eAction current_action;
			Feature feature;
			CharacterStatus status_prev;
			Command(AI_TEST::*decide)(const Feature& f);
			std::vector<Feature> log_f;
			//void saveFeatureLog();

			class ActionController
			{
			public:
				ActionController(AI_TEST& ai);
				Command act(eAction action, const CharacterStatus& cs);
			private:
				AI_TEST& ai;
				bool turned;
			};
			ActionController action_cont;
			DecisionMaker* decisionmaker;
			std::vector<DecisionMaker*> decisionmaker_buf;
		};
	};
};