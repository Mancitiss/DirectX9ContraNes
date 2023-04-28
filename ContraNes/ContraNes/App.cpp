#define _USE_MATH_DEFINES
#include<cmath>
#include <d3dx9.h>
#include "Keyboard.h"
#include "App.h"
#include "d3dUtil.h"
#include "Runner.h"

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
	if (player2)
	{
		delete player2;
		player2 = nullptr;
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
	for (auto& platform : platforms)
	{
		if (platform)
		{
			delete platform;
			platform = nullptr;
		}
	}
	for (auto& monster : monsters)
	{
		if (monster)
		{
			delete monster;
			monster = nullptr;
		}
	}
	for (auto& bullet : playerBullets)
	{
		if (bullet)
		{
			delete bullet;
			bullet = nullptr;
		}
	}
	for (auto& bullet : monsterBullets)
	{
		if (bullet)
		{
			delete bullet;
			bullet = nullptr;
		}
	}

}

int App::Run()
{
	// main message loop
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case WM_QUIT:
				return static_cast<int>(msg.wParam);
			default:
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				break;
			}
		}
		//else
		//{
			this->_Run();
		//}
	}

	return static_cast<int>(msg.wParam);
}

void App::_Run() 
{
	static constexpr float kTargetFrameTime = 1.0f / 60.0f; // 60 FPS
	static float sAccumulatedTime = 0.0f;

	gameTime->Update();

	sAccumulatedTime += gameTime->elapsedGameTime;
	while ( sAccumulatedTime >= kTargetFrameTime )
	{
		Update(kTargetFrameTime);
		Render(kTargetFrameTime);

		sAccumulatedTime -= kTargetFrameTime;
	}
}

ID3DXSprite* pSprite;

bool App::InitObjects()
{
	platforms.push_back(new StandableObject(-100,480,10000,1000, false)); // 0
	platforms.push_back(new StandableObject(50, 210, 1450, 50, true)); // 1
	platforms.push_back(new StandableObject(300, 270, 200, 50, true));
	platforms.push_back(new StandableObject(500, 340, 60, 50, true));
	platforms.push_back(new StandableObject(560, 400, 130, 50, true));
	platforms.push_back(new StandableObject(690, 340, 60, 50, true)); // 5
	platforms.push_back(new StandableObject(800, 270, 140, 50, true));
	platforms.push_back(new StandableObject(1190, 400, 140, 50, true));
	platforms.push_back(new StandableObject(1250, 300, 190, 150, true));
	platforms.push_back(new StandableObject(1750, 210, 320, 150, true));
	platforms.push_back(new StandableObject(2320, 210, 500, 160, true)); // 10
	platforms.push_back(new StandableObject(2700, 150, 1000, 250, true));
	platforms.push_back(new StandableObject(2760, 390, 200, 30, true));
	platforms.push_back(new StandableObject(2950, 300, 120, 90, true));
	platforms.push_back(new StandableObject(3130, 270, 400, 80, true));
	platforms.push_back(new StandableObject(3390, 390, 360, 30, true)); // 15

	monsters.push_back(new Runner(1450, 140, 0, 100, 100));
	monsters[0]->Init(m_pDevice3D, 0.15f);


	background = new GameplayObject(0, 0, 1.0f, 0, 0, 0);
	if (!background->Init(m_pDevice3D, L"resources/l1.png", 6771, 480)) return false;
	camera->SetLimit(background->GetPosition().x, background->GetPosition().y, background->GetSprite()->spriteWidth * 1.0f, background->GetSprite()->spriteHeight * 1.0f);

	player = new Player(100, 5, 0, (float)0, 200, 200, D3DXVECTOR3(2, 2, 1));
	if (!player->Init(m_pDevice3D, 0.15f)) return false;
	player->SetJerkIncrementPerSecond3(19702.0f);
	player->SetBaseJumpVelocity(350);
	player->SetGravitationalAcceleration(600);

	player2 = new GameplayObject(5, 5, 0, (float)0, 100, 300, D3DXVECTOR3(1, 1, 1));
	if (!player2->Init(m_pDevice3D, L"resources/tank-trans.png", 67, 68, (float)M_PI)) return false;
	return true;
}

D3DXMATRIX view, proj;

bool App::Init()
{
	if (!ContraApp::Init())
		return false;

	int cWidth = (m_uiClientWidth / 512) * 512;
	int cHeight = (m_uiClientHeight / 480) * 480;
	D3DXVECTOR3 scale = D3DXVECTOR3((float)cWidth / 512, (float)cHeight / 480, 1.0f);
	camera = new Camera(0, 0, 0, cWidth, cHeight, 0, scale);


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
	if ((GetAsyncKeyState(VK_F3) & 0x8001) == 0x8001)
	{
		debugMode = !debugMode;
	}
	if ((GetAsyncKeyState(VK_ESCAPE) & 0x8001) == 0x8001) 
	{
		pause = !pause;
	}
	if (pause) return;

	if (player2 && player2->IsInitialized())
	{
		player2->Update(gameTime);

	}

	if (player && player->IsInitialized())
	{
		player->HandleInput(gameTime);
		player->Update(gameTime);

		if (player->fired)
		{
			player->fired = false;
			// determine direction
			if (player->GetDirection().x == 0.0f && player->GetDirection().y == 0.0f)
			{
				// facing
				if (player->GetFacing() == Facing::RIGHT)
				{
// right
					playerBullets.push_back(new Bullet(player->GetPosition().x + player->GetSprite()->spriteWidth, player->GetPosition().y + player->GetSprite()->spriteHeight / 2));
					playerBullets[playerBullets.size() - 1]->Init(m_pDevice3D);
				}
				else
				{
					// left
					playerBullets.push_back(new Bullet(player->GetPosition().x, player->GetPosition().y + player->GetSprite()->spriteHeight / 2, D3DXToRadian(180) ));
					playerBullets[playerBullets.size() - 1]->Init(m_pDevice3D);
				}
			}
			else
			{
				// moving
				if (player->GetDirection().x > 0.0f)
				{
					// right
					playerBullets.push_back(new Bullet(player->GetPosition().x + player->GetSprite()->spriteWidth, player->GetPosition().y + player->GetSprite()->spriteHeight / 2));
					playerBullets[playerBullets.size() - 1]->Init(m_pDevice3D);
				}
				else
				{
					// left
					playerBullets.push_back(new Bullet(player->GetPosition().x, player->GetPosition().y + player->GetSprite()->spriteHeight / 2, D3DXToRadian(180)));
					playerBullets[playerBullets.size() - 1]->Init(m_pDevice3D);
				}
			}
		}

		if (player->respawn)
		{
			// find direction of the map 
			D3DXVECTOR3 dir = D3DXVECTOR3(0, 0, 0);
			for (int i = 0; i < platforms.size() - 1; i++)
			{
				if (platforms[i]->GetBounds().left < platforms[i + 1]->GetBounds().left)
				{
					dir.x += 1;
				}
				else if (platforms[i]->GetBounds().left > platforms[i + 1]->GetBounds().left)
				{
					dir.x -= 1;
				}
			}
			for (int i = 0; i < platforms.size() - 1; i++)
			{
				if (platforms[i]->GetBounds().top < platforms[i + 1]->GetBounds().top)
				{
					dir.y += 1;
				}
				else if (platforms[i]->GetBounds().top > platforms[i + 1]->GetBounds().top)
				{
					dir.y -= 1;
				}
			}

			// find the nearest platform to respawn
			float minDistance = 1000000;
			int minIndex = 0;
			for (int i = 0; i < platforms.size(); i++)
			{
				// check if the platform is behind the player by using the direction vector above
				D3DXVECTOR3 minus = player->GetPosition() - platforms[i]->GetPosition();
				if (D3DXVec3Dot(&minus, &dir) < 0)
					continue;
				float distance = D3DXVec3Length(&minus);
				if (distance < minDistance)
				{
					minDistance = distance;
					minIndex = i;
				}
			}
			player->SetPositionX(platforms[minIndex]->GetPosition().x);
			player->SetPositionY(platforms[minIndex]->GetPosition().y);

			player->respawn = false;

		}
	}

	RECT playerRect = { (LONG)player->GetPosition().x, (LONG)player->GetPosition().y, (LONG)(player->GetPosition().x + player->GetSprite()->spriteWidth), (LONG)(player->GetPosition().y + player->GetSprite()->spriteHeight) };

	for (auto& platform : platforms)
	{
		RECT platformRect = platform->GetBounds();
		if ( CheckIntersection( &playerRect, &platformRect) && (player->ignore.find(platform) == player->ignore.end()) )
		{
			//player->currentCollaped++;
			platform->ApplyCollision(player, gameTime);
		}
		else if (player->ignore.find(platform) != player->ignore.end())
		{
			player->ignore.erase(platform);
		}
	}

	for (auto& monster : monsters)
	{
		if (!monster || !monster->IsInitialized() || monster->GetMoveType() == MoveType::NONE) continue;
		monster->HandleInput(gameTime);
		monster->Update(gameTime);
		RECT monsterRect = { (LONG)monster->GetPosition().x, (LONG)monster->GetPosition().y, (LONG)(monster->GetPosition().x + monster->GetSprite()->spriteWidth), (LONG)(monster->GetPosition().y + monster->GetSprite()->spriteHeight) };
		for (auto& platform : platforms)
		{
			RECT platformRect = platform->GetBounds();
			if (CheckIntersection(&monsterRect, &platformRect))
			{
				platform->ApplyCollision(monster, gameTime);
				break;
			}
		}
	}

	for (auto& bullet : playerBullets)
	{
		if (!bullet || !bullet->IsInitialized()) continue;
		bullet->Update(gameTime);
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

		RECT cam = camera->GetBounds();
		for (auto& monster : monsters)
		{
			if (monster && monster->IsInitialized())
			{
				// log monster position
				//OutputDebugString(ConvertToLPCWSTR("M: " + std::to_string(monster->GetPosition().x) + " " + std::to_string(monster->GetPosition().y) + ""));
				RECT monsterRect = { (LONG)monster->GetPosition().x, (LONG)monster->GetPosition().y, (LONG)(monster->GetPosition().x + monster->GetSprite()->spriteWidth), (LONG)(monster->GetPosition().y + monster->GetSprite()->spriteHeight) };
				if (CheckIntersection(&monsterRect, &cam))
				{
					monster->SetMoveType(MoveType::LEFT);
				}
				else if (monster->GetMoveType() != MoveType::NONE || monster->GetStatus() == ObjectStatus::DEAD )
				{
					// delete monster and remove it from the vector
					monsters.erase(std::remove(monsters.begin(), monsters.end(), monster), monsters.end());
					//delete monster; // don't have to delete it, it will be deleted by the vector
					monster = NULL;
					// log
					//OutputDebugString(L"Monster deleted\n");
				}

				if (CheckIntersection(&playerRect, &monsterRect))
				{
					monster->ApplyCollision(player, gameTime);
				}
			}
		}

		for (auto& bullet : playerBullets)
		{
			if (!bullet || !bullet->IsInitialized()) continue;
			RECT bulletRect = bullet->GetBounds();
			if (!CheckIntersection(&bulletRect, &cam) || bullet->GetStatus() == ObjectStatus::DEAD )
			{
				// delete bullet and remove it from the vector
				playerBullets.erase(std::remove(playerBullets.begin(), playerBullets.end(), bullet), playerBullets.end());
				bullet = NULL;
				// log
				//OutputDebugString(L"Bullet deleted\n");
			}
			else
			{
				for (auto& monster : monsters)
				{
					if (!monster || !monster->IsInitialized()) continue;
					RECT monsterRect = { (LONG)monster->GetPosition().x, (LONG)monster->GetPosition().y, (LONG)(monster->GetPosition().x + monster->GetSprite()->spriteWidth), (LONG)(monster->GetPosition().y + monster->GetSprite()->spriteHeight) };
					if (!CheckIntersection(&bulletRect, &monsterRect)) continue;
					bullet->ApplyCollision(monster);
					break;
				}
			}
		}
	}

	if (player->GetPosition().y > 10000) 
	{ 
		// reset player position if they fall off the map somehow
		// we will do the same for other objects later
		// but instead of resetting their position, we will just destroy them
		player->SetPositionY(0);
	}

	if (player->GetPosition().x < 0) 
	{
		player->SetPositionX(0);
	}

	//OutputDebugString(ConvertToLPCWSTR("Camera position: " + std::to_string(camera->GetBounds().left) + " " + std::to_string(camera->GetBounds().top) + "\n"));

	//OutputDebugString(ConvertToLPCWSTR(std::to_string(gameTime) + "\n"));
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

		if (monsters.size() > 0)
		{
			for (auto& monster : monsters)
			{
				if (monster && monster->IsInitialized()) 
				{
					camera->Render(monster);
				}
			}
		}

		if (playerBullets.size() > 0)
		{
			for (auto& bullet : playerBullets)
			{
				if (bullet && bullet->IsInitialized())
				{
					camera->Render(bullet);
				}
			}
		}
	}

	if (font && debugMode) {
		message = ConvertToLPCWSTR("Player Top Left: " + std::to_string(player->GetPosition().x) + " " 
			+ std::to_string(player->GetPosition().y) + "\n" + "Player Bottom Right: " 
			+ std::to_string(player->GetPosition().x + player->GetSprite()->spriteWidth) + " " 
			+ std::to_string(player->GetPosition().y + player->GetSprite()->spriteHeight)
		    /*camera position and size*/
			+ "\n" + "Camera Top Left: " + std::to_string(camera->GetBounds().left) + " " + std::to_string(camera->GetBounds().top) 
			+ "\n" + "Camera Bottom Right: " + std::to_string(camera->GetBounds().right) + " " + std::to_string(camera->GetBounds().bottom)
			+ "\n" + "Player health: " + std::to_string(player->GetHealth())
			+ "\n" + "FPS: " + std::to_string(1 / gameTime)
		);
		font->DrawTextW(NULL, message, -1, &messageRect, DT_LEFT, D3DCOLOR_ARGB(255, 255, 255, 0));
	}

	m_pDevice3D->EndScene();
	m_pDevice3D->Present(0, 0, 0, 0);

}