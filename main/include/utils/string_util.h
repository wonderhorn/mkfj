#pragma once
#include<string>
#include<vector>
#include<map>
#include<strstream>

namespace myutil
{
	std::string int2str(int i);
	int str2int(const std::string& str);
	double str2double(const std::string& str);
	template <class T_src, class T_dst> std::string map2str(const std::map<T_src, T_dst>& m);
	int find(const std::string& str, int ori, int ter, const std::string& target);
	std::string replace(const std::string &str, const std::string& from, const std::string& to) ;
	int countWord(const std::string& str, int ori, int ter, const std::string& target);
	std::vector<std::string> split(const std::string& str, const std::string& delim);
	std::string chomp(const std::string& str);
	int getMultiByteCharLength(char c);

	template <class T_src, class T_dst> std::string map2str(const std::map<T_src, T_dst>& m)
	{
		std::strstream ss;
		for (auto i : m)
		{
			ss << m->first << ": " << m->second << std::endl;
		}
	}
};