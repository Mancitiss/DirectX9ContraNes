#define _USE_MATH_DEFINES
#include<cmath>
#include <d3dx9.h>

#include "App.h"
#include "d3dUtil.h"

App::App(HINSTANCE hInstance) : ContraApp(hInstance)
{

}

App::~App()
{

}

void App::_Run() 
{
	gameTime->Update();
	Update(gameTime->elapsedGameTime);
	Render(gameTime->elapsedGameTime);
}

ID3DXSprite* pSprite;

bool App::InitObjects()
{
	player = new GameplayObject(5, 5, 1, (float)M_PI_4, 300, 300);
	if (!player->Init(m_pDevice3D, L"resources/tank.png", 67, 68)) return false;

	return true;
}

D3DXMATRIX view, proj;

bool App::Init()
{
	if (!ContraApp::Init())
		return false;

	// initialize game objects / check game object creation
	if (!InitObjects())
		return false;

	// initialize game time
	gameTime = new GameTime();
	if (!gameTime->Initialize())
		return false;

	return true;
}

void App::Update(float gameTime)
{
	if (player && player->IsInitialized())
	{
		player->HandleInput();
		player->Update(gameTime);

	}

}

void App::Render(float gameTime)
{
	m_pDevice3D->Clear(0, 0,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		d3dColors::Black, 1.0f, 0);
	m_pDevice3D->BeginScene();

	if (player && player->IsInitialized()) 
	{
		player->Draw(0);
	}

	m_pDevice3D->EndScene();
	m_pDevice3D->Present(0, 0, 0, 0);

}