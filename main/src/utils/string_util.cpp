#include"utils/string_util.h"
#include<stdlib.h>
#include<string.h>
//using namespace string_utilities;
using namespace std;

std::string myutil::int2str(int i)
{
	std::strstream ss;
	ss << i << '\0';
	std::string rtn = ss.str();
	return rtn;
}
int myutil::str2int(const std::string& str)
{
	return atoi(str.c_str());
}

double myutil::str2double(const std::string& str)
{
	return atof(str.c_str());
}

int myutil::find(const std::string& str, int ori, int ter, const std::string& target)
{
	if(ter < 0)
		ter = str.size();
	while(str.compare(ori, target.size(), target, 0, target.size()) != 0)
	{
		if(ori + target.size() >= ter + 1)
			return -1;
		ori++;
	}
	return ori;
}

std::string myutil::replace(const std::string &str, const std::string& from, const std::string& to)
{
	std::string rtn(str);
	unsigned int pos = 0; 
	while (!(pos == std::string::npos || from.empty()))
	{
		pos = rtn.find(from);
		if ((pos == std::string::npos || from.empty()))
			break;
		const int len = from.length();
		rtn = rtn.replace(pos, len, to);
	}
	return rtn;
}


int myutil::countWord(const std::string& str, int ori, int ter, const std::string& target)
{
	int rtn = 0;
	int pos = 0;
	while ((pos = myutil::find(str, pos, -1, target)) != -1)
	{
		rtn++;
		pos++;
	}
	return rtn;
}


std::vector<std::string> myutil::split(const std::string& str, const std::string& delim)
{
	vector<string> rtn;
	int pos_ori = 0;
	int pos_ter = -1;
	while ((pos_ter = myutil::find(str, pos_ori, -1, delim)) != -1)
	{
		rtn.push_back(str.substr(pos_ori, pos_ter - pos_ori));
		//pos_ori = pos_ter + 1;
		pos_ori = pos_ter + delim.length();
	}
	rtn.push_back(str.substr(pos_ori, -1));
	return rtn;
}

std::string myutil::chomp(const std::string& str)
{
	if (str.size() == 0)
		return str;
	string rtn = str;
	if (rtn[0] == ' ')
	{
		rtn = rtn.substr(1, -1);
		return chomp(rtn);
	}
	if (rtn[rtn.size() - 1] == '\n' || rtn[rtn.size() - 1] == ' ')
	{
		rtn = rtn.substr(0, rtn.size() - 1);
		return chomp(rtn);
	}
	return rtn;
}

int myutil::getMultiByteCharLength(char c)
{
	unsigned char uc = c;
	if (uc < 128)
		return 1;
	else if (uc < 224)
		return 2;
	else if (uc < 240)
		return 3;
	else return 4;
}