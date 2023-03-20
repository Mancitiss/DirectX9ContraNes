#include "GameSprite.h"
#include "d3dUtil.h"

GameSprite::GameSprite()
{
	// when color is set to white, the sprite will be drawn with its original color
	color = D3DCOLOR_ARGB(255, 255, 255, 255);

	// sprite is not initialized
	initialized = false;
}

GameSprite::~GameSprite()
{
	// release the texture
	if (tex != NULL)
		tex->Release();

	// release the sprite
	if (sprite != NULL)
		sprite->Release();
}

bool GameSprite::Init(LPDIRECT3DDEVICE9 device, LPCTSTR rss, int width, int height)
{
	// same functionality as D3DXCreateTextureFromFile EXCEPT it allows you to specify the width and height
	// this is useful for sprites that are not a power of 2

	if (!SUCCEEDED(D3DXCreateTextureFromFileEx(device, rss, width, height,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(0, 0, 0),
		NULL, NULL, &tex)))
	{
		MessageBox(NULL, L"There was an error loading the texture", L"Error", MB_OK);
		return false;
	}


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
	spriteWidth = width;
	spriteHeight = height;

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
		sprite->Begin(D3DXSPRITE_ALPHABLEND || D3DXSPRITE_OBJECTSPACE);

		// draw the sprite
		sprite->Draw(tex, NULL, NULL, position, color);

		// end the sprite
		sprite->End();
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