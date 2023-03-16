#include <Windows.h>
#include "ContraApp.h"
#include "d3dUtil.h"
#include <DirectXMath.h>

struct VertexPositionColor
{
	VertexPositionColor() {}
	VertexPositionColor(float x, float y, float z, D3DCOLOR color)
		: x(x), y(y), z(z), color(color) {}
	float x, y, z; //positions
	D3DCOLOR color; //color
	static const DWORD FVF;
};

const DWORD VertexPositionColor::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

class App : public ContraApp
{
public:
	// Constructor
	App(HINSTANCE hInstance);

	// Destructor
	~App();

	// methods

	bool Init() override;
	void Update(float dt) override;
	void Render() override;
};

IDirect3DVertexBuffer9* VB; //vertex buffer - list of vertices - the graphics card will use this to draw the triangles
IDirect3DIndexBuffer9* IB; //index buffer - list of indices - the graphics card will use this to know the index to draw the triangles

App::App(HINSTANCE hInstance) : ContraApp(hInstance)
{

}

App::~App()
{

}

bool App::Init()
{
	if (!ContraApp::Init())
		return false;

	VertexPositionColor verts[3] = 
	{
		VertexPositionColor(0.0f, 1.0f, 0.0f, d3dColors::Green),
		VertexPositionColor(1.0f, -1.0f, 0.0f, d3dColors::Blue),
		VertexPositionColor(-1.0f, -1.0f, 0.0f, d3dColors::Red)
	};	

	m_pDevice3D->CreateVertexBuffer(3 * sizeof(VertexPositionColor), 0, VertexPositionColor::FVF, D3DPOOL_MANAGED, &VB, NULL);

	VOID* pVerts;
	VB->Lock(0, 0, (void**) &pVerts, 0);
	memcpy(pVerts, verts, sizeof(verts));
	VB->Unlock();

	DirectX::FXMMATRIX view, proj;
	DirectX::XMVECTOR

	return true;
}

void App::Update(float dt)
{
	
}

void App::Render()
{
	m_pDevice3D->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , d3dColors::Blue, 1.0f, 0);
	m_pDevice3D->Present(0, 0, 0, 0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	App app(hInstance);
	if (!app.Init())
		return 1;

	return app.Run();
}
