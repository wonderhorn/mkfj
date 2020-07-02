#include"lp/Executer.h"

using namespace lp;

Executer::Executer(space_tree::tree<Token>& syntax)
	:syntax(syntax){}

void Executer::execute()
{
	space_tree::tree<Token>::iterator it = syntax.Root();
	evaluate(it);
}

Value Executer::evaluate(space_tree::tree<Token>::iterator it)
{
	switch(it->Type())
	{
	case CONTROL:
		return evaluate_operator(it);
		break;
	case TokenType::CONST:
		return atoi(it->Str().c_str());
		break;
	case TokenType::VALIABLE:
		return valiables[it->Str()];
		break;
	case TokenType::OPERATOR:
		return evaluate_operator(it);
		break;
	default:
		for(int i = 0; i < it.ChildNum(); i++)
		{
			it.goToChild(i);
			evaluate(it);
			it.goToParent();
		}
	}
	return 0;
}

Value Executer::evaluate_operator(space_tree::tree<Token>::iterator it)
{
	if(it->Str() == "=")
	{
		space_tree::tree<Token>::iterator left = it;
		left.goToChild(0);
		space_tree::tree<Token>::iterator right = it;
		right.goToChild(1);
		Value rtn = evaluate(right);
		std::cout << *left << " = " << rtn << std::endl;
		valiables.insert(Valiables::value_type(left->Str(), rtn));
		return rtn;
	}
	else if(it->Str() == "+")
	{
		space_tree::tree<Token>::iterator left = it;
		left.goToChild(0);
		space_tree::tree<Token>::iterator right = it;
		right.goToChild(1);
		Value rtn = evaluate(left) + evaluate(right);
		return rtn;
	}
	else if(it->Str() == "*")
	{
		space_tree::tree<Token>::iterator left = it;
		left.goToChild(0);
		space_tree::tree<Token>::iterator right = it;
		right.goToChild(1);
		Value rtn = evaluate(left) * evaluate(right);
		return rtn;
	}
	else if(it->Str() == "if")
	{
		space_tree::tree<Token>::iterator left = it;
		left.goToChild(0);
		space_tree::tree<Token>::iterator right = it;
		right.goToChild(1);
		if(right != it && evaluate(left))
			evaluate(right);
	}
	return 0;
}