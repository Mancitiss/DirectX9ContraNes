#pragma once
#include "GameplayObject.h"

enum class Waterlogged {
	YES,
	NO
};

enum class JumpStatus {
	JUMPING,
	IDLE
};

enum class Facing {
	LEFT,
	RIGHT,
	UP
};

class Character : public GameplayObject
{
public:
	Character(float x, float y) : GameplayObject(x, y, 0, 0, 0, 300.0f) {};
	Character(float x, float y, float rotation, float speed, float maxSpeed) : GameplayObject(x, y, rotation, speed, maxSpeed) {};
	Character(float x, float y, float z, float rotation, float speed, float maxSpeed) : GameplayObject(x, y, z, rotation, speed, maxSpeed) {};
	virtual ~Character() {};
	virtual D3DXVECTOR3 GetVelocity() const = 0;
	virtual void SetVelocityX(float x) = 0;
	virtual void SetVelocityY(float y) = 0;
	virtual void SetVelocityZ(float z) = 0;
	virtual void SetPositionX(float x) = 0;
	virtual void SetPositionY(float y) = 0;
	virtual void SetPositionZ(float z) = 0;

	virtual void SetGravitationalAcceleration(float g) = 0;
	virtual void SetJumpVelocity(float j) = 0;

	virtual JumpStatus GetJumpState() const = 0;
	virtual void SetJumpState(JumpStatus jumpStatus) = 0;

protected:
	JumpStatus jumpStatus;

	float gravitationalAcceleration;
	float jumpVelocity;
};