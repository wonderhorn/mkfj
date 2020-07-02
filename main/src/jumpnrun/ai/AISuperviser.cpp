#include"jumpnrun/ai/AISuperviser.h"
#include"jumpnrun/ai/FeatureExtractor.h"
#include"jumpnrun/ai/AI.h"
#include"utils/string_util.h"
#include<fstream>
using namespace gmtr;
using namespace gfw;
using namespace jnr;
using namespace ai;

AISuperviser::AISuperviser(){}

void AISuperviser::push(const Feature& f, int score, int decision)
{
	data.push_back(DataElement(f, score, decision));
}

void AISuperviser::push(const CharacterStatus& cs, const gfw::VirtualController& vc)
{
	FeatureExtractor fex;
	Feature f = fex.extract(cs);
	int b = NoInput;
	if (vc.Left().isDown())
	{
		b = b | eButton::Left;
	}
	if (vc.Right().isDown())
	{
		b = b | eButton::Right;
	}
	if (vc.Up().isDown())
	{
		b = b | eButton::Up;
	}
	if (vc.Down().isDown())
	{
		b = b | eButton::Down;
	}
	if (vc.Z().isDown())
	{
		b = b | eButton::Z;
	}
	if (vc.X().isDown())
	{
		b = b | eButton::X;
	}

	if (data.size() < 100000)
		data.push_back(DataElement(f, cs.hp, b));
}
void AISuperviser::save(const std::string& filename)const
{
	std::ofstream ofs(filename);
	if (header.size())
		ofs << header << std::endl;
	for (auto e : data)
	{
		for (int i = 0; i < e.f.size(); i++)
		{
			if (i != 0)
				ofs << ",";
			ofs << e.f[i];
		}
		ofs << ",";
		ofs << e.b;
		ofs << ",";
		ofs << e.score;
		ofs << std::endl;
	}
}

void AISuperviser::setHeader(const std::string& str)
{
	header = str;
}

std::vector<AISuperviser::DataElement> AISuperviser::read(const std::string& filename)const
{
	std::vector<AISuperviser::DataElement> rtn;
	std::ifstream ifs(filename);
	if (!ifs)
	{
		return rtn;
	}

	std::string line;
	while (std::getline(ifs, line))
	{
		Feature f;
		auto tokens = myutil::split(line, ",");
		int n = tokens.size();
		for (int i = 0; i < n - 2; i++)
		{
			f.push_back(myutil::str2double(tokens[i]));
		}
		DataElement de(f, myutil::str2double(tokens[n - 1]), myutil::str2double(tokens[n - 2]));
		rtn.push_back(de);
	}
	return rtn;
}