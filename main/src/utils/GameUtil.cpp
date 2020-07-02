#include"framework/GameUtil.h"

std::string gfw::num2str(int n)
{
	std::strstream ss;
	ss << n << std::ends;
	return ss.str();
}