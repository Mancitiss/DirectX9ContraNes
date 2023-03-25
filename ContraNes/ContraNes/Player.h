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

class Player : public GameplayObject
{
public:
	Player(float x, float y);
	Player(float x, float y, float rotation, float speed, float maxSpeed);
	Player(float x, float y, float z, float rotation, float speed, float maxSpeed);
	~Player();
	virtual bool Init(LPDIRECT3DDEVICE9 device, float frameDelay = 0);
	virtual void HandleInput(float gameTime) override;
	virtual void Update(float gameTime) override;
	void SetGravitationalAcceleration(float g);
	void SetJumpVelocity(float j);
protected:
	float gravitationalAcceleration;
	float jumpVelocity;
	JumpStatus jumpStatus;

	float frameTime;
	float frameDelay;

	Waterlogged waterlogged;
	bool lockXFacing, lockYFacing;
	D3DXVECTOR3 currentDirection;
	Facing facing;

	GameSprite* prev;

	GameSprite* pMoveRight;
	GameSprite* pMoveLeft;
	GameSprite* pMoveUp;
	GameSprite* pMoveRightUp;
	GameSprite* pMoveRightDown;
	GameSprite* pMoveLeftUp;
	GameSprite* pMoveLeftDown;

	GameSprite* pJumpRight;
	GameSprite* pJumpLeft;
	GameSprite* pJumpUp; // unused

	GameSprite* pIdleRight;
	GameSprite* pIdleLeft;
	GameSprite* pIdleUpLeft;
	GameSprite* pIdleUpRight;
	GameSprite* pIdleUp;
	GameSprite* pIdleDownLeft;
	GameSprite* pIdleDownRight;

	/*GameSprite* pShootWR;
	GameSprite* pShootWL;
	GameSprite* pShootR;
	GameSprite* pShootL;*/
};