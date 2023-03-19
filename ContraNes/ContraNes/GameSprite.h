#pragma once

#include<d3d9.h>
#include<d3dx9.h>
#include<string>


class GameSprite
{
public:
	GameSprite();
	~GameSprite();

	// methods
	bool Init(LPDIRECT3DDEVICE9 device, LPCTSTR rss, int width, int height);
	bool IsInitialized();
	void Draw(float dt, D3DXVECTOR3 position);
	LPDIRECT3DTEXTURE9 GetTexture() const;

	// attributes
	int spriteWidth;
	int spriteHeight;

private:
	// attributes
	LPDIRECT3DTEXTURE9 tex;
	LPD3DXSPRITE sprite;

	D3DCOLOR color;


	bool initialized;
};