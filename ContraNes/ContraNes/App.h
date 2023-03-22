#pragma once
#include "ContraApp.h"
#include "GameplayObject.h"
#include "GameTime.h"
#include "Camera.h"

class App : public ContraApp
{
public:

	// Constructor
	App(HINSTANCE hInstance);

	// Destructor
	~App();

	// methods

	void _Run() override;
	bool Init() override;
	void Update(float dt) override;
	void Render(float gameTime) override;

	// initilize game objects
	bool InitObjects();

private:
	Camera* camera;

	GameplayObject* background;
	GameplayObject* player, *player2;

	GameTime* gameTime;
	ID3DXFont* font;
	LPCWSTR message;
	RECT messageRect;

	int width;
	int height;
};