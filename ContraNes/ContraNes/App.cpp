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
	platforms.push_back(new StandableObject(-100,480,10000,1000, false)); // 0
	platforms.push_back(new StandableObject(50, 200, 1450, 50, true)); // 1
	platforms.push_back(new StandableObject(300, 270, 200, 50, true));
	platforms.push_back(new StandableObject(500, 340, 60, 50, true));
	platforms.push_back(new StandableObject(560, 400, 130, 50, true));
	platforms.push_back(new StandableObject(690, 340, 60, 50, true)); // 5
	platforms.push_back(new StandableObject(800, 270, 140, 50, true));
	platforms.push_back(new StandableObject(1190, 400, 140, 50, true));
	platforms.push_back(new StandableObject(1250, 300, 190, 50, true));


	background = new GameplayObject(0, 0, 1.0f, 0, 0, 0);
	if (!background->Init(m_pDevice3D, L"resources/l1.png", 6771, 480)) return false;
	camera->SetLimit(background->GetPosition().x, background->GetPosition().y, background->GetSprite()->spriteWidth * 1.0f, background->GetSprite()->spriteHeight * 1.0f);

	player = new Player(100, 5, 0, (float)0, 300, 300, D3DXVECTOR3(2, 2, 1));
	if (!player->Init(m_pDevice3D, 0.15f)) return false;
	player->SetJerkIncrementPerSecond3(19702.0f);
	player->SetBaseJumpVelocity(400);
	player->SetGravitationalAcceleration(1000);

	player2 = new GameplayObject(5, 5, 0, (float)0, 100, 300, D3DXVECTOR3(2, 2, 1));
	if (!player2->Init(m_pDevice3D, L"resources/tank-trans.png", 67, 68, (float)M_PI)) return false;
	return true;
}

D3DXMATRIX view, proj;

bool App::Init()
{
	if (!ContraApp::Init())
		return false;

	camera = new Camera(0, 0, 0, width, height, 0, D3DXVECTOR3(2.0f, 2.0f, 1.0f));


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

	if (player2 && player2->IsInitialized())
	{
		player2->Update(gameTime);

	}

	if (player && player->IsInitialized())
	{
		player->HandleInput(gameTime);
		player->Update(gameTime);
	}
	RECT playerRect = { (LONG)player->GetPosition().x, (LONG)player->GetPosition().y, (LONG)(player->GetPosition().x + player->GetSprite()->spriteWidth), (LONG)(player->GetPosition().y + player->GetSprite()->spriteHeight) };
	for (auto& platform : platforms)
	{
		RECT platformRect = platform->GetBounds();
		//OutputDebugString(ConvertToLPCWSTR(std::to_string(platformRect.left) + " " + std::to_string(platformRect.top) + " " + std::to_string(platformRect.right) + " " + std::to_string(platformRect.bottom) + "\n"));
		//OutputDebugString(ConvertToLPCWSTR("P: " + std::to_string(playerRect.left) + " " + std::to_string(playerRect.top) + " " + std::to_string(playerRect.right) + " " + std::to_string(playerRect.bottom) + "\n"));
		// check collision
		if ( CheckIntersection( &playerRect, &platformRect) )
		{
			platform->ApplyCollision(player, gameTime);
		}
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
		message = ConvertToLPCWSTR(std::to_string(player->GetPosition().x) + " " + std::to_string(player->GetPosition().y) + "\n" + std::to_string(player->GetPosition().x + player->GetSprite()->spriteWidth) + " " + std::to_string(player->GetPosition().y + player->GetSprite()->spriteHeight));
		font->DrawTextW(NULL, message, -1, &messageRect, DT_LEFT, D3DCOLOR_ARGB(255, 255, 255, 0));
	}

	m_pDevice3D->EndScene();
	m_pDevice3D->Present(0, 0, 0, 0);

}