#include"framework/VirtualController.h"
#include<iostream>
#include<fstream>
#include<assert.h>
using namespace gfw;

ReplayController::ReplayController() : push_z(false), push_x(false), push_c(false)
, push_a(false), push_s(false)
, push_up(false), push_right(false), push_down(false), push_left(false), push_space(false), cursor(0){}

std::vector<ButtonData> ReplayController::test_data()
{
	std::vector<ButtonData> rtn(300);
	for (int i = 0; i < rtn.size(); i++)
	{
		if (i % 2 == 0)
		{
			//bool push_z, push_x, push_c, push_a, push_s, push_up, push_right, push_down, push_left;
			ButtonData ar = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			rtn[i] = ar;
		}
		else
		{
			ButtonData ar = { 0, 0, 0, 0, 0, 1, 0, 0, 0 ,0};
			rtn[i] = ar;
		}
	}
	//{
	//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	//};
	return rtn;
}

bool ReplayController::poll()
{
	if (cursor >= data.size())
		return false;
	z.setValue(data[cursor][0]);
	x.setValue(data[cursor][1]);
	c.setValue(data[cursor][2]);
	a.setValue(data[cursor][3]);
	s.setValue(data[cursor][4]);

	up.setValue(data[cursor][5]);
	right.setValue(data[cursor][6]);
	down.setValue(data[cursor][7]);
	left.setValue(data[cursor][8]);
	space.setValue(data[cursor][9]);

	cursor++;

	return true;
}

bool ReplayController::readFile(const std::string& file_name)
{
	std::ifstream ifs(file_name);
	if(!ifs)
	{
		std::cout << "ReplayController::readFile could not read file: " << file_name << std::endl;
		return false;
	}
	std::string line;
	while (std::getline(ifs, line))
	{
		if (line[0] == '#')
			continue;
		ButtonData buttons;
		assert(line.size() >= 10);
		for (int i = 0; i < 10; i++)
		{
			buttons[i] = line[i] - '0';
		}
		data.push_back(buttons);
	}
	return true;
}
