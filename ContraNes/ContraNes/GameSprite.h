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
	bool Init(LPDIRECT3DDEVICE9 device, LPCTSTR rss, int width, int height, float baseZRotation = 0, D3DCOLOR backColor = D3DCOLOR_ARGB(0, 0, 0, 0), D3DCOLOR displayColor = D3DCOLOR_ARGB(255, 255, 255, 255));
	bool IsInitialized();
	void Draw(D3DXVECTOR3 *position);
	void Draw(D3DXVECTOR3 *position, float rotation);
	LPDIRECT3DTEXTURE9 GetTexture() const;
	D3DXVECTOR3 GetPosition() const;
	void SetDisplayColor(D3DCOLOR color);

	// attributes
	int spriteWidth;
	int spriteHeight;

	GameSprite* pNext;
	GameSprite* pDefault;

private:
	// attributes
	LPDIRECT3DTEXTURE9 tex;
	LPD3DXSPRITE sprite;

	D3DCOLOR backColor;
	D3DCOLOR displayColor;

	D3DXVECTOR3 position;

	bool initialized;
	float baseZRotation;


};