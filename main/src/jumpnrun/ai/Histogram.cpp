#include"jumpnrun/ai/Histogram.h"
#include"utils/string_util.h"
#include<sstream>
#include<fstream>
using namespace mymath;

NumericHistogram::NumericHistogram(): bin_width (0.1){}
double NumericHistogram::add(double k, double dv)
{
	int index =  k / bin_width;
	if (histogram.size() > index)
	{
		histogram[index] += dv;
		return histogram[index];
	}
	else
	{
		histogram.resize(index + 1);
		histogram[index] = dv;
		return dv;
	}
}
double NumericHistogram::get(double k)const
{
	int index = k / bin_width;
	if (histogram.size() > index)
	{
		return histogram[index];
	}
	else
	{
		return 0;
	}
}

std::string NumericHistogram::toString()const
{
	std::stringstream ss;
	ss << "binwidth=" << bin_width << std::endl;
	ss << "data=";
	for (int i = 0; i < histogram.size(); i++)
	{
		ss << histogram[i] << ",";
	}
	return ss.str();
}

bool NumericHistogram::read(const std::string& file_name)
{
	std::ifstream ifs(file_name);
	if (!ifs)
	{
		return false;
	}

	char buf[10240];
	std::string line;
	while (std::getline(ifs, line))
	{
		line = myutil::chomp(line);
		auto tokens = myutil::split(line, "=");
		if (tokens[0] == "binwidth")
		{
			this->bin_width = myutil::str2double(tokens[1]);
		}
		else if (tokens[0] == "data")
		{
			histogram.clear();
			auto v = myutil::split(tokens[1], ",");
			histogram.resize(v.size());
			for (int i = 0; i < v.size(); i++)
			{
				histogram[i] = myutil::str2double(v[i]);
			}
		}
	}
	return true;
}