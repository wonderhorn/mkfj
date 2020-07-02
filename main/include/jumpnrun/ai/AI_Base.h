#pragma once
#include"jumpnrun/ai/AI.h"
#include"jumpnrun/ai/FeatureExtractor.h"
#include"jumpnrun/ai/CharacterStatus.h"

#include<vector>
#include<string>
typedef std::vector<double> Feature;

typedef int Decision;

namespace jnr
{
	namespace ai
	{
		class AIFeature
		{
		public:
			AIFeature();
			std::vector<bool> bools;
			std::vector<double> doubles;
		};

		class AIModule
		{
		public:
			AIModule(): name("noname"){}
			virtual Decision decide(const CharacterStatus& cs) = 0;
			virtual void train(const CharacterStatus& cs, int score){}
			std::string name;
		};
		class AI_Base: public AI
		{
		public:
			AI_Base();

			~AI_Base();
			void initialize();
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
			static Command randomCommand(const Feature& f);

			std::deque<Command> commands;
			std::deque<eAction> actions;
			eAction current_action;
			Feature feature;
			CharacterStatus status_prev;
			std::vector<Feature> log_f;
			//void saveFeatureLog();

			class ActionController
			{
			public:
				ActionController(AI_Base& ai);
				Command act(eAction action, const CharacterStatus& cs);
			private:
				AI_Base& ai;
				bool turned;
			};
			ActionController action_cont;
			std::vector<AIModule*> mods;
			AIModule* mods_root;
		};
	};
};