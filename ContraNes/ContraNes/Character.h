#pragma once
#include "GameplayObject.h"
#include <unordered_set>

enum class Waterlogged {
	YES,
	NO
};

enum class Facing {
	LEFT,
	RIGHT,
	UP
};

class Character : public GameplayObject
{
public:
	Character(float x, float y, D3DXVECTOR3 internalScale = D3DXVECTOR3(1, 1, 1)) : GameplayObject(x, y, 0, 0, 0, 300.0f, internalScale) {};
	Character(float x, float y, float rotation, float speed, float maxSpeed, D3DXVECTOR3 internalScale = D3DXVECTOR3(1, 1, 1)) : GameplayObject(x, y, rotation, speed, maxSpeed, internalScale) {};
	Character(float x, float y, float z, float rotation, float speed, float maxSpeed, D3DXVECTOR3 internalScale = D3DXVECTOR3(1, 1, 1)) : GameplayObject(x, y, z, rotation, speed, maxSpeed, internalScale) {};
	virtual ~Character() {};
	virtual GameSprite* GetPrev() const = 0;
	virtual D3DXVECTOR3 GetVelocity() const = 0;
	virtual D3DXVECTOR3 GetDirection() const = 0;
	virtual D3DXVECTOR3 GetMovementVector() const = 0;
	virtual void SetVelocityX(float x) = 0;
	virtual void SetVelocityY(float y) = 0;
	virtual void SetVelocityZ(float z) = 0;
	virtual void SetPositionX(float x) = 0;
	virtual void SetPositionY(float y) = 0;
	virtual void SetPositionZ(float z) = 0;

	virtual void SetGravitationalAcceleration(float g) = 0;
	virtual void SetJumpVelocity(float j) = 0;

	virtual void ResetJumpCount() = 0;

	virtual int GetJumpCount() const = 0;
	virtual int GetMaxJumpCount() const = 0;

	virtual void SetBaseJumpVelocity(float bj) = 0;

	virtual void SetJumpDown(bool b) = 0;
	virtual bool GetJumpDown() const = 0;

	std::unordered_set<GameplayObject*> ignore;
protected:
	int jumpCount;
	int maxJump;
	bool jumpDown; // indicate if the jump down action is being performed (not up)

	float gravitationalAcceleration;
	float jumpVelocity;
	float baseJumpVelocity;
};