#pragma once
#include"framework/MediaAlternative.h"
#include<map>
#include<string>
#include<assert.h>

namespace adv
{
	class CharacterImageSet
	{
	public:
		ImageHandler operator [](const std::string str){ assert(m.count(str)); return m[str]; }
		void insert(const std::string& name, ImageHandler img)
		{ m.insert(std::map<std::string, ImageHandler>::value_type(name, img)); }

		std::string name;
		std::map<std::string, ImageHandler> m;
	};
};