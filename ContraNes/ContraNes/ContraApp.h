#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <string>
#include <d3dx9.h>

class ContraApp
{
public:
	// constructor and destructor
	ContraApp(HINSTANCE hInstance);
	virtual ~ContraApp();

	// main application loop
	int Run();

	IDirect3DDevice9* GetDevice() const;

	virtual void _Run();

	// framework methods
	virtual bool Init();
	virtual void Update(float dt) = 0;
	virtual void Render(float gameTime) = 0;
	LRESULT CALLBACK MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	// attributes
	HWND			m_hAppWnd;
	HINSTANCE		m_hAppInstance;
	unsigned int	m_uiClientWidth;
	unsigned int	m_uiClientHeight;
	std::string		m_strAppTitle;
	LPCWSTR 	    m_LAppTitle;
	DWORD			m_WindowStyle;
	int				m_prevX;
	int				m_prevY;

	// directx attributes
	LPDIRECT3D9				m_pDirect3D;
	LPDIRECT3DDEVICE9		m_pDevice3D;
	D3DPRESENT_PARAMETERS	m_d3dpp;

	// methods

	// initialize
	bool InitWindow();

	// initialize Direct3D
	bool InitDirect3D();
};