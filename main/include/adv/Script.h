#pragma once
#include<vector>
#include<string>
#include"lp/Tokenizer.h"
namespace adv
{
	class Script
	{
	public:
		Script();
		void clear();
		bool parseFromFile(const std::string& filename);
		bool parseFromString(const std::string& src);
		std::vector<std::string>::iterator getCurrent(){ return tokens.begin() + cursor; }
		std::vector<std::string>::const_iterator end()const { return tokens.end(); }
		bool goNext();

	private:
		std::vector<std::string> tokens;
		int cursor;
		lp::Tokenizer tokenizer;

	};
};