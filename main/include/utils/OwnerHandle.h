#pragma once

class OWNERHANDLE
{
public:
	OWNERHANDLE(){value = -1;}
	OWNERHANDLE(char c){value = c;}
	char value;

	operator char(){return value;}

	bool operator ==(const OWNERHANDLE& oh)const
	{
		return (this->value >= 0)  && (oh.value >= 0) && (this->value == oh.value);
	}
	bool operator !=(const OWNERHANDLE& oh)const
	{
		return !(*this == oh);
	}
};