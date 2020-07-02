#include"lp/Vocabulary.h"

using namespace lp;

Vocabulary::Vocabulary(): num_entry(0){}

void Vocabulary::add(const std::string& str)
{
	if(dictionary.count(str) == 0)
	{
		dictionary.insert(std::map<std::string, int>::value_type(str, num_entry++));
		vocabulary.push_back(str);
	}
}