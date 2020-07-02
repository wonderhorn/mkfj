#include"jumpnrun/system/Saver.h"
#include"utils/string_util.h"
#include<fstream>
#include<iostream>
using namespace gfw;

Saver::Saver(){}

std::string Saver::get(const std::string key)
{
	if (data.count(key) == 0)
		return "";
	else return data[key];
}

std::vector<std::string> Saver::getKeys()
{
	std::vector<std::string> rtn;
	for (auto i : data)
	{
		rtn.push_back(i.first);
	}
	return rtn;
}

bool Saver::hasKey(const std::string key)
{
	return data.count(key) != 0;
}

void Saver::set(const std::string& key, const std::string& value)
{
	data[key] = value;
}

bool Saver::save(const std::string& file_name)
{
	std::ofstream ofs(file_name);
	if (!ofs)
	{
		std::cout << "Saver::save could not open " << file_name << std::endl;
		return false;
	}
	for (auto i : data)
	{
		ofs << i.first << "=" << i.second << std::endl;
	}
	ofs.close();
	return true;
}

bool Saver::load(const std::string& file_name)
{
	data.clear();
	std::ifstream ifs(file_name);
	if (!ifs)
	{
		std::cout << "Saver::load could not open " << file_name << std::endl;
		return false;
	}
	std::string line;
	while (std::getline(ifs, line))
	{
		auto tokens = myutil::split(line, "=");
		if (tokens.size() == 2)
		{
			data[myutil::chomp(tokens[0])] = myutil::chomp(tokens[1]);
		}
	}
	return true;
}