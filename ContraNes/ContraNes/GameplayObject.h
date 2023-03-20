#pragma once
#define _USE_MATH_DEFINES
#include<cmath>
#include"GameSprite.h"
#include<string>

enum class ObjectStatus {Active, Dying, Dead};

class GameplayObject
{
public:
	GameplayObject(float x, float y, float rotation, float speed, float maxSpeed);
	GameplayObject(float x, float y, float z, float rotation, float speed, float maxSpeed);

	~GameplayObject();

	virtual void HandleInput();
	virtual bool Init(LPDIRECT3DDEVICE9 device, LPCTSTR file, int width, int height);
	virtual void Update(float dt);
	virtual void Draw(float dt);

	ObjectStatus GetStatus() const;
	GameSprite* GetSprite() const;

	void SetSpeed(float speed);
	D3DXVECTOR3 GetPosition() const;

	bool IsInitialized() const;

protected:
	LPCTSTR Name;
	LPCTSTR Description;

	// position
	D3DXVECTOR3 position;
	D3DXVECTOR3 velocity;
	float rotation;
	float speed;

	ObjectStatus status;
	GameSprite* sprite;
	float maxSpeed;

	bool initialized;
};