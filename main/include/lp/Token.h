#pragma once
#include<string>
#include<ostream>

namespace lp{class Token;};
//std::ostream& operator <<(std::ostream& ost, const lp::Token& tok);
std::ostream& operator <<(std::ostream& ost, lp::Token const & tok);

namespace lp
{
	enum TokenType{CONST, STRING_LITERAL, OPERATOR, BRACKET, BRACKET_CLOSE, FUNCTION, CONTROL, VALIABLE, SEMICOLON, NODE};
	class Token
	{
		//friend std::ostream& operator <<(std::ostream& ost, const lp::Token& tok);
	public:
		Token(){}
		Token(const std::string& str);
		Token(const std::string& str, TokenType Type);
		TokenType Type()const{return type;}
		int size()const{return str.size();}
		//operator std::string()const{return str;}
		std::string Str()const{return str;}
		static TokenType checkTokenType(const std::string& str);
	private:
		std::string str;
		TokenType type;
	};
};
