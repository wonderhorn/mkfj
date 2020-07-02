#pragma once
#include<vector>
#include<fstream>
#define EPSILON 0.01
namespace myutil
{
	double sqsum(const std::vector<double>& v);
	double l2(const std::vector<double>& v);
	double l0(const std::vector<double>& v);
	std::vector<double> operator + (const std::vector<double>& v1, const std::vector<double>& v2);
	std::vector<double> operator - (const std::vector<double>& v1, const std::vector<double>& v2);
	std::vector<double> operator += (std::vector<double>& v1, const std::vector<double>& v2);
	std::vector<double> operator -= (std::vector<double>& v1, const std::vector<double>& v2);
	std::vector<double> operator * (const std::vector<double>& v, double d);
	std::vector<double> operator / (const std::vector<double>& v, double d);
	std::vector<double> operator * (double d, const std::vector<double>& v);
	double operator * (const std::vector<double>& v1, const std::vector<double>& v2);

	template <class T> T joint(const T& v1, const T& v2)
	{
		T rtn(v1.size() + v2.size());
		for (int i = 0; i < v1.size(); i++)
		{
			rtn[i] = v1[i];
		}
		for (int i = v1.size(); i < v1.size() + v2.size(); i++)
		{
			rtn[i] = v2[i];
		}
		return rtn;
	}

	template <class T> std::ostream& operator << (std::ostream& ost, const std::vector<T>& v)
	{
		ost << "[";
		for (auto i = v.begin(); i != v.end(); i++)
		{
			if (i != v.begin())
				ost << ",";
			ost << *i;
		}
		ost << "]";
		return ost;
	}
}