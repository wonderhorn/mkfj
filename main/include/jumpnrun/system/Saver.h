#include<map>
#include<string>
#include<vector>
typedef std::map<std::string, std::string> SaveData;
namespace gfw
{
	class Saver
	{
	public:
		Saver();
		std::string get(const std::string key);
		std::vector<std::string> getKeys();
		bool hasKey(const std::string key);
		void set(const std::string& key, const std::string& value);
		bool save(const std::string& file_name);
		bool load(const std::string& file_name);
		SaveData data;
	};
};