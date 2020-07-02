#include"jumpnrun/ai/AITrainer.h"
#include"utils/MyCppUtil.h"
#include<math.h>
using namespace jnr;
using namespace ai;

AITrainer::AITrainer(){}

double AITrainer::kernel(const Feature& f1, const Feature& f2)
{
	/*if (f1[0] != f2[0])
		return 0.0;
	return 1.0;*/
	const double eps = 0.1;
	$for_i(f1.size())
	{
		double df = f1[i] - f2[i];
		if (fabs(df) > eps)
		{
			return 0.0;
		}
	}
	return 1.0;
}

double AITrainer::predict(const Feature& f, char command, int delay)
{
	int sum_c = 0;
	double ave = 0.0;
	for (int i = 0; i < features.size(); i++)
	{
		if (command == commands[i] && this->kernel(f, features[i]))
		//if (f == features[i] && command == commands[i])
		{
			double sum = 0.0;
			for (int j = i; j < features.size() && j - i < delay; j++)
				sum += scores[j];
			sum /= delay;
			sum_c++;
			ave += sum;
		}
	}
	if (sum_c != 0)
		ave /= sum_c;
	return ave;
}

void AITrainer::push(const Feature& f, char command, double score)
{
	features.push_back(f);
	commands.push_back(command);
	scores.push_back(score);
}