#include"framework/Media.h"
using namespace gfw;

Media Media::instance;
Media::Media(){}
bool Media::initialize(HWND hWnd, WNDCLASSEX wc, int window_width, int window_height)
{
	bool result = true;
	graphics.window_width = window_width;
	graphics.window_height = window_height;
	graphics.windowed = true;
	graphics.initialize(hWnd);
	sounds.initialize(hWnd);
	musics.initialize(hWnd, wc);
	input.initialize(hWnd, wc);

	//sounds.initialize();
	//musics.initialize();
	return result;
}
void Media::finalize()
{
	graphics.finalize();
}