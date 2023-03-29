#pragma once
#define _USE_MATH_DEFINES
#include<cmath>
#include"GameSprite.h"
#include<string>

enum class ObjectStatus {ACTIVE, DYING, DEAD};

class GameplayObject
{
public:
	GameplayObject(float x, float y, float rotation, float speed, float maxSpeed, D3DXVECTOR3 internalScale = D3DXVECTOR3(1, 1, 1));
	GameplayObject(float x, float y, float z, float rotation, float speed, float maxSpeed, D3DXVECTOR3 internalScale = D3DXVECTOR3(1, 1, 1));

	~GameplayObject();

	virtual void HandleInput(float gameTime);
	virtual bool Init(LPDIRECT3DDEVICE9 device, LPCTSTR file, int width, int height, float baseZRotation = 0, D3DCOLOR backColor = D3DCOLOR_ARGB(0, 0, 0, 0), D3DCOLOR displayColor = D3DCOLOR_ARGB(255, 255, 255, 255));
	virtual void Update(float dt);
	virtual void Draw(float dt);
	virtual void Draw(D3DXVECTOR3* position, D3DXVECTOR3* scaleFactors, float dt);

	ObjectStatus GetStatus() const;
	GameSprite* GetSprite() const;

	void SetSpeed(float speed);
	D3DXVECTOR3 GetPosition() const;

	bool IsInitialized() const;

	void SetJerkIncrementPerSecond3(float jerk);
protected:
	LPCTSTR Name;
	LPCTSTR Description;

	D3DXVECTOR3 internalScale;

	// position
	D3DXVECTOR3 position;
	D3DXVECTOR3 velocity;
	float rotation;

	ObjectStatus status;
	GameSprite* sprite;
	float maxSpeed;

	bool initialized;

	float currentVelocity;
	float currentAcceleration;
	float currentJerk;
	float jerkIncrementPerSecond3;
};