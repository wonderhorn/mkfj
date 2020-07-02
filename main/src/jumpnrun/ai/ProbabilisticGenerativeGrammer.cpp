#include"jumpnrun/ai/ProbabilisticGenerativeGrammer.h"
#include"utils/string_util.h"
#include<iostream>
#include<stdlib.h>

using namespace lp;

int sample_from_dist(const std::vector<double>& v)
{
	double sum = 0;
	for (double d : v)
		sum += d;
	double u = (double)rand() / RAND_MAX;
	double accum = 0;
	for (int i = 0; i < v.size(); i++)
	{
		accum += v[i] / sum;
		if (u < accum)
			return i;
	}
	return v.size() - 1;
}

void ProbabilisticGenerativeGrammer::addRule(const std::string& src, const std::string& dst, double w)
{
	if (!probs.count(src))
	{
		probs.insert(GenerationProbabilities::value_type(src
			, std::tuple<std::vector<std::string>
			, std::vector<double >> (std::vector<std::string>(), std::vector<double>())));
	}
	std::get<0>(probs[src]).push_back(dst);
	std::get<1>(probs[src]).push_back(w);
}

std::string ProbabilisticGenerativeGrammer::generate(const std::string& src)
{
	std::string rtn = src;
	bool finished = true;
	for (auto token : nonterminals)
	{
		//int i = rtn.find_first_of(token);
		int i = -1;
		for (int j = 0; j <= rtn.size() - token.size(); j++)
		{
			auto subs = rtn.substr(j, token.size());
			if (subs == token)
				i = j;
		}
		if (i != -1)
		{
			const std::vector<double>&prob = std::get<1>(probs[token]);
			int j = sample_from_dist(prob);
			const std::vector<std::string>& rights = std::get<0>(probs[token]);
			//std::cout << "\"" << rtn << "\"-> ";
			rtn.replace(i, token.size(), rights[j]);
			//std::cout << "\"" << rtn << "\"" << std::endl;
			finished = false;
		}
	}
	if (!finished)
		return generate(rtn);
	else return rtn;
}