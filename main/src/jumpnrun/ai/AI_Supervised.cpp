#include"jumpnrun/ai/AI_Supervised.h"
#include"jumpnrun/ai/FeatureExtractor.h"
#include"jumpnrun/system/Parmanent.h"
#include"utils/MyCppUtil.h"
#include"utils/vecutil.h"

#include<math.h>

using namespace gmtr;
using namespace gfw;
using namespace jnr;
using namespace ai;

static double uniform()
{
	return (double)$P.rand.get() / RAND_MAX;
}

static int accumurate(const std::vector<int>& d)
{
	double sum = 0;
	for (int i = 0; i < d.size(); sum += d[i++]);
	double u = uniform();
	double acc = 0.0;
	for (int i = 0; i < d.size(); i++)
	{
		acc += (double)d[i] / sum;
		if (acc >= u)
			return i;
	}
	return d.size() - 1;
}

AI_Supervised::AI_Supervised(){}

AI_Supervised::~AI_Supervised(){}
void AI_Supervised::initialize()
{
	using namespace myutil;
	auto data = $P.aisuperviser.read("AISupervision\\supervision1.csv");
	Feature f_prev;
	for (auto de : data)
	{
		if (f_prev.size() == 0)
		{
			f_prev = de.f;
			continue;
		}
		auto df = de.f - f_prev;
		$for_i(df.size())
			df[i] = fabs(df[i]);
		if (f2act.count(df))
		{
			f2act[df][de.b] += 1;
		}
		else
		{
			f2act.insert(std::map<Feature, myutil::Histogram<int, int>>::value_type(df, myutil::Histogram<int, int>()));
			f2act[df][de.b] += 1;
		}
	}
	command = 0;
}
Feature AI_Supervised::processMyStatus(CharacterStatus cs)
{
	FeatureExtractor fex;
	return fex.extract(cs);
}
void AI_Supervised::think(const CharacterStatus& cs)
{
	using namespace myutil;
	Feature f = processMyStatus(cs);
	if (f_prev.size() == 0 || myutil::l0(f - f_prev))
	{
		if (f_prev.size() == 0)
			f_prev = f;
		auto df = f - f_prev;
		$for_i(df.size())
			df[i] = fabs(df[i]);
		if (f2act.count(df))
		{
			myutil::Histogram<int, int>& histogram = f2act[df];
			auto keyv = histogram.keyvec();
			auto valv = histogram.valvec();
			int index = accumurate(valv);
			command = keyv[index];
		}
		f_prev = f;
	}
	decodeButtons(command);
}

void AI_Supervised::decodeButtons(int i)
{
	mc.push_up = i & eButton::Up;
	mc.push_right = i & eButton::Right;
	mc.push_down = i & eButton::Down;
	mc.push_left = i & eButton::Left;
	mc.push_z = i & eButton::Z;
	mc.push_x = i & eButton::X;
}