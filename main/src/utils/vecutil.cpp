#include"utils/vecutil.h"
#include<math.h>
using namespace myutil;

double myutil::sqsum(const std::vector<double>& v)
{
	double rtn = 0.0;
	for (int i = 0; i < v.size(); i++)
	{
		rtn += v[i] * v[i];
	}
	return rtn;
}
double myutil::l2(const std::vector<double>& v)
{
	return sqrt(sqsum(v));
}
double myutil::l0(const std::vector<double>& v)
{
	double rtn = 0.0;
	for (double d : v)
	{
		if (fabs(d) >= EPSILON)
			rtn += 1.0;
	}
	return rtn;
}
std::vector<double> myutil::operator + (const std::vector<double>& v1, const std::vector<double>& v2)
{
	std::vector<double> rtn(v1.size());
	for (int i = 0; i < v1.size(); i++)
	{
		rtn[i] = v1[i] + v2[i];
	}
	return rtn;
}
std::vector<double> myutil::operator - (const std::vector<double>& v1, const std::vector<double>& v2)
{
	std::vector<double> rtn(v1.size());
	for (int i = 0; i < v1.size(); i++)
	{
		rtn[i] = v1[i] - v2[i];
	}
	return rtn;
}
std::vector<double> myutil::operator += (std::vector<double>& v1, const std::vector<double>& v2)
{
	for (int i = 0; i < v1.size(); i++)
	{
		v1[i] = v1[i] + v2[i];
	}
	return v1;
}
std::vector<double> myutil::operator -= (std::vector<double>& v1, const std::vector<double>& v2)
{
	for (int i = 0; i < v1.size(); i++)
	{
		v1[i] = v1[i] - v2[i];
	}
	return v1;
}
std::vector<double> myutil::operator * (const std::vector<double>& v, double d)
{
	std::vector<double> rtn(v.size());
	for (int i = 0; i < v.size(); i++)
	{
		rtn[i] = v[i] * d;
	}
	return rtn;
}
std::vector<double> myutil::operator / (const std::vector<double>& v, double d)
{
	std::vector<double> rtn(v.size());
	for (int i = 0; i < v.size(); i++)
	{
		rtn[i] = v[i] / d;
	}
	return rtn;
}
std::vector<double> myutil::operator * (double d, const std::vector<double>& v)
{
	return v * d;
}

double myutil::operator * (const std::vector<double>& v1, const std::vector<double>& v2)
{
	double rtn = 0.0;
	for (int i = 0; i < v1.size(); i++)
	{
		rtn += v1[i] * v2[i];
	}
	return rtn;
}