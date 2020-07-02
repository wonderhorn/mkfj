#include<vector>
#include<string>
#define CRETURN "%%CRETURN"
namespace lp
{
	class Tokenizer
	{
	public:
		Tokenizer();
		void Sentense(const std::string& sentense){this->sentense = sentense;}
		void tokenize();
		const std::vector<std::string>& Tokens(){return tokens;}
	private:
		std::string sentense;
		std::vector<std::string> tokens;
	};
};