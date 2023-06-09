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
	bool Init(LPDIRECT3DDEVICE9 device, LPCTSTR rss, int width, int height, float baseZRotation = 0, D3DXVECTOR3 internalScale = D3DXVECTOR3(1, 1, 1), D3DCOLOR backColor = D3DCOLOR_ARGB(0, 0, 0, 0), D3DCOLOR displayColor = D3DCOLOR_ARGB(255, 255, 255, 255));
	bool IsInitialized();
	void Draw(D3DXVECTOR3 *position);
	void Draw(D3DXVECTOR3 *position, D3DXVECTOR3* scaleFactors, float rotation);
	void Draw(RECT const& src, D3DXVECTOR3 *position, D3DXVECTOR3* drawPosition, D3DXVECTOR3* scaleFactors, float rotation);
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