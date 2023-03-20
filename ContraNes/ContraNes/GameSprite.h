#pragma once

#include<d3d9.h>
#include<d3dx9.h>
#include<string>


class GameSprite
{
public:
	GameSprite();
	GameSprite(float x, float y);
	~GameSprite();

	// methods
	bool Init(LPDIRECT3DDEVICE9 device, LPCTSTR rss, int width, int height);
	bool IsInitialized();
	void Draw(D3DXVECTOR3 *position);
	LPDIRECT3DTEXTURE9 GetTexture() const;
	D3DXVECTOR3 GetPosition() const;

	// attributes
	int spriteWidth;
	int spriteHeight;

private:
	// attributes
	LPDIRECT3DTEXTURE9 tex;
	LPD3DXSPRITE sprite;

	D3DCOLOR color;

	D3DXVECTOR3 position;

	bool initialized;
};