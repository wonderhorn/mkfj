#include"framework/MediaToFile.h"
#include<fstream>
#include<iostream>
#include<assert.h>
using namespace gfw;

GraphicsToFile::GraphicsToFile() {}

GraphicsToFile::~GraphicsToFile() {}

bool GraphicsToFile::initialize(const std::string& filename)
{
	this->filename = filename;
	return true;
}
void GraphicsToFile::finalize()
{
	std::ofstream ofs(filename);
	for (int i = 0; i < frames.size(); i++)
	{
		ofs << i << std::endl;
		for (int j = 0; j < frames[i].size(); j++)
		{
			auto& obj = frames[i][j];
			ofs << images[obj.image] << ","
				<< obj.x << ","
				<< obj.y << ","
				<< obj.refx << ","
				<< obj.refy << ","
				<< obj.w << ","
				<< obj.h << ","
				<< obj.turned << ","
				<< obj.blend << ","
				<< obj.magx << ","
				<< obj.magy << ","
				<< obj.theta << std::endl;
		}
	}
}

void printObject(const GraphicsToFileRenderedObject& obj)
{
    std::cout << obj.image << ","
        << obj.x << ","
        << obj.y << ","
        << obj.refx << ","
        << obj.refy << ","
        << obj.w << ","
        << obj.h << ","
        << obj.turned << ","
        << obj.blend << ","
        << obj.magx << ","
        << obj.magy << ","
        << obj.theta << std::endl;
}

ImageHandler GraphicsToFile::loadImage(const std::string& file_name, int w, int h, int color_key)
{
  std::cout << "#loading " << file_name << std::endl; 
	images.push_back(file_name);
  ImageHandler idx = images.size() - 1;
  //std::cout << "idx = " << idx << std::endl;
	return idx;
}
ImageHandler GraphicsToFile::createRenderingTarget(int w, int h)
{
	images.push_back("rendering_target");
	return images.size() - 1;
}
void GraphicsToFile::releaseImage(ImageHandler ih)
{
	assert(images.size() > ih);
	if (images.size() <= ih || images[ih].size() == 0)
		return;
	images[ih] = "";
}

bool GraphicsToFile::draw(ImageHandler ih, int x, int y, int z, int refx, int refy, int w, int h,
	bool turn, int blend)
{
  //std::cout << "draw = " << ih << std::endl;
	GraphicsToFileRenderedObject obj;
	obj.image = ih;
	obj.x = x;
	obj.y = y;
	obj.w = w;
	obj.h = h;
	obj.refx = refx;
	obj.refy = refy;
	obj.turned = turn;
	obj.blend = blend;
	obj.theta = 0;
	obj.magx = obj.magy = 1;
	frames[current_frame_idx].push_back(obj);
	return true;
}
bool GraphicsToFile::drawSpinResize(ImageHandler ih, int x, int y, int z,
	int refx, int refy, int w, int h, bool turn, int blend, float magx, float magy, float theta)
{
	GraphicsToFileRenderedObject obj;
	obj.image = ih;
	obj.x = x;
	obj.y = y;
	obj.w = w;
	obj.h = h;
	obj.refx = refx;
	obj.refy = refy;
	obj.turned = turn;
	obj.blend = blend;
	obj.theta = theta;
	obj.magx = magx;
	obj.magy = magy;
	frames[current_frame_idx].push_back(obj);
	return true;
}
bool GraphicsToFile::drawSpinResizeNoAlpha(ImageHandler ih, int x, int y, int z, int refx, int refy,
	int w, int h, bool turn, int blend, float magx, float magy, float theta)
{
  //std::cout << "drawSpinResizeNoAlpha ih = " << ih << std::endl;
	GraphicsToFileRenderedObject obj;
	obj.image = ih;
	obj.x = x;
	obj.y = y;
	obj.w = w;
	obj.h = h;
	obj.refx = refx;
	obj.refy = refy;
	obj.turned = turn;
	obj.blend = blend;
	obj.theta = 0;
	obj.magx = 1;
	obj.magy = 1;
	frames[current_frame_idx].push_back(obj);
	return true;
}
bool GraphicsToFile::drawLine(int orix, int oriy, int terx, int tery, int r, int g, int b, int a, int width)
{
	GraphicsToFileRenderedObject obj;
	obj.image = -1;
	obj.x = orix;
	obj.y = oriy;
	obj.w = terx;
	obj.h = tery;
	obj.refx = width;
	obj.refy = 0;
	obj.turned = false;
	obj.blend = 0xffffff;
	obj.theta = 0;
	obj.magx = 1;
	obj.magy = 1;
	frames[current_frame_idx].push_back(obj);
	return true;
}
bool GraphicsToFile::drawString(const std::string& text, int x, int y, int width, int height
	, float r , float g, float b, float a)
{
	return false;
}
bool GraphicsToFile::clear(int r, int g, int b, int a)
{
	return false;
}
bool GraphicsToFile::startRendering()
{
	while (frames.size() <= current_frame_idx)
	{
		frames.push_back(std::vector<GraphicsToFileRenderedObject>());
	}
	return true;
}
void GraphicsToFile::endRendering()
{
    int i = current_frame_idx;
    std::cout << "frame=" << i << std::endl;
    for (int j = 0; j < frames[i].size(); j++)
    {
      auto& obj = frames[i][j];
      //std::cout << "ih = " << obj.image << std::endl;
      //istd::cout << "images size = " << images.size() << std::endl;
      std::cout << images[obj.image] << ","
        << obj.x << ","
        << obj.y << ","
        << obj.refx << ","
        << obj.refy << ","
        << obj.w << ","
        << obj.h << ","
        << obj.turned << ","
        << obj.blend << ","
        << obj.magx << ","
        << obj.magy << ","
        << obj.theta << std::endl;
    }
	  current_frame_idx++;
}

bool GraphicsToFile::setRenderingTarget(ImageHandler img)
{
	return true;
}
bool GraphicsToFile::setRenderingTargetToBackBuffer()
{
	return true;
}
void GraphicsToFile::resizeWindow(int w, int h) {}
void GraphicsToFile::releaseAllImages() {}


bool MediaToFile::initialize(int window_width, int window_height, const std::string& file_name)
{
	graphics.initialize(file_name);
	return true;
}
void MediaToFile::finalize()
{
	graphics.finalize();
}
