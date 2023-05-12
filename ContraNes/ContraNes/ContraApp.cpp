#include "ContraApp.h"
#include "d3dUtil.h"
#include "GameSprite.h"
#include "CONSTANTS.H"

namespace
{
	// create a global pointer to ContraApp
	ContraApp* g_pApp = NULL;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// forward messages to ContraApp
	if (g_pApp)
		return g_pApp->MsgProc(hwnd, msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

ContraApp::ContraApp(HINSTANCE hInstance) 
{
	m_hAppInstance = hInstance;
	m_hAppWnd = NULL; 
	m_uiClientWidth = DEFAULT_SCREEN_WIDTH;
	m_uiClientHeight = DEFAULT_SCREEN_HEIGHT;
	/*m_uiClientWidth = 1024;
	m_uiClientHeight = 960;*/
	/*m_uiClientWidth = 1280;
	m_uiClientWidth = 960;*/
	m_strAppTitle = "ContraNes";
	m_LAppTitle = ConvertToLPCWSTR(m_strAppTitle);
	m_WindowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	g_pApp = this;
}

ContraApp::~ContraApp()
{
	// release Direct3D
	if (m_pDevice3D) {
		m_pDevice3D->Release();
		m_pDevice3D = NULL;
	}

	if (m_pDirect3D) 
	{
		m_pDirect3D->Release();
		m_pDirect3D = NULL; 
	}

}

int ContraApp::Run()
{
	// main message loop
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			this->_Run();
		}
	}

	return static_cast<int>(msg.wParam);
}

void ContraApp::_Run()
{
	// update game
	Update(0.0f);

	// render game
	Render(0.0f);
}

bool ContraApp::Init()
{
	// initialize window / check window creation
	if (!InitWindow())
		return false;

	// initialize Direct3D / check Direct3D creation
	if (!InitDirect3D())
		return false;

	return true;
}

bool ContraApp::InitWindow()
{
	// create window class
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.lpfnWndProc = MainWndProc;
	wcex.hInstance = m_hAppInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszClassName = m_LAppTitle;
	wcex.lpszMenuName = NULL;
	wcex.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	// register window class
	if (!RegisterClassEx(&wcex)) 
	{
		MessageBox(NULL, TEXT("Failed to register window class"), NULL, NULL);
		return false;
	}

	// cache client dimensions
	RECT rc = { 0, 0, static_cast<LONG>(m_uiClientWidth), static_cast<LONG>(m_uiClientHeight) };
	AdjustWindowRect(&rc, m_WindowStyle, FALSE);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	// create window
	m_hAppWnd = CreateWindow(
		TEXT("ContraNes"),
		m_LAppTitle,
		m_WindowStyle,
		GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2,
		GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2,
		width,
		height,
		NULL,
		NULL,
		m_hAppInstance,
		NULL
	);

	// check window creation
	if (!m_hAppWnd)
	{
		MessageBox(NULL, TEXT("Failed to create window"), NULL, NULL);
	}

	// show window
	ShowWindow(m_hAppWnd, SW_SHOW);
	
	return true;
}

bool ContraApp::InitDirect3D()
{
	// get interface to Direct3D
	m_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!m_pDirect3D)
	{
		MessageBox(NULL, TEXT("Failed to create Direct3D interface"), NULL, NULL);
		return false;
	}

	// check device capabilities
	D3DCAPS9 m_d3dDevCaps;
	m_pDirect3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_d3dDevCaps);

	// set vertex processing
	int vp;
	if (m_d3dDevCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// fill out the present parameters
	ZeroMemory(&m_d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	m_d3dpp.Windowed = TRUE;
	m_d3dpp.hDeviceWindow = m_hAppWnd;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	/*m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	m_d3dpp.BackBufferWidth = m_uiClientWidth;
	m_d3dpp.BackBufferHeight = m_uiClientHeight;
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	m_d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	m_d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	m_d3dpp.BackBufferCount = 1;
	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_d3dpp.MultiSampleQuality = 0;*/
	
	
	// create device
	m_pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		m_hAppWnd,
		vp,
		&m_d3dpp,
		&m_pDevice3D
	);

	// check device creation
	if (!m_pDevice3D)
	{
		MessageBox(NULL, TEXT("Failed to create Direct3D device"), NULL, NULL);
		return false;
	}

	/*D3DVIEWPORT9 viewport;
	ZeroMemory(&viewport, sizeof(D3DVIEWPORT9));
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = m_uiClientWidth;
	viewport.Height = m_uiClientHeight;
	viewport.MinZ = 0.0f;
	viewport.MaxZ = 1.0f;

	m_pDevice3D->SetViewport(&viewport);*/

	return true;
}

LRESULT CALLBACK ContraApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

IDirect3DDevice9* ContraApp::GetDevice() const
{
	return m_pDevice3D;
}