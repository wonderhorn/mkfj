#include"android/AndroidMedia.h"
#include<assert.h>
using namespace gfw;

Graphics::Graphics() {}
Graphics::~Graphics() {}

bool Graphics::initialize(GLGraphic* glg)
{
	this->glg = glg;
	this->window_width = 640;
	this->window_height = 480;
	if (glg == nullptr)
		return false;
	return true;
}

void Graphics::finalize()
{}

ImageHandler Graphics::loadImage(const std::string& file_name, int w, int h, int color_key)
{
	GLuint tex = glg->loadImage(file_name, color_key);
	if (tex < 0)
		return -1;
	ImageData imd = {tex, w, h, color_key};
	images.push_back(imd);
	return images.size() - 1;
}

ImageHandler Graphics::createRenderingTarget(int w, int h)
{
	return false;
}


void Graphics::releaseImage(ImageHandler ih)
{
	assert(images.size() > ih);
	if (images.size() <= ih || images[ih].texh >= 0)
		return;
	//images[ih]->Release();
	images[ih].texh = -1;
}

bool Graphics::draw(ImageHandler ih, int x, int y, int z, int refx, int refy, int w, int h, bool turn, int blend)
{
	if (images.size() <= ih || images[ih].texh < 0)
		return false;

	glg->draw2DRectangleRotate(images[ih].texh, 0,
		(float)x / window_width * 2 - 1,
		1.0 - (float)y / window_height * 2,
		(float)0,
		(float)w / window_width * 2,
		(float)h / window_height * 2,
		((float)refx) / images[ih].w,
		((float)refy) / images[ih].h,
		((float)(w)) / images[ih].w,
		((float)(h)) / images[ih].h,
		1.0 - 2 * turn, 1
	);
	//*/
	/*
	glg->draw2DRectangle(images[ih].texh,
		(float)x / window_width * 2 - 1,
		1.0 - (float)y / window_height * 2,
		(float)0,
		(float)w / window_width * 2,
		(float)h / window_height * 2,
		(float)refx,
		(float)refy,
		(float)w,
		(float)h
		//1, 1
	);
	*/
	//d3d->put(images[ih], x, y, z, refx, refy, w, h, turn, blend);

	return true;
}
bool Graphics::drawSpinResize(ImageHandler ih, int x, int y, int z, int refx, int refy, int w, int h, bool turn,
	int blend, float magx, float magy, float theta)
{
	if (images.size() <= ih || images[ih].texh < 0)
		return false;

	glg->draw2DRectangleRotate(images[ih].texh, theta,
		(float)x / window_width * 2 - 1.0,
		//(float)y / window_height * 2 - 1.0,
		1.0 - (float)y / window_height * 2,
		(float)z,
		(float)w / window_width * 2,
		(float)h / window_height * 2,
		(float)refx / images[ih].w,
		(float)refy / images[ih].h,
		(float)w / images[ih].w,
		(float)h / images[ih].h,
		magx, magy
	);
	//d3d->put(images[ih], x, y, z, refx, refy, w, h, turn, blend);

	return true;
}
bool Graphics::drawSpinResizeNoAlpha(ImageHandler ih, int x, int y, int z, int refx, int refy, int w, int h, bool turn, int blend, float magx, float magy, float theta)
{
	if (images.size() <= ih || images[ih].texh < 0)
		return false;

	glg->draw2DRectangle(images[ih].texh,
		(float)x / window_width * 2 - 1.0,
		//(float)y / window_height * 2 - 1.0,
		1.0 - (float)y / window_height * 2,
		(float)z,
		(float)w * magx / window_width * 2,
		(float)h * magy / window_height * 2,
		(float)refx / images[ih].w,
		(float)refy / images[ih].h,
		(float)w / images[ih].w,
		(float)h / images[ih].h
	);
	//d3d->put(images[ih], x, y, z, refx, refy, w, h, turn, blend);

	return true;

}
bool Graphics::drawLine(int orix, int oriy, int terx, int tery, int r, int g, int b, int a, int width)
{
	return false;
}
bool Graphics::drawString(const std::string& text, int x, int y, int width, int height
	, float r, float g, float b, float a)
{
	return false;
}
bool Graphics::wipe(int x, int y, int r)
{
	return false;
}
bool Graphics::clear(int r, int g, int b, int a)
{
	//d3d->Clear(r, g, b, a);
	return true;
}

bool Graphics::startRendering()
{
	return true;
}
void Graphics::endRendering()
{
}

bool Graphics::setRenderingTarget(ImageHandler img)
{
	return false;
}

bool Graphics::setRenderingTargetToBackBuffer()
{
	return false;
}

void Graphics::resizeWindow(int w, int h)
{
}

void Graphics::releaseAllImages()
{
	for (int i = 0; i < images.size(); i++)
	{
		if (images[i].texh >= 0)
		{
			glDeleteTextures(1, &images[i].texh);
			images[i].texh = -1;
		}
	}
	images.clear();
}

void Graphics::screenshot(const std::string& file_name)
{
}