#include"utils/MyIO.h"
#include<iostream>
#include<fstream>
using namespace std;
using namespace myutil;

std::vector<std::vector<std::string>> myutil::parseCSV(const std::string& file_name)
{
	vector<vector<string>> rtn;
	ifstream ifs(file_name);
	if (!ifs)
	{
		cout << "no file: " << file_name << endl;
		return rtn;
	}

	string line;
	while (getline(ifs, line))
	{
		vector<string> v = split(line, ",");
		rtn.push_back(v);
	}
	return rtn;
}