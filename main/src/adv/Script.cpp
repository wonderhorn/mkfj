#include"adv/Script.h"
#include"utils/string_util.h"
using namespace adv;

Script::Script()
{
	tokens = { "summon", "chara1", "-320", "0", ";"
		, "move", "chara1", "8", "0", "40", ";"
		, "wait", "50", ";"
		, "speak", "本日は晴天なり\n晴天なり\n晴天なり", ";"
		, "emote", "chara1", "astonished", ";"
		, "move", "chara1", "0", "-1", "6", ";"
		, "move", "chara1", "0", "1", "6", ";"
		, "move", "chara1", "0", "-1", "6", ";"
		, "move", "chara1", "0", "1", "6", ";"
		, "speak", "本日は晴天なり2\n晴天なり\n晴天なり", ";"
		, "move", "chara1", "-8", "0", "40", ";"
		, "wait", "50", ";"
		, "end", ";" };
}

void Script::clear()
{
	cursor = 0;
	tokens.clear();
}

bool Script::parseFromFile(const std::string& filename)
{
	clear();
	return true;
}

bool Script::parseFromString(const std::string& src)
{
	clear();
	auto lines = myutil::split(src, ";");
	for (auto& line : lines)
	{
		line = myutil::chomp(line);
		auto tokens = myutil::split(line, " ");
		for (auto& t: tokens)
			this->tokens.push_back(t);
		this->tokens.push_back(";");
	}
	return true;
}


bool Script::goNext()
{
	while (*this->getCurrent() != ";")
	{
		cursor++;
		if (tokens.size() <= cursor)
			return false;
	}
	cursor++;
	if (tokens.size() <= cursor)
		return false;
	return true;
}