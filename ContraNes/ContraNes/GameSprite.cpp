#include "GameSprite.h"
#include "d3dUtil.h"
#include <DxErr.h>
GameSprite::GameSprite()
{
	// when color is set to white, the sprite will be drawn with its original color
	// color = D3DCOLOR_ARGB(255, 255, 255, 255);

	// sprite is not initialized
	initialized = false;
}

GameSprite::~GameSprite()
{
	// release the texture
	//if (tex != nullptr)
	//	try {
	//	tex = nullptr;
	//}
	//catch (...) {}

	//// release the sprite
	//if (sprite != nullptr)
	//	try {
	//	sprite = nullptr;
	//} catch (...) {}

	if (pNext != nullptr) pNext = nullptr;
	if (pDefault != nullptr) pDefault = nullptr;

}

bool GameSprite::Init(LPDIRECT3DDEVICE9 device, LPCTSTR rss, int width, int height, float baseZRotation, D3DXVECTOR3 internalScale, D3DCOLOR backColor, D3DCOLOR displayColor)
{
	// same functionality as D3DXCreateTextureFromFile EXCEPT it allows you to specify the width and height
	// this is useful for sprites that are not a power of 2

	/*if (!SUCCEEDED(D3DXCreateTextureFromFileEx(device, rss, width, height,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, backColor, 
		NULL, NULL, &tex)))
	{
		MessageBox(NULL, L"There was an error loading the texture", L"Error", MB_OK);
		return false;
	}*/

	HRESULT hr = D3DXCreateTextureFromFileEx(device, rss, 
		static_cast<UINT>(width*internalScale.x), static_cast<UINT>(height*internalScale.y),
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, backColor,
		NULL, NULL, &tex);
	if (FAILED(hr))
	{
		LPCWSTR errorText = L"Error loading texture: ";
		wchar_t errorMsg[MAX_PATH];
		wcscpy_s(errorMsg, MAX_PATH, errorText);
		wcscat_s(errorMsg, MAX_PATH, DXGetErrorString(hr));
		wcscat_s(errorMsg, MAX_PATH, L" ");
		wcscat_s(errorMsg, MAX_PATH, rss);
		MessageBox(NULL, errorMsg, L"Error", MB_OK);
		return false;
	}

	this->backColor = backColor;
	this->displayColor = displayColor;


	// attempt to create the sprite
	if (!SUCCEEDED(D3DXCreateSprite(device, &sprite)))
	{
		std::string s = "There was an error creating the texture: ";
		LPCWSTR ws = ConvertToLPCWSTR(s);
		MessageBox(NULL, ws, NULL, NULL);
		delete[] ws;
		return false;
	}

	// set the sprite width and height
	spriteWidth = static_cast<int>( width * internalScale.x );
	spriteHeight = static_cast<int>( height * internalScale.y );
	this->baseZRotation = baseZRotation;

	// sprite is initialized
	initialized = true;

	return true;
}

bool GameSprite::IsInitialized()
{
	return initialized;
}

void GameSprite::Draw(D3DXVECTOR3* position)
{
	if (sprite && tex)
	{
		// begin the sprite
		sprite->Begin(D3DXSPRITE_ALPHABLEND);

		// draw the sprite
		sprite->Draw(tex, NULL, NULL, position, displayColor);

		// end the sprite
		sprite->End();
	}
}

void GameSprite::Draw(D3DXVECTOR3* position, D3DXVECTOR3* scaleFactors, float rotation)
{
	if (sprite && tex)
	{
		// rotation matrix
		D3DXMATRIX mat;
		D3DXVECTOR3 position2 = D3DXVECTOR3((position->x + spriteWidth / 2), (position->y + spriteHeight / 2), position->z);
		D3DXQUATERNION quat;
		// make quat
		D3DXQuaternionRotationYawPitchRoll(&quat, 0, 0, baseZRotation + rotation);
		D3DXMatrixAffineTransformation(&mat, 1.0f, &position2, &quat, NULL);

		// scale matrix
		D3DXMATRIX mat2;
		D3DXMatrixScaling(&mat2, scaleFactors->x, scaleFactors-> y, scaleFactors->z);

		// combine the matrices
		mat = mat * mat2;
		

		// set the rotation
		sprite->SetTransform(&mat);

		// begin the sprite
		sprite->Begin( D3DXSPRITE_DONOTSAVESTATE | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK | D3DXSPRITE_ALPHABLEND );

		// draw the sprite
		sprite->Draw(tex, NULL, NULL, position, displayColor);

		// end the sprite
		sprite->End();
		//OutputDebugString(ConvertToLPCWSTR("" + std::to_string(position->x) + ", " + std::to_string(position->y) + "," + std::to_string(position->z) + "\n"));

	}
}

LPDIRECT3DTEXTURE9 GameSprite::GetTexture() const
{
	return tex;
}

D3DXVECTOR3 GameSprite::GetPosition() const
{
	return position;
}

void GameSprite::SetDisplayColor(D3DCOLOR color)
{
	displayColor = color;
}