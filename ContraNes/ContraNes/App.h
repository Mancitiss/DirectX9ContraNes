#pragma once
#include "ContraApp.h"
#include "GameplayObject.h"
#include "GameTime.h"
#include "Camera.h"
#include "Player.h"
#include "StandableObject.h"
#include <vector>
#include "Monster.h"
#include "Bullet.h"
#include <list>
#include <memory>
#include "Level.h"
#include "d3dUtil.h"

class App : public ContraApp
{
public:

	// Constructor
	App(HINSTANCE hInstance);

	// Destructor
	~App();

	// methods
	int Run() override;
	void _Run() override;
	bool Init() override;
	void Update(float dt) override;
	void Render(float gameTime) override;

	void LoadLevel(int level);

	// initilize game objects
	bool InitObjects();

private:
	Camera* camera;

	GameplayObject* background, * player2;
	Player* player;

	std::unordered_set<StandableObject*> platforms;
	std::list<Monster*> monsters;
	std::list<std::unique_ptr<Bullet>> playerBullets;
	std::list<std::unique_ptr<Bullet>> monsterBullets;

	GameTime* gameTime;
	ID3DXFont* font;
	LPCWSTR message;
	RECT messageRect;

	int levelCounter;
	Level* level;
	IntVector3 currentSection;
	IntVector3 previousSection;

	int width;
	int height;

	bool pause = false;
	bool debugMode = false;
};