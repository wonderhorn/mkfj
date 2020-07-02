#include"jumpnrun/system/JumpnRunScreen.h"
#include<fstream>

using namespace jnr;
using namespace gfw;

bool JumpnRunScreen::dumpTaskListToFile(const std::string& file_name)
{
	std::ofstream ofs(file_name, std::ios::binary);
	$P.mutex_tasklist.lock();
	ofs.write(this->tl.Buffer(), tl.BUF_SIZE);
	$P.mutex_tasklist.unlock();
	return true;
}
bool JumpnRunScreen::readTaskListFromFile(const std::string& file_name)
{
	std::ifstream ifs(file_name, std::ios::binary);
	$P.mutex_tasklist.lock();
	ifs.read(this->tl.Buffer(), tl.BUF_SIZE);
	$P.mutex_tasklist.unlock();
	return true;
}