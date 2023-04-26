#pragma once

#include "Player.h"

class Bullet : public GameplayObject 
{
public:
	Bullet(float x, float y, float width, float height, float rotation = 0, float speed = 0, float maxSpeed = 0);
	~Bullet();
	virtual void Init(PDIRECT3DDEVICE9 device, LPCTSTR file = L"resources/bullet.png", D3DCOLOR backColor = D3DCOLOR_ARGB(0, 0, 0, 0), D3DCOLOR displayColor = D3DCOLOR_ARGB(255, 255, 255, 255));
	virtual void Update(float gametime);
	void ApplyCollision(Player* const& object, float gameTime);
	RECT GetBounds() { return bounds; }
	void SetDamage(int damage) { this->damage = damage; }
	int GetDamage() { return damage; }
	float GetWidth() { return width; }
	float GetHeight() { return height; }
protected:
	D3DXVECTOR3 platformPosition;
	RECT bounds;
	float width, height;
	int damage = 1;
	GameSprite* prev;
	GameSprite* pDying;
};