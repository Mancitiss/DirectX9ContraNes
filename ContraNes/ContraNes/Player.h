#pragma once
#include "Character.h"

class Player : public Character
{
public:
	Player(float x, float y);

	Player(float x, float y, float rotation, float speed, float maxSpeed);

	Player(float x, float y, float z, float rotation, float speed, float maxSpeed);

	~Player();

	virtual bool Init(LPDIRECT3DDEVICE9 device, float frameDelay = 0);

	virtual void HandleInput(float gameTime) override;

	virtual void Update(float gameTime) override;

	virtual D3DXVECTOR3 GetVelocity() const override;

	virtual D3DXVECTOR3 GetDirection() const override;

	virtual D3DXVECTOR3 GetMovementVector() const override;

	virtual void SetPositionX(float x) override;

	virtual void SetPositionY(float y) override;

	virtual void SetPositionZ(float z) override;

	virtual void SetVelocityX(float x) override;

	virtual void SetVelocityY(float y) override;

	virtual void SetVelocityZ(float z) override;

	virtual void SetGravitationalAcceleration(float gravitationalAcceleration) override;

	virtual void SetJumpVelocity(float jumpVelocity) override;

	virtual void ResetJumpCount() override;

	virtual int GetJumpCount() const override;

	virtual int GetMaxJumpCount() const override;

	virtual void SetBaseJumpVelocity(float bj) override;

protected:

	float frameTime;
	float frameDelay;

	Waterlogged waterlogged;
	bool lockXFacing, lockYFacing;
	D3DXVECTOR3 movementVector;
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

private:
	bool hasJumped = true;
};