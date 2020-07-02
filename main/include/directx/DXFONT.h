#pragma once;

#include<d3d9.h>
#include<d3dx9.h>

class DXFONT
{
private:
	LPD3DXFONT pfont;
	LPD3DXSPRITE psprite;

public:
	DXFONT(LPDIRECT3DDEVICE9 pd3dDevice,int height, int width, UINT weight, bool italic, LPCTSTR fontname);
	~DXFONT();
	void draw(const char* text, int x, int y, int width, int height, float r, float g, float b, float a);
	void draw(const char* text, int x, int y, int width, int height);
};