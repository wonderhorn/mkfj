#include"framework/Media.h"
#include"utils/MyUtil.h"
#include"jumpnrun/system/Parmanent.h"
#include<assert.h>
using namespace gfw;

Graphics::Graphics() : d3d(nullptr), pBackBuf(nullptr){}
Graphics::~Graphics(){}

bool Graphics::initialize(HWND hWnd)
{
	this->hwnd = hWnd;
	d3d = new CD3DSetup();
	bool result = d3d->InitD3D(hWnd, windowed, window_width, window_height) == S_OK;
	pBackBuf = nullptr;
	return result;
}

void Graphics::finalize()
{
	d3d->Cleanup();
	SAFE_DELETE(d3d);
}

ImageHandler Graphics::loadImage(const std::string& file_name, int w, int h, int color_key)
{
	LPDIRECT3DTEXTURE9 tex = d3d->LoadTexture(tex, file_name.c_str(), w, h, color_key);
	if (tex == NULL)
	{
		outputLog("画像ファイルがありません: " + file_name);
		return -1;
	}
	images.push_back(tex);
	return images.size() - 1;
}

ImageHandler Graphics::createRenderingTarget(int w, int h)
{
	LPDIRECT3DTEXTURE9 tex = d3d->CreateRenderingTarget(tex, w, h);
	if (tex == NULL)
	{
		outputLog("レンダリングターゲットを作成できませんでした。");
		return -1;
	}
	images.push_back(tex);
	return images.size() - 1;
}


void Graphics::releaseImage(ImageHandler ih)
{
	assert(images.size() > ih);
	if (images.size() <= ih || !images[ih])
		return;
	images[ih]->Release();
	images[ih] = nullptr;
}

bool Graphics::draw(ImageHandler ih, int x, int y, int z, int refx, int refy, int w, int h, bool turn, int blend)
{
	if (images.size() <= ih || !images[ih])
		return false;

	d3d->put(images[ih], x, y, z, refx, refy, w, h, turn, blend);

	return true;
}
bool Graphics::drawSpinResize(ImageHandler ih, int x, int y, int z, int refx, int refy, int w, int h, bool turn, int blend, float magx, float magy, float theta)
{
	if (images.size() <= ih || !images[ih])
		return false;
	d3d->putScalingSpinning(images[ih], x, y, z, refx, refy, w, h, turn, blend, magx, magy, theta);
	return false;
}
bool Graphics::drawSpinResizeNoAlpha(ImageHandler ih, int x, int y, int z, int refx, int refy, int w, int h, bool turn, int blend, float magx, float magy, float theta)
{
	if (images.size() <= ih || !images[ih])
		return false;
	d3d->putScalingSpinningNoAlpha(images[ih], x, y, z, refx, refy, w, h, turn, blend, magx, magy, theta);
	return false;
}
bool Graphics::drawLine(int orix, int oriy, int terx, int tery, int r, int g, int b, int a, int width)
{
	HRESULT res = d3d->drawLine(orix, oriy, terx, tery, r, g, b, a, width);
	return res == S_OK;
}
bool Graphics::drawString(const std::string& text, int x, int y, int width, int height
	, float r, float g, float b, float a)
{
	d3d->font->draw(text.c_str(), x, y, width, height, r, g, b, a);
	return true;
}
bool Graphics::Whipe(int x, int y, int r)
{
	d3d->Whipe(x, y, r);
	return true;
}
bool Graphics::clear(int r, int g, int b, int a)
{
	d3d->Clear(r, g, b, a);
	return true;
}

bool Graphics::startRendering()
{
	if (d3d->StartRendering() == S_OK)
	{
		d3d->Clear(0, 0, 255, 255);
		return true;
	}
	else
	{
		HRESULT hr = d3d->reset();
		if (FAILED(hr))
		{
			d3d->Cleanup();
			SAFE_DELETE(d3d);
			d3d = new CD3DSetup();
			//bool result = d3d->InitD3D(hwnd, windowed, window_width, window_height) == S_OK;
			bool result = d3d->InitD3D(hwnd, windowed, $P.current_window_w, $P.current_window_h) == S_OK;
			$P.loadAllImages();
			return result;
		}
		return true;
	}
}
void Graphics::endRendering()
{
	d3d->EndRendering();
}

bool Graphics::setRenderingTarget(ImageHandler img)
{
	if (images.size() <= img || !images[img])
		return false;
	LPDIRECT3DSURFACE9 pTarget = NULL, pTargetZbuffer = NULL;
	LPDIRECT3DTEXTURE9 pimg = images[img];
	pimg->GetSurfaceLevel(0, &pTarget);
	d3d->SetRenderTarget(pTarget);
	pTarget->Release();
	return true;
}

bool Graphics::setRenderingTargetToBackBuffer()
{
	//pimg->GetSurfaceLevel(0, &pTarget);
	auto bb = d3d->getBackBuffer();
	d3d->SetRenderTarget(bb);
	bb->Release();
	return true;
}

void Graphics::resizeWindow(int w, int h)
{
	releaseAllImages();
	d3d->resize(w, h);
	$P.loadAllImages();
}

void Graphics::releaseAllImages()
{
	for (int i = 0; i < images.size(); i++)
	{
		if (images[i])
		{
			images[i]->Release();
			images[i] = nullptr;
		}
	}
	images.clear();
}

void Graphics::screenshot(const std::string& file_name)
{
	d3d->ScreenShot(file_name.c_str());
}