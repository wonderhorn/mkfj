#include"lp/Tokenizer.h"

using namespace lp;

Tokenizer::Tokenizer(){}

void Tokenizer::tokenize()
{
	int pos = 0;
	for(int i = 0; i < sentense.length(); i++)
	{
		if(sentense[i] == '\n')
		{
			if(i - pos > 0)
			{
				std::string token = sentense.substr(pos, i - pos);
				tokens.push_back(token);
				tokens.push_back("CRETURN");
			}
			pos = i + 1;
		}
		if(sentense[i] == ' ' || sentense[i] == '\t' ||  sentense[i] == '\0')
		{
			if(i - pos > 0)
			{
				std::string token = sentense.substr(pos, i - pos);
				tokens.push_back(token);
			}
			pos = i + 1;
		}
		else if(sentense[i] == '(' || sentense[i] == ')' || sentense[i] == ';'
			 || sentense[i] == '+' || sentense[i] == '-' || sentense[i] == '*' || sentense[i] == '/')
		{
			if(i - pos > 0)
			{
				std::string token = sentense.substr(pos, i - pos);
				tokens.push_back(token);
			}
			std::string token = sentense.substr(i, 1);
			tokens.push_back(token);
			pos = i + 1;
		}
		else if(sentense[i] == '\"' && !(i == 0 || sentense[i - 1] == '\\'))
		{
			for(int j = i + 1; j < sentense.length(); j++)
			{
				if(sentense[j] == '\"' && !(j == 0 || sentense[j - 1] == '\\'))
				{
					std::string token = sentense.substr(i, j - i + 1);
					tokens.push_back(token);
					pos = j + 1;
					i = j;
					break;
				}
			}
		}
	}
	std::string token = sentense.substr(pos, -1);
	tokens.push_back(token);
}