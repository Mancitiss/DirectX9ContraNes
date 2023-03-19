

#include "App.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	App app(hInstance);
	if (!app.Init())
		return 1;

	return app.Run();
}
