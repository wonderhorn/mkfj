#include"directx/DXFONT.h"

DXFONT::DXFONT(LPDIRECT3DDEVICE9 pd3dDevice,int height, 
	int width, UINT weight, bool italic, LPCTSTR fontname)
{
	if(FAILED(D3DXCreateSprite(pd3dDevice, &psprite))) psprite = NULL;

	if(FAILED(D3DXCreateFont(pd3dDevice, -height,  -width, weight, 1, italic, 
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, 
		3 | FF_DONTCARE, fontname, &pfont)))
	{
		pfont = NULL;
	}

}

DXFONT::~DXFONT()
{
	if(pfont) pfont->Release();
	if(psprite) psprite->Release();
}

void DXFONT::draw(const char* text, int x, int y, 
	int width, int height, float r, float g, float b, float a)
{
	RECT rc = {x, y, x + width, y + height};
	RECT rc2 = {x + 1, y + 1, x + 1 + width, y + 1 + height};
	psprite->Begin(D3DXSPRITE_ALPHABLEND|D3DXSPRITE_SORT_TEXTURE);
	pfont->DrawTextA(psprite, text, -1, &rc, DT_LEFT|DT_VCENTER, D3DCOLOR_COLORVALUE(r, g, b, a));
	psprite->End();
}

void DXFONT::draw(const char* text, int x, int y, int width, int height)
{
	draw(text, x, y, width, height, 0, 0, 0, 1);
}