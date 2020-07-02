#include"framework/VirtualController.h"
#include<fstream>
#include<assert.h>
using namespace gfw;

void ControllerRecorder::push(const ButtonData& buttons)
{
	// size limit
	if (data.size() < 60*60*60*2)
		data.push_back(buttons);
}

bool ControllerRecorder::writeFile(const std::string& file_name)const
{
	std::ofstream ofs(file_name);
	if (!ofs)
	{
		return false;
	}
	ofs << "#ControllerRecorder 2016/03/02" << std::endl;
	ofs << "#button alignment: z/x/c/a/s/up/right/down/left/space" << std::endl;
	for (auto s : comments)
	{
		ofs << "#" << s << std::endl;
	}

	for (auto d : data)
	{
		char buf[11] = {};
		for (int i = 0; i < d.size(); i++)
		{
			buf[i] =  '0' + (int)d[i];
		}
		ofs << buf << std::endl;
	}
	return true;
}
bool ControllerRecorder::readFile(const std::string& file_name)
{
	data.clear();
	std::ifstream ifs(file_name);
	if (!ifs)
	{
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