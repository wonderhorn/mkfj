#pragma once
#include<string>
#include<map>
#include"tree/tree.h"
#include"lp/Token.h"
namespace lp
{
	typedef int Value;
	typedef std::map<std::string, Value> Valiables;
	class Executer
	{
	public:
		Executer(space_tree::tree<Token>& syntax);
		void execute();

	private:
		space_tree::tree<Token>& syntax;
		Value evaluate(space_tree::tree<Token>::iterator it);
		Value evaluate_operator(space_tree::tree<Token>::iterator it);

		Valiables valiables;
	};
};