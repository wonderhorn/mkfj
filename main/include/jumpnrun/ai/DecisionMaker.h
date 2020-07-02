#pragma once
#include"jumpnrun/ai/AI.h"
#include<map>
namespace jnr
{
	namespace ai
	{
		class DecisionMaker
		{
		public:
			virtual eAction decide(const Feature& f) = 0;
			virtual void train(const Feature& f, eAction action, double score){}
		};

		class ConstDecision : public DecisionMaker
		{
		public:
			ConstDecision(eAction action);
			eAction decide(const Feature& f);
			eAction action;
		};

		class Voting : public DecisionMaker
		{
		public:
			Voting();
			eAction decide(const Feature& f);
			void train(const Feature& f, eAction action, double score);
			std::vector<DecisionMaker*> children;
			class Log
			{
			public:
				Feature f;
				int vote;
				int score;
			};
			std::vector<Log> logs;
			std::vector<std::vector<Log>> logs_children;
			std::vector<Feature> f_children;
			int last_vote;
			double omega, alpha;

			void addChildren(DecisionMaker* c);
		};

		class LinearSeparator : public DecisionMaker
		{
			LinearSeparator();
			eAction decide(const Feature& f);
			std::vector<double> w;
			double b;
			DecisionMaker* upper, *lower;
		};

		class NearestNeighbor : public DecisionMaker
		{
		public:
			NearestNeighbor();
			eAction decide(const Feature& f);
			void train(const Feature& f, eAction action, double score);
		};

		class WeightDecisionList : public DecisionMaker
		{
		public:
			WeightDecisionList();
			class ListElement
			{
			public:
				ListElement(int f, eAction action, double weight) : f(f), action(action), weight(weight){}
				int f;
				eAction action;
				double weight;
			};
			std::vector<ListElement> list;
			eAction decide(const Feature& f);
			void train(const Feature& f, eAction action, double score);
		};
	};
};