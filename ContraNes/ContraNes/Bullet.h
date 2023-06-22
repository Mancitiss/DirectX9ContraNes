#pragma once

#include "Player.h"

class Bullet : public GameplayObject 
{
public:
	Bullet(float x, float y, float rotation = 0, float speed = 400, float maxSpeed = 400, float width = 15, float height = 15, D3DXVECTOR3 internalScale = {1, 1, 1});
	~Bullet();
	virtual bool Init(PDIRECT3DDEVICE9 device,float frameDelay = 0.1f, LPCTSTR file = L"resources/bullet.png", D3DCOLOR backColor = D3DCOLOR_ARGB(0, 0, 0, 0), D3DCOLOR displayColor = D3DCOLOR_ARGB(255, 255, 255, 255));
	virtual void Update(float gametime);
	void ApplyCollision(Player* const& object);
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
	
	float frameTime = 0.0f, frameDelay = 0.2f;

	static GameSprite* prev;
	static GameSprite* pNormal;
	static GameSprite* pDying;
};