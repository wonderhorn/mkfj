#include"lp/Token.h"
using namespace lp;

Token::Token(const std::string& str): str(str)
{
	type = checkTokenType(str);
}

Token::Token(const std::string& str, TokenType type): str(str), type(type){}

static bool isDigits(const std::string& str)
{
	for(int i = 0; i < str.size(); i++)
	{
		char c = str[i];
		if(!('0' <= c && c <= '9'))
			return false;
	}
}

static bool isNumerical(const std::string& str)
{
	if(str.size() == 0)
		return 0;
	return isDigits(str) || (str[0] == '-' && str.size() > 1 && isDigits(str.substr(2, -1)));
}


TokenType Token::checkTokenType(const std::string& str)
{
	if(str == ";")
		return SEMICOLON;
	else if(isNumerical(str))
		return CONST;
	else if(str[0] == '\"')
		return STRING_LITERAL;
	else if(str == "if" || str == "while" || str == "return")
		return CONTROL;
	else if(str == "(" || str == "{" || str == "[")
		return BRACKET;
	else if(str == ")" || str == "}" || str == "]")
		return BRACKET_CLOSE;
	else if(str == "=" || str == "+" || str == "-" || str == "*" || str == "/" || str == "&" || str == "|" || str == "&&" || str == "||" || str == "==" || str == "!=")
		return OPERATOR;
	else return VALIABLE;
}

std::ostream& operator <<(std::ostream& ost, lp::Token const & tok)
{
	return ost << tok.Str();
}