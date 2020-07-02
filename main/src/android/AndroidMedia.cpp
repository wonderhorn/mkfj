#include"android/AndroidMedia.h"
using namespace gfw;

Media::Media()
{}

bool Media::initialize(GLGraphic* glg)
{
	graphics.initialize(glg);
}
void Media::finalize() {}
