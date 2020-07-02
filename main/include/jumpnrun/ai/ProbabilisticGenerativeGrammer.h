#include<string>
#include<vector>
#include<map>
#include<tuple>

typedef std::map<std::string, std::tuple<std::vector<std::string>, std::vector<double>>> GenerationProbabilities;
namespace lp
{
	class ProbabilisticGenerativeGrammer
	{
	public:
		std::string generate(const std::string& src);
		void addRule(const std::string& src, const std::string& dst, double w);
		GenerationProbabilities probs;
		std::vector<std::string> nonterminals;
	};
};