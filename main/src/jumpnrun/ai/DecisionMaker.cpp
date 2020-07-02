#include"jumpnrun/ai/DecisionMaker.h"
#include"utils/vecutil.h"
#include"utils/MyCppUtil.h"
#include<float.h>
using namespace jnr;
using namespace ai;
using namespace myutil;


ConstDecision::ConstDecision(eAction action): action(action){}
eAction ConstDecision::decide(const Feature& f){ return action; }

Voting::Voting() : last_vote(0), omega(.002), alpha(0.03){}
eAction Voting::decide(const Feature& f)
{
	std::vector<double> votings(children.size()), num_called(children.size());
	int delay = 20;

	for (int i = 0; i < children.size(); i++)
	{
		votings[i] = 0.0;
		num_called[i] = 0.0;
	}

	for (int i = 0; i < logs.size(); i++)
	{
		Log& l = logs[i];
		for (int j = 0; i + j < logs.size() && j < delay; j++)
		{
			votings[l.vote] += logs[i + j].score;
			num_called[l.vote]++;
		}
	}

	for (int i = 0; i < children.size(); i++)
	{
		votings[i] /= (num_called[i] + 0.01);
	}
	for (int i = 0; i < children.size(); i++)
	{
		votings[i] += omega / (num_called[i] + 0.01);
	}

	int argmax = 0;
	double max = FLT_MIN;
	for (int i = 0; i < votings.size(); i++)
	{
		if (max < votings[i])
		{
			max = votings[i];
			argmax = i;
		}
	}
	last_vote = argmax;
	return children[argmax]->decide(f);

	/*std::vector<double> distances(children.size());
	double d_min = FLT_MAX;
	int argmin = 0;
	for (int i = 0; i < children.size(); i++)
	{
		if (f_children.size() == 0)
		{
			distances[i] = 0;
			d_min = 0;
			argmin = i;
		}
		else
		{
			double d = l2(f_children[i] - f);
			distances[i] = d;
			if (d < d_min)
			{
				d_min = d;
				argmin = i;
			}
		}
	}
	last_vote = argmin;
	return children[argmin]->decide(f);*/
}
void Voting::train(const Feature& f, eAction action, double score)
{
	Log l;
	l.f = f;
	l.vote = last_vote;
	l.score = score;
	logs.push_back(l);
	children[last_vote]->train(f, action, score);
	logs_children[last_vote].push_back(l);

	if (f_children[last_vote].size() == 0)
	{
		f_children[last_vote] = f;
	}
	else if (l2(f) >= 0.00001)
	{
		Feature df = (f - f_children[last_vote]) / l2(f) * alpha * score;
		f_children[last_vote] += df;
	}

	if (score != 0)
	{
		int a = 0;
	}
}

void Voting::addChildren(DecisionMaker* c)
{
	children.push_back(c);
	logs_children.push_back(std::vector<Log>(0));
	f_children.push_back(Feature());
}


LinearSeparator::LinearSeparator(){}
eAction LinearSeparator::decide(const Feature& f)
{
	double d = f * w + b;
	if (d >= 0)
		return upper->decide(f);
	else return lower->decide(f);
}

WeightDecisionList::WeightDecisionList(){}

eAction WeightDecisionList::decide(const Feature& f)
{
	std::vector<double> votes(8);
	for (int i = 0; i < list.size(); i++)
	{
		if (f[list[i].f] != 0.0)
			votes[list[i].action] += list[i].weight;
	}
	assert(1);
	int amax = 0;
	//int amax = myutil::argmax(votes);
	return (eAction)amax;
}

void WeightDecisionList::train(const Feature& f, eAction action, double score)
{}