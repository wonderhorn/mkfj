#pragma once
#include"jumpnrun/ai/AI.h"
#include"jumpnrun/ai/FeatureExtractor.h"
#include"jumpnrun/ai/Histogram.h"
#include"jumpnrun/ai/AIActionDecoder.h"
#include"tree/tree.h"
#include<string>
#include<deque>
#include<fstream>
#include<ostream>

namespace jnr
{
	namespace ai
	{
		class AI_Bandit : public AI
		{
		public:
			AI_Bandit();

			~AI_Bandit();
			void initialize();
			Feature processMyStatus(CharacterStatus cs);
			void think(const CharacterStatus& cs);
			int decide(const CharacterStatus& cs);
		private:
			Feature f_prev;
			AICommand command;
			std::deque<double> q_score, q_action;
			mymath::NumericHistogram act2score, act2num;
			int delay_feedback;
			AIActionDecoder actdec;
		};

		class AI_BanditTree : public AI
		{
		public:
			AI_BanditTree();

			~AI_BanditTree();
			void initialize();
			void initialize(space_tree::tree<std::string>& program);
			void initialize(const std::string& src);
			Feature processMyStatus(CharacterStatus cs);
			void think(const CharacterStatus& cs);
			int decide(const CharacterStatus& cs, const Feature& f);
			bool save(const std::string& path);
			bool load(const std::string& path);
			class Node
			{
			public:
				Node() : i(-1), th(0){}
				Node(int i, double th) : i(i), th(th){}
				Node(const std::string& str);
				int i;
				double th;
				//std::ostream& operator << (std::ostream& ost)const;

				//friend std::ostream& operator << (std::ostream& ost, const jnr::ai::AI_BanditTree::Node& n);
			};

		private:
			Feature f_prev;
			AICommand command;
			std::deque<double> q_score, q_action;
			std::deque<int> q_bandit;
			mymath::NumericHistogram act2score, act2num;
			int delay_feedback;
			AIActionDecoder actdec;


			class Bandit
			{
			public:
				mymath::NumericHistogram act2score, act2num;
			};

			space_tree::tree<Node> decision_tree;
			std::vector<Bandit> bandits;
			//eAction doBandit(Bandit& bandit);
			int doTree(const Feature& f);
		};
	};
};


std::ostream& operator << (std::ostream& ost, const jnr::ai::AI_BanditTree::Node& n);
/*{
	ost << n.i << "_" << n.th;
	return ost;
}*/