#include<string>

namespace jnr
{
	namespace spl
	{
		class SpellName2Text
		{
		public:
			static std::string Name(const std::string& str);
			static std::string Text(const std::string& str);
		};
	};
};