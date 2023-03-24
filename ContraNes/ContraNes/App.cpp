#define _USE_MATH_DEFINES
#include<cmath>
#include <d3dx9.h>
#include "Keyboard.h"
#include "App.h"
#include "d3dUtil.h"
#include "Player.h"

App::App(HINSTANCE hInstance) : ContraApp(hInstance)
{
	width = m_uiClientWidth;
	height = m_uiClientHeight;
}

App::~App()
{
	if (player)
	{
		delete player;
		player = nullptr;
	}
	if (background)
	{
		delete background;
		background = nullptr;
	}
	if (camera) 
	{ 
		delete camera; 
		camera = nullptr; 
	}
	if (gameTime) 
	{
		delete gameTime;
		gameTime = nullptr;
	}
	if (font) 
	{
		font->Release();
		font = nullptr;
	}
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

	background = new GameplayObject(0, 0, 1.0f, 0, 0, 0);
	if (!background->Init(m_pDevice3D, L"resources/l1.png", 6771, 480)) return false;
	camera->SetLimit(background->GetPosition().x, background->GetPosition().y, background->GetSprite()->spriteWidth * 1.0f, background->GetSprite()->spriteHeight * 1.0f);

	player = new Player(5, 5, 0, (float)0, 300, 300);
	if (!player->Init(m_pDevice3D)) return false;
	player->SetJerkIncrementPerSecond3(19702.0f);

	player2 = new GameplayObject(5, 5, 0, (float)0, 100, 300);
	if (!player2->Init(m_pDevice3D, L"resources/tank-trans.png", 67, 68, (float)M_PI)) return false;
	return true;
}

D3DXMATRIX view, proj;

bool App::Init()
{
	if (!ContraApp::Init())
		return false;

	camera = new Camera(0, 0, 0, width, height, 0, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));


	// initialize game objects / check game object creation
	if (!InitObjects())
		return false;

	// initialize game time
	gameTime = new GameTime();
	if (!gameTime->Initialize())
		return false;

	font = NULL;
	message = L"Hello World!";
	HRESULT hr = D3DXCreateFont(m_pDevice3D, 19, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &font);
	if (FAILED(hr)) 
	{
		MessageBox(NULL, L"Failed to create font", NULL, NULL);
		return false;
	}
	SetRect(&messageRect, 0, 0, 500, 200);
	
	

	return true;
}

void App::Update(float gameTime)
{
	if (GetAsyncKeyState(VK_ESCAPE)) 
	{
		PostQuitMessage(0);
	}

	if (player && player->IsInitialized())
	{
		player->HandleInput(gameTime);
		player->Update(gameTime);

	}

	if (player2 && player2->IsInitialized())
	{
		player2->Update(gameTime);

	}

	if (camera) {
		if (GetAsyncKeyState(70)) // F
		{
			if (!camera->IsFollowing()) 
			{
				camera->Follow(player);
			}
		}
		if (GetAsyncKeyState(85)) // U
		{
			if (camera->IsFollowing()) {
				camera->Unfollow();
			}
		}
		camera->Update(gameTime);
	}

}

void App::Render(float gameTime)
{
	m_pDevice3D->Clear(0, 0,
		D3DCLEAR_TARGET ,
		D3DCOLOR_XRGB(0, 100, 100), 1.0f, 0);

	m_pDevice3D->BeginScene(); 

	if (camera) {
		if (background && background->IsInitialized())
		{
			camera->Render(background);
		}

		if (player && player->IsInitialized())
		{
			camera->Render(player);
		}

		if (player2 && player2->IsInitialized())
		{
			camera->Render(player2);
		}
	}

	if (font) {
		font->DrawTextW(NULL, message, -1, &messageRect, DT_LEFT, D3DCOLOR_ARGB(255, 255, 255, 0));
	}

	m_pDevice3D->EndScene();
	m_pDevice3D->Present(0, 0, 0, 0);

}