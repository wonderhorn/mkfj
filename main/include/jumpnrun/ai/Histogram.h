#pragma once
#include<map>
#include<vector>
namespace mymath
{
	class NumericHistogram
	{
	public:
		NumericHistogram();
		double add(double k, double dv);
		double get(double k)const;
		double bin_width;
		const std::vector<double> vec(){ return histogram; }
		std::string toString()const;
		bool read(const std::string& file_name);

	private:
		std::vector<double> histogram;
	};
};