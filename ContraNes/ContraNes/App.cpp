#define _USE_MATH_DEFINES
#include<cmath>
#include <d3dx9.h>
#include "Keyboard.h"
#include "App.h"
#include "d3dUtil.h"
#include "Runner.h"
#include "Shooter.h"
#include "Level1.h"

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

	platforms.clear();

	monsters.clear();
	playerBullets.clear();
	monsterBullets.clear();

	delete[] message;
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
	// this is not 60 fps 
	// but more like a largest time unit for update and render
	// anything larger than this will be split into smaller time units
	static constexpr float kTargetFrameTime = 1.0f / 60.0f; 

	static float sAccumulatedTime = 0.0f;
	static float renderTime = 0.0f;

	gameTime->Update();

	sAccumulatedTime += gameTime->elapsedGameTime;
	renderTime = sAccumulatedTime;
	while ( sAccumulatedTime >= kTargetFrameTime )
	{
		Update(kTargetFrameTime);
		//Render(kTargetFrameTime);

		sAccumulatedTime -= kTargetFrameTime;
	}
	Update(sAccumulatedTime);
	Render(renderTime);


	// max fps
	//static constexpr float kMaxFrameTime = 1.0f / 60.0f; // 60 FPS

	//gameTime->Update();

	//float deltaTime = gameTime->elapsedGameTime;
	//if (deltaTime < kMaxFrameTime)
	//{
	//	// Sleep the thread if the elapsed time is less than the maximum frame time
	//	const DWORD sleepTime = static_cast<DWORD>((kMaxFrameTime - deltaTime) * 1000.0f);
	//	Sleep(sleepTime);
	//}
	//if (deltaTime > 0.15f) deltaTime = 0.0f;
	//Update(deltaTime);
	//Render(deltaTime);


	// max fps without sleep
	//static constexpr float kMaxFrameTime = 1.0f / 60.0f; // max FPS

	//gameTime->Update();

	//const float deltaTime = gameTime->elapsedGameTime;
	//if (deltaTime < kMaxFrameTime)
	//{
	//	// Calculate the time to wait
	//	const DWORD waitTime = static_cast<DWORD>((kMaxFrameTime - deltaTime) * 1000.0f);

	//	// Create and set the timer
	//	HANDLE hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
	//	LARGE_INTEGER dueTime;
	//	dueTime.QuadPart = -static_cast<LONGLONG>(waitTime) * 10000LL;
	//	SetWaitableTimer(hTimer, &dueTime, 0, NULL, NULL, FALSE);

	//	// Wait for the timer to expire
	//	WaitForSingleObject(hTimer, INFINITE);
	//	CloseHandle(hTimer);
	//}
	//if (deltaTime > 0.25f) return; // pause the game when the game is stopped / or below 4 fps (0.25s)
	//Update(deltaTime);
	//Render(deltaTime);
	
}

ID3DXSprite* pSprite;

bool App::InitObjects()
{
	player = new Player(100, 5, 0, (float)0, 200, 200, D3DXVECTOR3(2, 2, 1));
	if (!player->Init(m_pDevice3D, 0.15f)) return false;
	player->SetJerkIncrementPerSecond3(19702.0f);
	player->SetBaseJumpVelocity(350);
	player->SetGravitationalAcceleration(600);
	player->SetInvincibilityDelay(1);
	player->SetHealth(100);

	currentSection = D3DXVECTOR3(player->GetPosition().x / camera->GetWidth(), player->GetPosition().y / camera->GetHeight(), 0);
	previousSection = currentSection + 1;

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

	levelCounter = 0;
	LoadLevel(++levelCounter);
	this->level->Init(m_pDevice3D, this->camera);
	this->player->SetPositionX(this->level->GetStartPosition().x);
	this->player->SetPositionY(this->level->GetStartPosition().y);
	this->player->SetPositionZ(this->level->GetStartPosition().z);
	this->level->GetSurroundingObjects(player->GetPosition(), this->background, this->platforms, this->monsters);

	// initialize game time
	gameTime = new GameTime();
	if (!gameTime->Initialize())
		return false;

	font = NULL;
	message = new wchar_t[1] {0};
	HRESULT hr = D3DXCreateFont(m_pDevice3D, 19, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &font);
	if (FAILED(hr)) 
	{
		MessageBox(NULL, L"Failed to create font", NULL, NULL);
		return false;
	}
	SetRect(&messageRect, 0, 0, 500, 200);

	return true;
}

void CheckCollision(Player* const& player, std::vector<StandableObject*> const& platforms, float gameTime)
{
	RECT playerRect = { (LONG)(player->prev_position.x), (LONG)(player->prev_position.y), (LONG)(player->GetPosition().x + player->GetSprite()->spriteWidth), (LONG)(player->GetPosition().y + player->GetSprite()->spriteHeight) };
	float playerS = (playerRect.right - playerRect.left) * (playerRect.bottom - playerRect.top);
	D3DXVECTOR3 position = player->GetPosition();
	bool first = true;
	for (auto& platform : platforms)
	{
		RECT platformRect = platform->GetBounds();
		if (CheckIntersection(&playerRect, &platformRect) && (player->ignore.find(platform) == player->ignore.end()))
		{
			/*LPCWSTR str = ConvertToLPCWSTR(std::to_string(gameTime));
			OutputDebugString(str);
			delete[] str;*/
			platform->ApplyCollision(player, gameTime);
			if (player->ignore.find(platform) != player->ignore.end()) continue;

			float S2 = ((player->GetPosition().x + player->GetSprite()->spriteWidth - player->prev_position.x) * (player->GetPosition().y + player->GetSprite()->spriteHeight - player->prev_position.y));
			if (std::abs(S2) < std::abs(playerS) || first)
			{
				playerRect = { (LONG)(player->prev_position.x), (LONG)(player->prev_position.y), (LONG)(player->GetPosition().x + player->GetSprite()->spriteWidth), (LONG)(player->GetPosition().y + player->GetSprite()->spriteHeight) };
				position = player->GetPosition();
				playerS = S2;
				first = false;
			}
			else
			{
				player->SetPositionX(position.x);
				player->SetPositionY(position.y);
			}
			bool condition;
			if (player->GetPosition().y > 149.0f) condition = true;
			else condition = false;
			/*LPCWSTR str2 = ConvertToLPCWSTR(std::to_string(condition));
			OutputDebugString(str2);
			delete[] str2;*/
		}
		else if (player->ignore.find(platform) != player->ignore.end())
		{
			player->ignore.erase(platform);
		}
	}
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

	currentSection = D3DXVECTOR3(player->GetPosition().x / camera->GetWidth(), player->GetPosition().y / camera->GetHeight(), 0);
	if (currentSection != previousSection) {
		previousSection = currentSection;
		this->level->GetSurroundingObjects(player->GetPosition(), this->background, this->platforms, this->monsters);
	}
	OutputDebugString(ConvertToLPCWSTR(std::to_string(this->platforms.size())));
	OutputDebugString(L"\n");
	OutputDebugString(ConvertToLPCWSTR(std::to_string(this->monsters.size())));
	OutputDebugString(L"\n");

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
			if (player->GetDirection().x == 0.0f && player->GetDirection().y >= 0.0f)
			{
				// facing
				if (player->GetFacing() == Facing::RIGHT)
				{
					// right
					playerBullets.emplace_back(std::make_unique<Bullet>(player->GetPosition().x + player->GetSprite()->spriteWidth, player->GetPosition().y + player->GetSprite()->spriteHeight / 4));
					playerBullets.back()->Init(m_pDevice3D);
				}
				else
				{
					// left
					playerBullets.emplace_back(std::make_unique<Bullet>(player->GetPosition().x, player->GetPosition().y + player->GetSprite()->spriteHeight / 4, D3DXToRadian(180) ));
					playerBullets.back()->Init(m_pDevice3D);
				}
			}
			else
			{
				// moving
				if (player->GetDirection().x > 0.0f)
				{
					if (player->GetDirection().y == 0.0f) 
					{
						// right
						playerBullets.emplace_back(std::make_unique<Bullet>(player->GetPosition().x + player->GetSprite()->spriteWidth, player->GetPosition().y + player->GetSprite()->spriteHeight / 4));
						playerBullets.back()->Init(m_pDevice3D);
					}
					else if (player->GetDirection().y > 0.0f) 
					{
						// bottom right
						playerBullets.emplace_back(std::make_unique<Bullet>(player->GetPosition().x + player->GetSprite()->spriteWidth, player->GetPosition().y + player->GetSprite()->spriteWidth * 3/4, D3DXToRadian(30)));
						playerBullets.back()->Init(m_pDevice3D);
					}
					else
					{
						// top right
						playerBullets.emplace_back(std::make_unique<Bullet>(player->GetPosition().x + player->GetSprite()->spriteWidth * 3 / 4, player->GetPosition().y, D3DXToRadian(-30)));
						playerBullets.back()->Init(m_pDevice3D);
					}
				}
				else if (player->GetDirection().x < 0.0f)
				{
					if (player->GetDirection().y == 0.0f)
					{
						// left
						playerBullets.emplace_back(std::make_unique<Bullet>(player->GetPosition().x, player->GetPosition().y + player->GetSprite()->spriteHeight / 4, D3DXToRadian(180)));
						playerBullets.back()->Init(m_pDevice3D);
					}
					else if (player->GetDirection().y > 0.0f)
					{
						// bottom left
						playerBullets.emplace_back(std::make_unique<Bullet>(player->GetPosition().x, player->GetPosition().y + player->GetSprite()->spriteWidth * 3 / 4, D3DXToRadian(150)));
						playerBullets.back()->Init(m_pDevice3D);
					}
					else
					{
						// top left
						playerBullets.emplace_back(std::make_unique<Bullet>(player->GetPosition().x + player->GetSprite()->spriteWidth * 1 / 4, player->GetPosition().y, D3DXToRadian(-150)));
						playerBullets.back()->Init(m_pDevice3D);
					}
				}
				else
				{
					if (player->GetDirection().y < 0.0f)
					{
						// top
						playerBullets.emplace_back(std::make_unique<Bullet>(player->GetPosition().x + player->GetSprite()->spriteWidth / 2, player->GetPosition().y, D3DXToRadian(-90)));
						playerBullets.back()->Init(m_pDevice3D);
					}
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

	CheckCollision(player, platforms, gameTime);
	RECT playerRect = { (LONG)(player->GetPosition().x), (LONG)(player->GetPosition().y), (LONG)(player->GetPosition().x + player->GetSprite()->spriteWidth), (LONG)(player->GetPosition().y + player->GetSprite()->spriteHeight)};
	D3DXVECTOR3 position;

	for (auto& monster : monsters)
	{
		if (!monster || !monster->IsInitialized() || monster->GetMoveType() == MoveType::NONE) continue;
		monster->HandleInput(gameTime);
		monster->Update(gameTime);
		CheckCollision(&*monster, platforms, gameTime);

		if (monster->fired)
		{
			monster->fired = false;
			monsterBullets.emplace_back(std::make_unique<Bullet>(monster->GetPosition().x + monster->GetSprite()->spriteWidth / 2, monster->GetPosition().y, monster->GetDirection().z));
			monsterBullets.back()->Init(m_pDevice3D);
		}
		this->level->Update(monster);
	}

	for (auto& bullet : playerBullets)
	{
		if (!bullet || !bullet->IsInitialized()) continue;
		bullet->Update(gameTime);
	}

	for (auto& bullet : monsterBullets)
	{
		if (!bullet || !bullet->IsInitialized()) continue;
		bullet->Update(gameTime);
	}

	if (camera) {

		if (GetAsyncKeyState(69)) // E
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
			if (monster && monster->IsInitialized() && monster->GetStatus() == ObjectStatus::ACTIVE)
			{
				// log monster position
				//OutputDebugString(ConvertToWString("M: " + std::to_string(monster->GetPosition().x) + " " + std::to_string(monster->GetPosition().y) + "").c_str());
				RECT monsterRect = { (LONG)monster->GetPosition().x, (LONG)monster->GetPosition().y, (LONG)(monster->GetPosition().x + monster->GetSprite()->spriteWidth), (LONG)(monster->GetPosition().y + monster->GetSprite()->spriteHeight) };
				if (CheckIntersection(&monsterRect, &cam) && monster->GetMoveType() == MoveType::NONE)
				{
					monster->SetMoveType(MoveType::LEFT);
					monster->target = player;
				}
				//else if (!CheckIntersection(&monsterRect, &cam) && monster->GetMoveType() != MoveType::NONE)
				else if ((monster->GetPosition().x + monster->GetSprite()->spriteWidth < cam.left - (cam.right - cam.left) || monster->GetPosition().y > cam.bottom) && monster->GetMoveType() != MoveType::NONE)
				{
					monster->SetStatus(ObjectStatus::DEAD);
					continue;
				}

				if (CheckIntersection(&playerRect, &monsterRect))
				{
					monster->ApplyCollision(player, gameTime);
				}
			}
		}
		monsters.remove_if([this](Monster*& monster) { 
			bool remove = monster->GetStatus() == ObjectStatus::DEAD; 
			if (!remove) return false;
			level->Remove(monster);
			return true;
		});

		for (auto& bullet : playerBullets)
		{
			if (!bullet || !bullet->IsInitialized()) continue;
			RECT bulletRect = bullet->GetBounds();
			if (!CheckIntersection(&bulletRect, &cam) || bullet->GetStatus() == ObjectStatus::DEAD )
			{
				// log
				//OutputDebugString(L"Bullet deleted\n");
				bullet->SetStatus(ObjectStatus::DEAD);
			}
			else
			{
				for (auto& monster : monsters)
				{
					if (!monster || !monster->IsInitialized()) continue;
					RECT monsterRect = { (LONG)monster->GetPosition().x, (LONG)monster->GetPosition().y, (LONG)(monster->GetPosition().x + monster->GetSprite()->spriteWidth), (LONG)(monster->GetPosition().y + monster->GetSprite()->spriteHeight) };
					if (!CheckIntersection(&bulletRect, &monsterRect)) continue;
					bullet->ApplyCollision(&*monster);
					break;
				}
			}
		}
		playerBullets.remove_if([](const std::unique_ptr<Bullet>& bullet) { return bullet->GetStatus() == ObjectStatus::DEAD; });

		for (auto& bullet : monsterBullets)
		{
			if (!bullet || !bullet->IsInitialized()) continue;
			RECT bulletRect = bullet->GetBounds();
			if (!CheckIntersection(&bulletRect, &cam) || bullet->GetStatus() == ObjectStatus::DEAD)
			{
				// log
				//OutputDebugString(L"Bullet deleted\n");
				bullet->SetStatus(ObjectStatus::DEAD);
			}
			else
			{
				RECT playerRect = { (LONG)player->GetPosition().x, (LONG)player->GetPosition().y, (LONG)(player->GetPosition().x + player->GetSprite()->spriteWidth), (LONG)(player->GetPosition().y + player->GetSprite()->spriteHeight) };
				if (!CheckIntersection(&bulletRect, &playerRect)) continue;
				bullet->ApplyCollision(player);
			}
		}
		monsterBullets.remove_if([](const std::unique_ptr<Bullet>& bullet) { return bullet->GetStatus() == ObjectStatus::DEAD; });
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
}

void App::Render(float gameTime)
{
	m_pDevice3D->Clear(0, 0,
		D3DCLEAR_TARGET ,
		D3DCOLOR_XRGB(0, 100, 100), 1.0f, 0);

	m_pDevice3D->BeginScene(); 

	if (camera) {
		RECT cam = camera->GetBounds();
		if (background && background->IsInitialized())
		{
			camera->Render2(background);
		}

		if (player && player->IsInitialized())
		{
			camera->Render2(player);
		}

		if (player2 && player2->IsInitialized())
		{
			camera->Render2(player2);
		}

		if (monsters.size() > 0)
		{
			for (auto& monster : monsters)
			{
				if (monster && monster->IsInitialized()) 
				{
					camera->Render2(&*monster);
				}
			}
		}

		if (playerBullets.size() > 0)
		{
			for (auto& bullet : playerBullets)
			{
				if (bullet && bullet->IsInitialized())
				{
					camera->Render2(&*bullet);
				}
			}
		}

		if (monsterBullets.size() > 0)
		{
			for (auto& bullet : monsterBullets)
			{
				if (bullet && bullet->IsInitialized())
				{
					camera->Render2(&*bullet);
				}
			}
		}
	}

	static constexpr float displayTimeDelay = 0.1f;
	static float displayTime = 0.0f;
	displayTime += gameTime;
	if (font && debugMode) {
		if (displayTime >= displayTimeDelay) {
			// at the start message is assigned with wchar_t[1]{0} so we can delete it
			// the assignment is done in App::Init() function
			delete[] message; 
			LPCWSTR s = ConvertToLPCWSTR(
				"Player Top Left: " + std::to_string(player->GetPosition().x) + " " 
				+ std::to_string(player->GetPosition().y) + "\n" + "Player Bottom Right: "
				+ std::to_string(player->GetPosition().x + player->GetSprite()->spriteWidth) + " "
				+ std::to_string(player->GetPosition().y + player->GetSprite()->spriteHeight)
				/*camera position and size*/
				+ "\n" + "Camera Top Left: " + std::to_string(camera->GetBounds().left) + " " + std::to_string(camera->GetBounds().top)
				+ "\n" + "Camera Bottom Right: " + std::to_string(camera->GetBounds().right) + " " + std::to_string(camera->GetBounds().bottom)
				+ "\n" + "Player health: " + std::to_string(player->GetHealth())
				+ "\n" + "FPS: " + std::to_string(1 / gameTime)
			);
			message = s;
			displayTime = 0.0f;
		}
		font->DrawTextW(NULL, message, -1, &messageRect, DT_LEFT, D3DCOLOR_ARGB(255, 255, 255, 0));
	}

	m_pDevice3D->EndScene();
	m_pDevice3D->Present(0, 0, 0, 0);

}

void App::LoadLevel(int levelCounter)
{
	switch (levelCounter)
	{
	case 1:
		this->level = new Level1();
	}
}
