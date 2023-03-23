#pragma once
#include "GameplayObject.h"

class Player : public GameplayObject
{
public:
	Player(float x, float y);
	Player(float x, float y, float rotation, float speed, float maxSpeed);
	Player(float x, float y, float z, float rotation, float speed, float maxSpeed);
	~Player();
	virtual void HandleInput(float gameTime) override;
	virtual void Update(float gameTime) override;
	void SetGravitationalAcceleration(float g);
	void SetJumpVelocity(float j);
protected:
	float gravitationalAcceleration;
	float jumpVelocity;
	JumpStatus jumpStatus;
};