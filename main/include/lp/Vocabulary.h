#include<map>
#include<string>
#include<vector>
namespace lp
{
	class Vocabulary
	{
	public:
		Vocabulary();
		void add(const std::string& str);
		std::string operator[](int i);
		int operator[](const std::string& str);
	private:
		std::map<std::string, int> dictionary;
		std::vector<std::string> vocabulary;
		int num_entry;
	};
};