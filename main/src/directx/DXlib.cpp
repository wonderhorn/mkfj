#include"directx/DXlib.h"
#include"directx/DXFONT.h"
#include"utils/MyUtil.h"
//#include "headers.h"
#include<iostream>
#define FVF_CUSTOM ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
using namespace std;

const int d3dtop = D3DTOP_SELECTARG1;
const int d3daplphatest = 0;
extern FILE* dousalog;
//�E�B���h�E�v���V�[�W��
extern LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

//Direct3D9�t���[���N���X

//�R���X�g���N�^
CD3DSetup::CD3DSetup()
{
	lpD3D = NULL;
	lpD3DDEV = NULL;
	psprite = NULL;
	font = NULL;
	fontw = NULL;

	iloop = 1;
}

//�f�X�g���N�^
CD3DSetup::~CD3DSetup()
{
	Cleanup();		//������������[�`��
}

//������������[�`��
HRESULT CD3DSetup::Cleanup()
{
	SAFE_DELETE(font);
	SAFE_DELETE(fontw);
	//RemoveFontResource("FOT-RodinNTLGPro-EB.otf\0");
	//SendMessage(HWND_BROADCAST, WM_FONTCHANGE , NULL, NULL);
	SAFE_RELEASE(pstencil);
	SAFE_RELEASE(pforeground);
	SAFE_RELEASE(pline);
	SAFE_RELEASE(psprite);
	SAFE_RELEASE(lpD3DDEV);
	SAFE_RELEASE(lpD3D);

 	return TRUE;
}

//�N��
HRESULT CD3DSetup::Run(HINSTANCE hInst, char szWinName[], int width, int height, BOOL bWin)
{
	m_iwidth = width;
	m_iheight = height;

	HRESULT	hr;
	HWND hWnd;

	// �E�B���h�E�N���X�ݒ�
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, NULL, 0L, 0L,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                       szWinName, NULL };
    RegisterClassEx( &wc );

	// �E�B���h�E����
 	if(bWin)
 	{
 		//Window���[�h
 	    hWnd = CreateWindow( szWinName, szWinName,
				WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
 				GetDesktopWindow(), NULL, wc.hInstance, NULL );
 	}
 	else
	{
	//�t���X�N���[��
		hWnd = CreateWindow( szWinName, szWinName,
			WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
 		    HWND_DESKTOP, NULL, wc.hInstance, NULL );
	}

	INT iMsg;
	if(SUCCEEDED(hr = InitD3D(hWnd, bWin, m_iwidth, m_iheight )))
	{
		// �E�B���h�E�`��
        ShowWindow( hWnd, SW_SHOWDEFAULT );
        UpdateWindow( hWnd );

		iMsg = AppLoop();	//�A�v���P�[�V�������[�v
 	}

	UnregisterClass(szWinName, wc.hInstance );

	return iMsg;
}

//Direct3D9������
HRESULT CD3DSetup::InitD3D(HWND hWnd, BOOL bWin, int width, int height)
{
	HRESULT	hr;

	outputLog("Direct3D�����������܂��B\0");

	if( NULL == ( lpD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		outputLog("Direct3D9�I�u�W�F�N�g�̐����Ɏ��s���܂����B\0");
		MessageBox(0, "Direct3D9�I�u�W�F�N�g�̐����Ɏ��s���܂����B\0", NULL, MB_OK);
		return E_FAIL;
	}
	outputLog("Direct3D9�I�u�W�F�N�g�̐����ɐ������܂����B\0");

	D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = bWin;		//TRUE=Window,FALSE=FULLSCREEN
	if (1)
	//if (!bWin)
 	{ 		//�t���X�N���[�����̓o�b�N�o�b�t�@�̕��A�������Ďw��
 		d3dpp.BackBufferWidth = width;		//��
		d3dpp.BackBufferHeight = height;		//����
		//d3dpp.BackBufferFormat = D3DFMT_R5G6B5;	//16bit
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	//���݂̉�ʃ��[�h�𗘗p
	}
 	else
 	{
 		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	//���݂̉�ʃ��[�h�𗘗p
 	}
     d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
 	// Z �o�b�t�@�̎����쐬
 	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	//d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;

	param = d3dpp;

 	//HAL(pure vp)
 	if(FAILED(hr=lpD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,
 		D3DCREATE_HARDWARE_VERTEXPROCESSING|D3DCREATE_MULTITHREADED,&d3dpp,&lpD3DDEV)))
 	{
		outputLog("Direct3D�f�o�C�X�̃n�[�h�E�F�A���_�������[�h�ł̎擾�Ɏ��s���܂����B\0");
 		//HAL(soft vp)
		if(FAILED(hr=lpD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,
 			D3DCREATE_SOFTWARE_VERTEXPROCESSING|D3DCREATE_MULTITHREADED,&d3dpp,&lpD3DDEV)))
 		{
			outputLog("Direct3D�f�o�C�X�̃\�t�g�E�F�A���_�������[�h�ł̎擾�Ɏ��s���܂����B\0");
 			//REF
 			if(FAILED(hr=lpD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,hWnd,
 				D3DCREATE_HARDWARE_VERTEXPROCESSING|D3DCREATE_MULTITHREADED,&d3dpp,&lpD3DDEV)))
 			{
				outputLog("Direct3D�f�o�C�X�̃��t�@�����X���X�^���C�U�A�n�[�h�E�F�A���_�������[�h�ł̎擾�Ɏ��s���܂����B\0");
				outputLog("Direct3D�f�o�C�X�̎擾�Ɏ��s���܂����B\0");
				MessageBox(0, "Direct3D�f�o�C�X�̎擾�Ɏ��s���܂����B\0", NULL, MB_OK);

				return E_FAIL;
 			}
			else outputLog("Direct3D�f�o�C�X�̃\�t�g�E�F�A�f�o�C�X�A�n�[�h�E�F�A���_�������[�h�ł̎擾�ɐ������܂����B\0");
		}
		else outputLog("Direct3D�f�o�C�X�̃\�t�g�E�F�A���_�������[�h�ł̎擾�ɐ������܂����B\0");
 	}
	else outputLog("Direct3D9�f�o�C�X�̃n�[�h�E�F�A���_�������[�h�ł̎擾�ɐ������܂����B\0");

	//if(FAILED(lpD3D->CheckDeviceFormat())
	D3DCAPS9 Caps;
	lpD3DDEV->GetDeviceCaps(&Caps);
	if( FAILED( lpD3D->CheckDeviceFormat( Caps.AdapterOrdinal,
		Caps.DeviceType,
		D3DFMT_R5G6B5,
		D3DUSAGE_DEPTHSTENCIL,
		D3DRTYPE_SURFACE,
		D3DFMT_D24S8 ) ) )
	{
		outputLog("�X�e���V�����T�|�[�g����Ă��܂���B\0");
		return E_FAIL;
	}


	//�X�v���C�g�̍쐬
	if(FAILED(D3DXCreateSprite(lpD3DDEV, &psprite)))
	{
		outputLog("�X�v���C�g�̍쐬�Ɏ��s���܂����B\0");
		return E_FAIL;
	}

	lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	lpD3DDEV->SetRenderState(D3DRS_ALPHATESTENABLE, d3daplphatest);
	lpD3DDEV->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
	lpD3DDEV->SetRenderState( D3DRS_ZENABLE, FALSE);
	hr = lpD3DDEV->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	hr = lpD3DDEV->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	hr = lpD3DDEV->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	//���C���̍쐬
	if(FAILED(D3DXCreateLine(lpD3DDEV, &pline)))
	{
		outputLog("�X�v���C�g�̍쐬�Ɏ��s���܂����B\0");
		return E_FAIL;
	}
	lpD3DDEV->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pforeground, NULL);

	//bool b =  AddFontResource("�V�f��Aug15\\FOT-RodinNTLGPro-EB.otf\0");
	//SendMessage(HWND_BROADCAST, WM_FONTCHANGE , NULL, NULL);//"FOT-���_���J�g���A Pro"

	font = new DXFONT(lpD3DDEV, 16, 0, 0, 0, "MS P �S�V�b�N\0");
	//font = new DXFONT(lpD3DDEV, 16, 0, 0, 0, "���C���I\0");
	fontw = new DXFONT(lpD3DDEV, 20, 0, -3, 0, "MS �S�V�b�N\0");
	//font = new DXFONT(lpD3DDEV, 16, 0, 0, 0, "Meiryo UI\0");
	//fontw = new DXFONT(lpD3DDEV, 22, 0, 0, 0, "Meiryo UI\0");

	//if(FAILED(LoadTexture(ptitle, _T("title.png\0")))) return E_FAIL;
	//ptitle = LoadTexture(ptitle, _T("title.png\0"), 512, 512);
	//LoadTexture(pstencil, "images\\stencil.bmp", 640, 480, 0);

	outputLog("Direct3D�̏������𐳏�ɏI�����܂����B\0");
 	return hr;
 }

//�e�N�X�`���̐���
LPDIRECT3DTEXTURE9 CD3DSetup::LoadTexture(LPDIRECT3DTEXTURE9 &ptexture, LPCTSTR filename, int x, int y, int toukasyoku)
{
	if(FAILED(D3DXCreateTextureFromFileEx(lpD3DDEV, filename, x, y, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_POINT, toukasyoku, NULL, NULL, &ptexture)))
	{
		//MessageBox(0, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B\0", NULL, MB_OK);
		return NULL;
	}
	return ptexture;
}
LPDIRECT3DTEXTURE9 CD3DSetup::LoadTexture(LPDIRECT3DTEXTURE9 &ptexture, LPCTSTR filename, int x, int y)
{
	return LoadTexture(ptexture, filename, x, y, 0xffffffff);
}
LPDIRECT3DTEXTURE9 CD3DSetup::LoadTextureFromResource(LPDIRECT3DTEXTURE9 &ptexture, LPCTSTR filename, int x, int y, int toukasyoku)
{
	if(FAILED(D3DXCreateTextureFromResourceEx(lpD3DDEV, NULL, filename, x, y, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_POINT, toukasyoku, NULL, NULL, &ptexture)))
	{
		//MessageBox(0, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B\0", NULL, MB_OK);
		return NULL;
	}
	return ptexture;
}
LPDIRECT3DTEXTURE9 CD3DSetup::LoadTextureFromMem(LPDIRECT3DTEXTURE9 &ptexture, void* data,int size, int x, int y, int toukasyoku)
{
	if(FAILED(D3DXCreateTextureFromFileInMemoryEx(lpD3DDEV, data, size, x, y, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_POINT, toukasyoku, NULL, NULL, &ptexture)))
	{

		//MessageBox(0, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B\0", NULL, MB_OK);
		//HRESULT hr = D3DXCreateTextureFromFileInMemoryEx(lpD3DDEV, data, size, x, y, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, toukasyoku, NULL, NULL, &ptexture);
		return NULL;
	}
	return ptexture;
}

LPDIRECT3DTEXTURE9 CD3DSetup::CreateRenderingTarget(LPDIRECT3DTEXTURE9 &ptexture, int x, int y)
{
	if (!lpD3DDEV || FAILED(lpD3DDEV->CreateTexture(x, y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &ptexture, NULL)))
	{

		//MessageBox(0, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B\0", NULL, MB_OK);
		//HRESULT hr = lpD3DDEV->CreateTexture(x, y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &ptexture, NULL);
		return NULL;
	}
	return ptexture;
}

//�����`�̕`��
void CD3DSetup::Set2DRectangle(float x, float y, float width, float height, LPDIRECT3DTEXTURE9 ptex)
{
	IDirect3DVertexBuffer9* pVertex;
	lpD3DDEV->CreateVertexBuffer( sizeof(CUSTOMVERTEX)*4, D3DUSAGE_WRITEONLY, FVF_CUSTOM, D3DPOOL_MANAGED, &pVertex, NULL);

	void *pData;
	pVertex->Lock( 0, 0, ( void** )&pData, 0 );

	CUSTOMVERTEX Vertices[4] =
	{
		{x + width, y, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f},
		{x + width, y + height,  0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f},
		{x, y, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f},
		{x, y + height, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f}
	};
	memcpy(pData, Vertices, sizeof(CUSTOMVERTEX)*4);

	pVertex->Unlock();

	lpD3DDEV->SetTexture(0, ptex);

	lpD3DDEV->SetStreamSource(0, pVertex, 0, sizeof(CUSTOMVERTEX));
	lpD3DDEV->SetFVF(FVF_CUSTOM);
	lpD3DDEV->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

void CD3DSetup::Set2DCircle(float x, float y, float z, float radius, int color)
{
	static const int num_v = 20;
	IDirect3DVertexBuffer9* pVertex;
	lpD3DDEV->CreateVertexBuffer( sizeof(CUSTOMVERTEX)*(num_v + 1), D3DUSAGE_WRITEONLY, FVF_CUSTOM, D3DPOOL_MANAGED, &pVertex, NULL);

	void *pData;
	pVertex->Lock( 0, 0, ( void** )&pData, 0 );

	CUSTOMVERTEX Vertices[num_v + 1] = {};
	for(int i = 0; i < num_v; i++)
	{
		Vertices[i].dwColor = color;
		Vertices[i].rhw = 1.0;
		Vertices[i].x = x+ radius * cos(2.0 * PI * i / num_v);
		Vertices[i].y = y + radius * sin(2.0 * PI * i / num_v);
		Vertices[i].z = z;
		Vertices[i].u =  cos(2.0 * PI * i / num_v) /2 + 0.5;
		Vertices[i].v =  sin(2.0 * PI * i / num_v) /2 + 0.5;
	}
	Vertices[num_v] = Vertices[0];

	memcpy(pData, Vertices, sizeof(CUSTOMVERTEX)*(num_v + 1));

	pVertex->Unlock();

	lpD3DDEV->SetTexture(0, NULL);

	lpD3DDEV->SetStreamSource(0, pVertex, 0, sizeof(CUSTOMVERTEX));
	lpD3DDEV->SetFVF(FVF_CUSTOM);
	lpD3DDEV->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, num_v);
}


HRESULT CD3DSetup::put(LPDIRECT3DTEXTURE9 ptex, float x, float y, float z, int toukasyoku)
{
	if(ptex == NULL)
	{
		//MessageBox(0, _T("�e�N�X�`��������܂���\0"), NULL, MB_OK);
		return E_FAIL;
	}

	D3DXVECTOR3 vCent(-x, -y, -0.0);
	D3DXVECTOR3 vZ(0, 0, 1.0);
	D3DXMATRIXA16 matTranslation;
	//D3DXMatrixIdentity(&matTranslation);
	D3DXMatrixTranslation(&matTranslation, 0.0, 0.0, z);


	HRESULT hr;
	//��������Ɠ��߂��Ăѐ���
	hr = lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	hr = lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	hr = lpD3DDEV->SetRenderState(D3DRS_ALPHATESTENABLE, d3daplphatest);
	hr = lpD3DDEV->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
	hr = lpD3DDEV->SetRenderState( D3DRS_ALPHAREF, (DWORD)toukasyoku );

	if(FAILED(hr))
	{
		//MessageBox(0, _T("�A���t�@�u�����f�B���O�Ɏ��s���܂���\0"), 0, MB_OK);
		return hr;
	}

	psprite->Begin(D3DXSPRITE_ALPHABLEND);
	psprite->SetTransform(&matTranslation);
	psprite->Draw(ptex, NULL, &vCent, &vZ, toukasyoku);
	psprite->End();

	hr = lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	hr = lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	hr = lpD3DDEV->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}
HRESULT CD3DSetup::put(LPDIRECT3DTEXTURE9 ptex, int x, int y, float z, int toukasyoku)
{
	return put( ptex, (float) x, (float) y, (float) z, toukasyoku);
}
HRESULT CD3DSetup::put(LPDIRECT3DTEXTURE9 ptex, float x, float y, float z)
{
	return put(ptex, x, y, z, 0xffffffff);
}
HRESULT CD3DSetup::put(LPDIRECT3DTEXTURE9 ptex, int x, int y, float z)
{
	return put(ptex, (float) x, (float) y, (float)z);
}

HRESULT CD3DSetup::put(LPDIRECT3DTEXTURE9 ptex, float x, float y, float z,int x_on_graph, int y_on_graph, int width, int height)
{
	if(ptex == NULL)
	{
		//MessageBox(0, _T("�e�N�X�`��������܂���\0"), NULL, MB_OK);
		//return E_FAIL;
		return S_OK;
	}
	//HRESULT hr;

	D3DXVECTOR3 vCent(0, 0, .0);
	D3DXMATRIXA16 matTranslation;
	D3DXMATRIXA16 matRotation;
	//D3DXMatrixIdentity(&matTranslation);
	D3DXMatrixTranslation(&matTranslation, x, y, z);
	D3DXMatrixScaling(&matRotation, 1, 1, 1);
	matTranslation = matRotation*matTranslation;



	RECT rc = {x_on_graph, y_on_graph, x_on_graph+width-1, y_on_graph+height-1};

	psprite->Begin(D3DXSPRITE_ALPHABLEND);
	psprite->SetTransform(&matTranslation);
	psprite->Draw(ptex, &rc, &vCent, NULL, 0xffffffff);
	psprite->End();

	//hr = lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	//hr = lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	//hr = lpD3DDEV->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;

}
HRESULT CD3DSetup::put(LPDIRECT3DTEXTURE9 ptex, int x, int y, float z,int x_on_graph, int y_on_graph, int width, int height)
{
	return put( ptex, (float) x, (float) y, (float)z, x_on_graph,  y_on_graph,  width,  height);
}

HRESULT CD3DSetup::put(LPDIRECT3DTEXTURE9 ptex, float x, float y, float z,int x_on_graph, int y_on_graph, int width, int height, bool muki)
{
	return put(ptex, x, y, z, x_on_graph, y_on_graph, width, height, muki, 0xffffffff);
	//�f�o�C�X���X�g��ɃA�N�Z�X�ᔽ
	/*if(ptex == NULL)
	{
		//MessageBox(0, _T("�e�N�X�`��������܂���\0"), NULL, MB_OK);
		//return E_FAIL;
		return S_OK;
	}
	HRESULT hr;
	hr = lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	hr = lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	hr = lpD3DDEV->SetTextureStageState(0, D3DTSS_COLOROP , D3DTOP_MODULATE );
	hr = lpD3DDEV->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);        //�A���t�@�u�����f�B���O�̗L����
	hr = lpD3DDEV->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);   //�u�����h�t�@�N�^�[�̒l
	hr = lpD3DDEV->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVBLENDFACTOR);   //�u�����h�t�@�N�^�[�̋t�̒l
	//DWORD dwAlpha = alfa;                         //�F
	//hr = lpD3DDEV->SetRenderState(D3DRS_BLENDFACTOR,dwAlpha);

	D3DXVECTOR3 vCent(0, 0, 0);
	D3DXMATRIXA16 matTranslation;
	D3DXMATRIXA16 matRotation;
	//D3DXMatrixIdentity(&matTranslation);
	D3DXMatrixTranslation(&matTranslation, x + width*((int)muki), y, 0.0);
	D3DXMatrixScaling(&matRotation, 1- ((int)muki)*2, 1, 1);
	matTranslation = matRotation*matTranslation;



	RECT rc = {x_on_graph, y_on_graph, x_on_graph+width-1, y_on_graph+height-1};

	psprite->Begin(D3DXSPRITE_ALPHABLEND);
	psprite->SetTransform(&matTranslation);
	psprite->Draw(ptex, &rc, &vCent, NULL, 0xffffffff);
	psprite->End();

	hr = lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	hr = lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	hr = lpD3DDEV->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;*/

}
HRESULT CD3DSetup::put(LPDIRECT3DTEXTURE9 ptex, int x, int y, float z, int x_on_graph, int y_on_graph, int width, int height, bool muki)
{
	return put( ptex, (float) x, (float) y, (float)z, x_on_graph, y_on_graph, width, height, muki);
}

HRESULT CD3DSetup::put(LPDIRECT3DTEXTURE9 ptex, float x, float y, float z,int x_on_graph, int y_on_graph, int width, int height, bool muki, int alfa)
{//�f�o�C�X���X�g��̓��߂Ɏ��s
	if(ptex == NULL)
	{
		//MessageBox(0, _T("�e�N�X�`��������܂���\0"), NULL, MB_OK);
		//return E_FAIL;
		return S_OK;
	}


	D3DXVECTOR3 vCent(0, 0, 0);
	D3DXMATRIXA16 matTranslation;
	D3DXMATRIXA16 matRotation;
	//D3DXMatrixIdentity(&matTranslation);
	D3DXMatrixTranslation(&matTranslation, x + width*((int)muki), y, z);
	D3DXMatrixScaling(&matRotation, 1- ((int)muki)*2, 1, 1);
	matTranslation = matRotation*matTranslation;

	HRESULT hr = S_OK;

	/*hr = lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	hr = lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	hr = lpD3DDEV->SetTextureStageState(0, D3DTSS_COLOROP , D3DTOP_MODULATE );

	hr = lpD3DDEV->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);        //�A���t�@�u�����f�B���O�̗L����
	hr = lpD3DDEV->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);   //�u�����h�t�@�N�^�[�̒l
	hr = lpD3DDEV->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVBLENDFACTOR);   //�u�����h�t�@�N�^�[�̋t�̒l
	DWORD dwAlpha = alfa;                         //�F
	hr = lpD3DDEV->SetRenderState(D3DRS_BLENDFACTOR,dwAlpha);          //�u�����f�B���O�W���̐ݒ�

	hr = lpD3DDEV->SetRenderState( D3DRS_ALPHAREF, (DWORD)alfa);//?
	hr = lpD3DDEV->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);
	hr = lpD3DDEV->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );*/

	if(FAILED(hr))
	{
		//MessageBox(0, _T("�A���t�@�u�����f�B���O�Ɏ��s���܂���\0"), 0, MB_OK);
		return hr;
	}

	RECT rc = {x_on_graph, y_on_graph, x_on_graph+width, y_on_graph+height};
	psprite->Begin(D3DXSPRITE_ALPHABLEND);
	psprite->SetTransform(&matTranslation);
	psprite->Draw(ptex, &rc, &vCent, NULL, alfa);
	psprite->End();

	hr = lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	hr = lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	hr = lpD3DDEV->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;

}
HRESULT CD3DSetup::put(LPDIRECT3DTEXTURE9 ptex, int x, int y, float z,int x_on_graph, int y_on_graph, int width, int height, bool muki, int alfa)
{
	return put( ptex, (float) x, (float) y, (float)z, x_on_graph, y_on_graph, width, height, muki, alfa);
}

HRESULT CD3DSetup::putScaling(LPDIRECT3DTEXTURE9 ptex, float x, float y, float z,int x_on_graph, int y_on_graph, int width, int height, bool muki, int alfa, float bairitsux, float bairitsuy)
{
	if(ptex == nullptr)
		return E_FAIL;


	D3DXVECTOR3 vCent(width/2, height/2, 0);
	D3DXMATRIXA16 matTranslation;
	D3DXMATRIXA16 matRotation;
	//D3DXMatrixIdentity(&matTranslation);
	D3DXMatrixTranslation(&matTranslation, x + width/2, y + height/2, z);
	D3DXMatrixScaling(&matRotation, (1- ((int)muki)*2)*bairitsux, bairitsuy, 1);
	matTranslation = matRotation*matTranslation;

	HRESULT hr = S_OK;;
	
	if(FAILED(hr))
	{
		//MessageBox(0, _T("�A���t�@�u�����f�B���O�Ɏ��s���܂���\0"), 0, MB_OK);
		return hr;
	}

	RECT rc = {x_on_graph, y_on_graph, x_on_graph+width-1, y_on_graph+height-1};

	psprite->Begin(D3DXSPRITE_ALPHABLEND);
	psprite->SetTransform(&matTranslation);
	psprite->Draw(ptex, &rc, &vCent, NULL, alfa);
	psprite->End();

	hr = lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	hr = lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	hr = lpD3DDEV->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;

}
HRESULT CD3DSetup::putScaling(LPDIRECT3DTEXTURE9 ptex, int x, int y, float z,int x_on_graph, int y_on_graph, int width, int height, bool muki, int alfa, float bairitsux, float bairitsuy)
{
	return putScaling( ptex, (float) x, (float) y, (float) z, x_on_graph, y_on_graph, width, height, muki, alfa, bairitsux, bairitsuy);
}
HRESULT CD3DSetup::putScalingSpinning(LPDIRECT3DTEXTURE9 ptex, float x, float y, float z,int x_on_graph, int y_on_graph, int width, int height, bool muki, int alfa, float bairitsux, float bairitsuy, float theta)
{
	if(ptex == NULL)
		return E_FAIL;
		//return S_OK;


	D3DXVECTOR3 vCent((float)width/2.0, (float)height/2.0, 0);
	//D3DXVECTOR3 vCent(0, 0, 0);
	D3DXMATRIXA16 matTranslation;
	D3DXMATRIXA16 matScale;
	D3DXMATRIXA16 matRotation;
	//D3DXMatrixIdentity(&matTranslation);
	D3DXMatrixTranslation(&matTranslation, x + width/2, y + height/2, z);

	//D3DXMatrixTranslation(&matTranslation, x + width*((int)muki), y, 0);
	//D3DXMatrixScaling(&matScale, ((float)1- (float)((int)muki)*2)*bairitsux, bairitsuy, 1);
	D3DXMatrixScaling(&matScale, bairitsux, bairitsuy, 1);
	D3DXMatrixRotationZ(&matRotation, theta);

	//matTranslation = matScale*matTranslation*matRotation;
	matTranslation = matRotation * matScale * matTranslation;

	HRESULT hr = S_OK;;

	RECT rc = {x_on_graph, y_on_graph, x_on_graph+width, y_on_graph+height};

	psprite->Begin(D3DXSPRITE_ALPHABLEND);
	//psprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_DONOTMODIFY_RENDERSTATE);
	psprite->SetTransform(&matTranslation);
	psprite->Draw(ptex, &rc, &vCent, NULL, alfa);
	psprite->End();

	hr = lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	hr = lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	hr = lpD3DDEV->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	hr = lpD3DDEV->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	hr = lpD3DDEV->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
	hr = lpD3DDEV->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;

}

HRESULT CD3DSetup::putScalingSpinningNoAlpha(LPDIRECT3DTEXTURE9 ptex, float x, float y, float z, int x_on_graph, int y_on_graph, int width, int height, bool muki, int alfa, float bairitsux, float bairitsuy, float theta)
{
	if (ptex == NULL)
		return E_FAIL;


	D3DXVECTOR3 vCent((float)width / 2.0, (float)height / 2.0, 0);
	//D3DXVECTOR3 vCent(0, 0, 0);
	D3DXMATRIXA16 matTranslation;
	D3DXMATRIXA16 matScale;
	D3DXMATRIXA16 matRotation;
	//D3DXMatrixIdentity(&matTranslation);
	D3DXMatrixTranslation(&matTranslation, x + width / 2, y + height / 2, z);

	//D3DXMatrixTranslation(&matTranslation, x + width*((int)muki), y, 0);
	//D3DXMatrixScaling(&matScale, ((float)1- (float)((int)muki)*2)*bairitsux, bairitsuy, 1);
	D3DXMatrixScaling(&matScale, bairitsux, bairitsuy, 1);
	D3DXMatrixRotationZ(&matRotation, theta);

	//matTranslation = matScale*matTranslation*matRotation;
	matTranslation = matRotation * matScale * matTranslation;

	HRESULT hr = S_OK;;

	RECT rc = { x_on_graph, y_on_graph, x_on_graph + width, y_on_graph + height };

	psprite->Begin(0);
	psprite->SetTransform(&matTranslation);
	psprite->Draw(ptex, &rc, &vCent, NULL, alfa);
	psprite->End();

	hr = lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	hr = lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	hr = lpD3DDEV->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;

}


HRESULT CD3DSetup::drawLine(int orix, int oriy, int terx, int tery, int r, int g, int b, int a, int width)
{
	HRESULT res;
	D3DXVECTOR2 vec[] =
	{
		D3DXVECTOR2(orix, oriy),
		D3DXVECTOR2(terx, tery)
	};
	pline->SetWidth(width);
	res = pline->Begin();

	res = pline->Draw(vec, 2, D3DCOLOR_ARGB(a, r, g, b));
	pline->End();
	return res;
}

 //�E�B���h�E�v���V�[�W��
 LRESULT CD3DSetup::MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
 {
 	switch( uMsg )
 	{
 	case WM_DESTROY:
		iloop = 0;
 		PostQuitMessage(0);
		break;
 	case WM_KEYDOWN:
 		switch(wParam)
		{
		case VK_ESCAPE:
 			//�G�X�P�[�v�ŏI��
 			iloop = 0;
 			PostQuitMessage(0);
 			break;
 		}
     }

     return DefWindowProc( hwnd, uMsg, wParam, lParam );
 }

 //�A�v���P�[�V�������[�v
 HRESULT CD3DSetup::AppLoop()
 {
 	HRESULT	hr;
 	MSG msg;
      ZeroMemory( &msg, sizeof(msg) );

    while( iloop )
     {
         while( PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE ) )
         {
 			if (msg.message == WM_QUIT)
 				break;

 			GetMessage(&msg,NULL,0,0);
            TranslateMessage( &msg );
             DispatchMessage( &msg );
         }

 		hr = Render();	//�`��
     }

	return (HRESULT)msg.wParam;
 }

 //�`��
 HRESULT CD3DSetup::Render()
 {
 	HRESULT	hr;

 	hr = lpD3DDEV->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );

 	//�V�[���J�n
 	if( SUCCEEDED( hr = lpD3DDEV->BeginScene() ) )
    {
 		hr = lpD3DDEV->EndScene();
 	}

 	//�o�b�N�o�b�t�@����v���C�}���o�b�t�@�֓]��
 	if(FAILED(hr = lpD3DDEV->Present( NULL, NULL, NULL, NULL )))
 	{
 		lpD3DDEV->Reset(&param);
  	}

 	return hr;
 }

 HRESULT CD3DSetup::StartRendering()
 {
	HRESULT	hr;
	if (!lpD3DDEV)
		return E_FAIL;
	if(FAILED(hr = lpD3DDEV->Clear( 0, NULL,
		D3DCLEAR_TARGET
		| D3DCLEAR_ZBUFFER
		| D3DCLEAR_STENCIL,
		D3DCOLOR_XRGB(0,0,255),
		1.0f,
		0x0000ffff )))
	{
		hr = lpD3DDEV->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0x0000ffff );
	}

 	//�V�[���J�n
 	hr = lpD3DDEV->BeginScene();
	return hr;
 }

 HRESULT CD3DSetup::EndRendering()
 {

	HRESULT	hr;
	hr = lpD3DDEV->EndScene();
	if(FAILED(hr = lpD3DDEV->Present( NULL, NULL, NULL, NULL )))
 	{
 		lpD3DDEV->Reset(&param);
  	}

 	return hr;
 }
 HRESULT CD3DSetup::Clear(int r, int g, int b, int a)
 {
	 HRESULT hr = S_OK;
	 if (a != 255)
	 {
		 LPDIRECT3DSURFACE9 pBackBuffer, pZBuffer;
		 lpD3DDEV->GetRenderTarget(0, &pBackBuffer);
		 lpD3DDEV->GetDepthStencilSurface(&pZBuffer);

		 LPDIRECT3DSURFACE9 pTarget = NULL, pTargetZbuffer = NULL;
		 pforeground->GetSurfaceLevel(0, &pTarget);
		 lpD3DDEV->SetRenderTarget(0, pTarget);
		 //lpD3DDEV->SetDepthStencilSurface(pTargetZbuffer);

		 if (FAILED(hr = lpD3DDEV->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(a, r, g, b), 1.0f, 0x00000000)))
		 {
			 hr = lpD3DDEV->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(a, r, g, b), 1.0f, 0x0000ffff);
		 }

		 //SAFE_RELEASE(pSurface);
		 SAFE_RELEASE(pTarget);

		 lpD3DDEV->SetRenderTarget(0, pBackBuffer);
		 lpD3DDEV->SetDepthStencilSurface(pZBuffer);

		 put(pforeground, 0, 0, .0, 0xffffffff);

		 //Set2DCircle(320, 240, 0.0, 30, 0xffffffff);
		 //Set2DRectangle(0, 0, 30, 30, pstencil);
		 SAFE_RELEASE(pBackBuffer);
		 SAFE_RELEASE(pZBuffer);
		 SAFE_RELEASE(pTargetZbuffer);

		 return hr;
	 }
	 else
	 {
		 if (FAILED(hr = lpD3DDEV->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(a, r, g, b), -1.0f, 0x00000000)))
		 {
			 hr = lpD3DDEV->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(a, r, g, b), -1.0f, 0x0000ffff);
		 }
		 return hr;
	 }
 }

 HRESULT CD3DSetup::Whipe(float x, float y, float r)
 {
	 HRESULT hr = S_OK;

	LPDIRECT3DSURFACE9 pBackBuffer, pZBuffer;
	lpD3DDEV->GetRenderTarget(0, &pBackBuffer);
	lpD3DDEV->GetDepthStencilSurface(&pZBuffer);

	LPDIRECT3DSURFACE9 pTarget = NULL, pTargetZbuffer = NULL;
	pforeground->GetSurfaceLevel(0, &pTarget);
	lpD3DDEV->SetRenderTarget(0, pTarget);
	//lpD3DDEV->SetDepthStencilSurface(pTargetZbuffer);

	if(FAILED(hr = lpD3DDEV->Clear( 0,
		NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL,
		D3DCOLOR_ARGB(255, 0,0, 0), 1.0f, 0x00000000 )))
	{
		return E_FAIL;
	}
	//SAFE_RELEASE(pSurface);
	SAFE_RELEASE(pTarget);

	lpD3DDEV->SetRenderTarget(0, pBackBuffer);
	lpD3DDEV->SetDepthStencilSurface(pZBuffer);

	lpD3DDEV->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	lpD3DDEV->SetRenderState(D3DRS_ZENABLE, TRUE);
	lpD3DDEV->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);
	lpD3DDEV->SetRenderState(D3DRS_STENCILMASK, 0xff);
	lpD3DDEV->SetRenderState(D3DRS_STENCILWRITEMASK, 0xff);
	lpD3DDEV->SetRenderState(D3DRS_STENCILREF, 0x05);
	lpD3DDEV->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	lpD3DDEV->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	lpD3DDEV->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);
	lpD3DDEV->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_REPLACE);

	Set2DCircle(x, y, 0.0, r, 0xff000000);
	//Set2DCircle(400, 400, 0, 20, 0xffff0000);

	//put(pstencil, 0, 0, -1.0, 0, 0, 20, 20, 1, 0xffffffff);

	//lpD3DDEV->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	lpD3DDEV->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	lpD3DDEV->SetRenderState(D3DRS_STENCILREF, 0x00);
	lpD3DDEV->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	lpD3DDEV->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
	lpD3DDEV->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	lpD3DDEV->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);

	Set2DRectangle(0, 0, param.BackBufferWidth, param.BackBufferHeight, pforeground);
	lpD3DDEV->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	//put(pforeground, 0, 0, -1.0, 0xffffffff);
	lpD3DDEV->SetDepthStencilSurface(pZBuffer);

	return hr;
 }

 HRESULT CD3DSetup::Whipe(int time)
 {
	 return Whipe(320, 240, time);
 }

 HRESULT CD3DSetup::SetRenderTarget(LPDIRECT3DSURFACE9 psurface)
 {
	 HRESULT hr = lpD3DDEV->SetRenderTarget(0, psurface);
	 lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	 lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	 lpD3DDEV->SetRenderState(D3DRS_ALPHATESTENABLE, d3daplphatest);
	 lpD3DDEV->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	 return hr;
 }

 LPDIRECT3DSURFACE9 CD3DSetup::getBackBuffer()
 {
	 LPDIRECT3DSURFACE9 ps = nullptr;
	 lpD3DDEV->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &ps);
	 return ps;
 }

 HRESULT CD3DSetup::ScreenShot(const char* file_name)
 {
	 HRESULT hr;
	 auto bb = getBackBuffer();
	 if (!bb)
		 return E_FAIL;

	 hr = D3DXSaveSurfaceToFile(file_name, D3DXIFF_PNG, bb, NULL, NULL);
	 bb->Release();
	 return hr;
 }

HRESULT CD3DSetup::reset()
 {
	outputLog("�f�o�C�X���������܂����B\0");
	while(lpD3DDEV->TestCooperativeLevel() == D3DERR_DEVICELOST)
	{
		 while( PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE ) )
		{
 			if (msg.message == WM_QUIT)
				return E_FAIL;

			GetMessage(&msg,NULL,0,0);
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}
	psprite->OnLostDevice();
	SAFE_DELETE(font);
	SAFE_DELETE(fontw);

	HRESULT hr = lpD3DDEV->TestCooperativeLevel();
	for(int i = 0; i < 99; i++)
	{
		if(SUCCEEDED(hr = lpD3DDEV->Reset(&param)))
		{
			outputLog("�f�o�C�X�����A���܂����B\0");
			break;
		}
		else if (i == 98)
		{
			//MessageBox(0, _T("�f�o�C�X�����������܂ܕ��A���܂���ł����B\0"), 0, MB_OK);
			outputLog("�f�o�C�X�����������܂ܕ��A���܂���ł����B\0");
			return E_FAIL;
		}

		 while( PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE ) )
		{
 			if (msg.message == WM_QUIT)
				return E_FAIL;

			GetMessage(&msg,NULL,0,0);
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		 Sleep(10);
	}
	hr = lpD3DDEV->TestCooperativeLevel();

	font = new DXFONT(lpD3DDEV, 16, 0, 0, 0, "MS �S�V�b�N\0");
	fontw = new DXFONT(lpD3DDEV, 20, 0, -3, 0, "MS �S�V�b�N\0");
	//font = new DXFONT(lpD3DDEV, 16, 0, 0, 0, "Meiryo UI\0");
	//fontw = new DXFONT(lpD3DDEV, 22, 0, 0, 0, "Meiryo UI\0");
	psprite->OnResetDevice();
	lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	lpD3DDEV->SetRenderState(D3DRS_ALPHATESTENABLE, d3daplphatest);
	lpD3DDEV->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
	lpD3DDEV->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	lpD3DDEV->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	lpD3DDEV->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	return S_OK;
 }

void CD3DSetup::resize(int w, int h)
{
	param.BackBufferWidth = w;
	param.BackBufferHeight = h;
	if (pforeground)
		pforeground->Release();
	pforeground = nullptr;
	lpD3DDEV->CreateTexture(w, h, 1, D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pforeground, NULL);

	if (font)
	{
		delete font;
		font = nullptr;
	}
	if (fontw)
	{
		delete fontw;
		fontw = nullptr;
	}
	if (pline)
	{
		pline->Release();
		pline = nullptr;
	}
	psprite->OnLostDevice();

	HRESULT hr = lpD3DDEV->Reset(&param);

	if (hr != D3D_OK)
	{
		std::cout << "d3d reset error" << std::endl;
	}

	lpD3DDEV->CreateTexture(w, h, 1, D3DUSAGE_RENDERTARGET, 
		D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pforeground, NULL);

	if (FAILED(D3DXCreateLine(lpD3DDEV, &pline)))
	{
		outputLog("�X�v���C�g�̍쐬�Ɏ��s���܂����B\0");
	}
	font = new DXFONT(lpD3DDEV, 16, 0, 0, 0, "MS �S�V�b�N\0");
	fontw = new DXFONT(lpD3DDEV, 20, 0, -3, 0, "MS �S�V�b�N\0");

	lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	lpD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	lpD3DDEV->SetRenderState(D3DRS_ALPHATESTENABLE, d3daplphatest);
	lpD3DDEV->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
}


HRESULT OutPutDxDiagLog()
{
   HRESULT hr;
   FILE* f = NULL;
   IDxDiagProvider* pDxDiagProvider = NULL;
   IDxDiagContainer* pDxDiagRoot = NULL;
   IDxDiagContainer* pDxChild = NULL;
   char str[100], strDestination[100];
   VARIANT var;

   fopen_s(&f, "���샍�O.txt\0", "w" );

   hr = CoInitialize(NULL);
   if( FAILED( hr ) )
      return hr;

   hr = CoCreateInstance( CLSID_DxDiagProvider,
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          IID_IDxDiagProvider,
                          (LPVOID*) &pDxDiagProvider );
   if( SUCCEEDED(hr) )
   {
      DXDIAG_INIT_PARAMS dxDiagInitParam;
      ZeroMemory( &dxDiagInitParam, sizeof(DXDIAG_INIT_PARAMS) );
      dxDiagInitParam.dwSize                  = sizeof(DXDIAG_INIT_PARAMS);
      dxDiagInitParam.dwDxDiagHeaderVersion   = DXDIAG_DX9_SDK_VERSION;
      dxDiagInitParam.bAllowWHQLChecks        = false;
      dxDiagInitParam.pReserved               = NULL;

      hr = pDxDiagProvider->Initialize( &dxDiagInitParam );
      if( SUCCEEDED(hr) )
      {
         //���[�g �R���e�i�ɂ��� IDxDiagContainer �I�u�W�F�N�g���쐬���A����������B
         hr = pDxDiagProvider->GetRootContainer( &pDxDiagRoot );
         if( SUCCEEDED(hr) )
         {
            //DirectX�f�f�c�[���̃V�X�e���^�u�̏����擾����B
            hr = pDxDiagRoot->GetChildContainer( L"DxDiag_SystemInfo\0", &pDxChild );
            if( SUCCEEDED(hr) )
            {
               fputs( "--- �V�X�e�� ---\n\0", f );

               //OS�o�[�W����
               VariantInit( &var );
               hr = pDxChild->GetProp( L"szOSExLocalized\0", &var );
               if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
               {
                  WideCharToMultiByte( CP_ACP, 0, var.bstrVal, -1, strDestination, 100*sizeof(CHAR), NULL, NULL );
                  sprintf_s( str, "OS�F %s\n\0", strDestination );
                  fputs( str, f );
               }
               VariantClear( &var );

               //CPU
               VariantInit( &var );
               hr = pDxChild->GetProp( L"szProcessorEnglish\0", &var );
               if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
               {
                  WideCharToMultiByte( CP_ACP, 0, var.bstrVal, -1, strDestination, 100*sizeof(CHAR), NULL, NULL );
                  sprintf_s( str, "CPU�F %s\n\0", strDestination );
                  fputs( str, f );
               }
               VariantClear( &var );

               //SystemMemory�̗e��
               VariantInit( &var );
               hr = pDxChild->GetProp( L"szPhysicalMemoryEnglish\0", &var );
               if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
               {
                  WideCharToMultiByte( CP_ACP, 0, var.bstrVal, -1, strDestination, 100*sizeof(CHAR), NULL, NULL );
                  sprintf_s( str, "�V�X�e���������F %s\n\0", strDestination );
                  fputs( str, f );
               }
               VariantClear( &var );

               //DirectX�����^�C���̃o�[�W����
               VariantInit( &var );
               hr = pDxChild->GetProp( L"szDirectXVersionLongEnglish\0", &var );
               if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
               {
                  WideCharToMultiByte( CP_ACP, 0, var.bstrVal, -1, strDestination, 100*sizeof(CHAR), NULL, NULL );
                  sprintf_s( str, "DirectX�̃o�[�W�����F %s\n\0", strDestination );
                  fputs( str, f );
               }
               VariantClear( &var );

               //DirectXMode
               VariantInit( &var );
               hr = pDxChild->GetProp( L"bIsD3DDebugRuntime\0", &var );
               if( SUCCEEDED(hr) && var.vt == VT_BOOL )
               {
                  if( var.boolVal == TRUE )
                     sprintf_s( str, "DirectXMode�F DebugRuntime\n" );
                  else
                  sprintf_s( str, "DirectXMode�F RetailRuntime\n" );
                  fputs( str, f );
               }
               VariantClear( &var );

               pDxChild->Release();
            }

            //�f�B�X�v���C�^�u�̏����擾����B
            hr = pDxDiagRoot->GetChildContainer( L"DxDiag_DisplayDevices\0", &pDxChild );
            if( SUCCEEDED(hr) )
            {
               DWORD DisplayCnt;
               WCHAR wszContainer[100];
               IDxDiagContainer* pDxDisplay = NULL;

               //�f�B�X�v���C�A�_�v�^�̐����擾����B
               pDxChild->GetNumberOfChildContainers( &DisplayCnt );

               for( DWORD i=0; i < DisplayCnt; i++ )
               {
                  sprintf_s( str, "--- �f�B�X�v���C�A�_�v�^%d ---\n\0", i );
                  fputs( str, f );

                  //�f�B�X�v���C�̗�
                  hr = pDxChild->EnumChildContainerNames( i, wszContainer, 100 );
                  if( SUCCEEDED( hr ) )
                  {
                     hr = pDxChild->GetChildContainer( wszContainer, &pDxDisplay );
                     if( SUCCEEDED( hr ) )
                     {
                        //****************************************************************
                        //�`�b�v�̎��
                        //****************************************************************
                        VariantInit( &var );
                        hr = pDxDisplay->GetProp( L"szChipType\0", &var );
                        if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
                        {
                           WideCharToMultiByte( CP_ACP, 0, var.bstrVal, -1, strDestination, 100*sizeof(CHAR), NULL, NULL );
                           sprintf_s( str, "Chip�F %s\n\0", strDestination );
                           fputs( str, f );
                        }
                        VariantClear( &var );

                        //****************************************************************
                        //VRAM
                        //****************************************************************
                        VariantInit( &var );
                        hr = pDxDisplay->GetProp( L"szDisplayMemoryEnglish\0", &var );
                        if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
                        {
                           WideCharToMultiByte( CP_ACP, 0, var.bstrVal, -1, strDestination, 100*sizeof(CHAR), NULL, NULL );
                           sprintf_s( str, "VRAM�F %s\n\0", strDestination );
                           fputs( str, f );
                        }
                        VariantClear( &var );

                        //****************************************************************
                        //DDI
                        //****************************************************************
                        VariantInit( &var );
                        hr = pDxDisplay->GetProp( L"szDDIVersionEnglish\0", &var );
                        if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
                        {
                           WideCharToMultiByte( CP_ACP, 0, var.bstrVal, -1, strDestination, 100*sizeof(CHAR), NULL, NULL );
                           sprintf_s( str, "�Ή�DirectX�o�[�W�����F %s\n\0", strDestination );
                           fputs( str, f );
                        }
                        VariantClear( &var );

                        //****************************************************************
                        //DirectDraw�A�N�Z�����[�^
                        //****************************************************************
                        VariantInit( &var );
                        hr = pDxDisplay->GetProp( L"bDDAccelerationEnabled\0", &var );
                        if( SUCCEEDED(hr) && var.vt == VT_BOOL )
                        {
                           if( var.boolVal == TRUE )
                              sprintf_s( str, "DirectDraw�F �g�p�\\n" );
                           else
                              sprintf_s( str, "DirectDraw�F ����\n" );
                           fputs( str, f );
                        }
                        VariantClear( &var );

                        //****************************************************************
                        //Direct3D�A�N�Z�����[�^
                        //****************************************************************
                        VariantInit( &var );
                        hr = pDxDisplay->GetProp( L"b3DAccelerationEnabled\0", &var );
                        if( SUCCEEDED(hr) && var.vt == VT_BOOL )
                        {
                           if( var.boolVal == TRUE )
                              sprintf_s( str, "Direct3D�F �g�p�\\n" );
                           else
                              sprintf_s( str, "Direct3D�F ����\n" );
                           fputs( str, f );
                        }
                        VariantClear( &var );

                        //****************************************************************
                        //AGP�A�N�Z�����[�^
                        //****************************************************************
                        VariantInit( &var );
                        hr = pDxDisplay->GetProp( L"bAGPEnabled\0", &var );
                        if( SUCCEEDED(hr) && var.vt == VT_BOOL )
                        {
                           if( var.boolVal == TRUE )
                              sprintf_s( str, "AGP�F �g�p�\\n" );
                           else
                              sprintf_s( str, "AGP�F ����\n" );
                           fputs( str, f );
                        }
                        VariantClear( &var );

                        pDxDisplay->Release();
                     }
                  }
               }
               pDxChild->Release();
            }
			//****************************************************************
            //DirectX�f�f�c�[���̃V�X�e���^�u�̏����擾����B
            //****************************************************************
            hr = pDxDiagRoot->GetChildContainer( L"DxDiag_SystemInfo\0", &pDxChild );
            if( SUCCEEDED(hr) )
            {
               fputs( "--- �V�X�e�� ---\n\0", f );

               //****************************************************************
               //OS�o�[�W����
               //****************************************************************
               VariantInit( &var );
               hr = pDxChild->GetProp( L"szOSExLocalized\0", &var );
               if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
               {
                  WideCharToMultiByte( CP_ACP, 0, var.bstrVal, -1, strDestination, 100*sizeof(CHAR), NULL, NULL );
                  sprintf_s( str, "OS�F %s\n\0", strDestination );
                  fputs( str, f );
               }
               VariantClear( &var );

               //****************************************************************
               //CPU
               //****************************************************************
               VariantInit( &var );
               hr = pDxChild->GetProp( L"szProcessorEnglish\0", &var );
               if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
               {
                  WideCharToMultiByte( CP_ACP, 0, var.bstrVal, -1, strDestination, 100*sizeof(CHAR), NULL, NULL );
                  sprintf_s( str, "CPU�F %s\n\0", strDestination );
                  fputs( str, f );
               }
               VariantClear( &var );

               //****************************************************************
               //SystemMemory�̗e��
               //****************************************************************
               VariantInit( &var );
               hr = pDxChild->GetProp( L"szPhysicalMemoryEnglish\0", &var );
               if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
               {
                  WideCharToMultiByte( CP_ACP, 0, var.bstrVal, -1, strDestination, 100*sizeof(CHAR), NULL, NULL );
                  sprintf_s( str, "�V�X�e���������F %s\n\0", strDestination );
                  fputs( str, f );
               }
               VariantClear( &var );

               //****************************************************************
               //DirectX�����^�C���̃o�[�W����
               //****************************************************************
               VariantInit( &var );
               hr = pDxChild->GetProp( L"szDirectXVersionLongEnglish\0", &var );
               if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
               {
                  WideCharToMultiByte( CP_ACP, 0, var.bstrVal, -1, strDestination, 100*sizeof(CHAR), NULL, NULL );
                  sprintf_s( str, "DirectX�̃o�[�W�����F %s\n\0", strDestination );
                  fputs( str, f );
               }
               VariantClear( &var );

               //****************************************************************
               //DirectXMode
               //****************************************************************
               VariantInit( &var );
               hr = pDxChild->GetProp( L"bIsD3DDebugRuntime\0", &var );
               if( SUCCEEDED(hr) && var.vt == VT_BOOL )
               {
                  if( var.boolVal == TRUE )
                     sprintf_s( str, "DirectXMode�F DebugRuntime\n" );
                  else
                  sprintf_s( str, "DirectXMode�F RetailRuntime\n" );
                  fputs( str, f );
               }
               VariantClear( &var );

               pDxChild->Release();
            }
            pDxDiagRoot->Release();
         }
      }
      pDxDiagProvider->Release();
   }

   CoUninitialize();

   fputs( "===================================\n\0", f );

   fclose( f );
}



 //FPS�Ď��N���X
 FPSChecker::FPSChecker()
 {
	NumFrame = 0;
	looptime_ave = 0;
	looptime_sum = 0;
	time = timeGetTime();
	byougaochi = 0;
 }

 void FPSChecker::run()
 {
	const int FPS = 16;
	int curtime = timeGetTime();
	int looptime = curtime - time;
	if(looptime<FPS)Sleep((FPS-looptime));
	looptime = timeGetTime() - time;
	time = curtime;
	if(byougaochi)
		byougaochi = false;
	else byougaochi = looptime > 16;
	//byougaochi = 0;

	if(NumFrame < 10)
	{
		looptime_sum += looptime;

		NumFrame ++;
	}

	if(NumFrame == 10)
	{
		NumFrame = 0;
		looptime_ave = looptime_sum / 10;
		looptime_sum = 0;
	}
 }
