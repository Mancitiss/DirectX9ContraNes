#pragma once
#include "Character.h"

class Player : public Character
{
public:
	Player(float x, float y, D3DXVECTOR3 internalScale = D3DXVECTOR3(1, 1, 1));

	Player(float x, float y, float rotation, float speed, float maxSpeed, D3DXVECTOR3 internalScale = D3DXVECTOR3(1, 1, 1));

	Player(float x, float y, float z, float rotation, float speed, float maxSpeed, D3DXVECTOR3 internalScale = D3DXVECTOR3(1, 1, 1));

	virtual ~Player();

	virtual bool Init(LPDIRECT3DDEVICE9 device, float frameDelay = 0, D3DXVECTOR3 internalScale = D3DXVECTOR3(1, 1, 1));

	virtual void HandleInput(float gameTime) override;

	virtual void Update(float gameTime) override;

	D3DXVECTOR3 GetVelocity() const override;

	D3DXVECTOR3 GetDirection() const override;

	D3DXVECTOR3 GetMovementVector() const override;

	void SetPositionX(float x) override;

	void SetPositionY(float y) override;

	void SetPositionZ(float z) override;

	void SetVelocityX(float x) override;

	void SetVelocityY(float y) override;

	void SetVelocityZ(float z) override;

	void SetGravitationalAcceleration(float gravitationalAcceleration) override;

	void SetJumpVelocity(float jumpVelocity) override;

	void ResetJumpCount() override;

	int GetJumpCount() const override;

	int GetMaxJumpCount() const override;

	void SetBaseJumpVelocity(float bj) override;

	void SetJumpDown(bool b) override;

	bool GetJumpDown() const override;

protected:
	virtual void _defaultHandle(D3DXVECTOR3& movement, D3DXVECTOR3& direction);

	int health; // TODO: implement health system

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