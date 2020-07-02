#include"utils/Sprine.h"
using namespace gmtr;
Sprine::Sprine(): length(0){}

bool Sprine::push_back(const Vector2D& p)
{
	if (length >= max_length)
		return false;
	else
	{
		points[length] = p;
		length++;
	}
}