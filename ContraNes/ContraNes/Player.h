#pragma once
#include "Character.h"

class Player : public Character
{
public:
	Player(float x, float y, D3DXVECTOR3 internalScale = D3DXVECTOR3(1, 1, 1));

	Player(float x, float y, float rotation, float speed, float maxSpeed, D3DXVECTOR3 internalScale = D3DXVECTOR3(1, 1, 1));

	Player(float x, float y, float z, float rotation, float speed, float maxSpeed, D3DXVECTOR3 internalScale = D3DXVECTOR3(1, 1, 1));

	virtual ~Player();

	virtual bool Init(LPDIRECT3DDEVICE9 device, float frameDelay = 0);

	virtual void HandleInput(float gameTime) override;

	virtual void Update(float gameTime) override;

	D3DXVECTOR3 GetVelocity() const override;

	D3DXVECTOR3 GetDirection() const override; // aim direction

	D3DXVECTOR3 GetMovementVector() const override; // movement direction

	virtual GameSprite* GetPrev() const override;

	// position
	void SetPositionX(float x) override;

	void SetPositionY(float y) override;

	void SetPositionZ(float z) override;

	// velocity
	void SetVelocityX(float x) override;

	void SetVelocityY(float y) override;

	void SetVelocityZ(float z) override;

	// jump related functions
	void SetGravitationalAcceleration(float gravitationalAcceleration) override;

	void SetJumpVelocity(float jumpVelocity) override;

	void ResetJumpCount() override;

	int GetJumpCount() const override;

	int GetMaxJumpCount() const override;

	void SetBaseJumpVelocity(float bj) override;

	void SetJumpDown(bool b) override;

	bool GetJumpDown() const override;

	// damage related functions
	virtual void TakeDamage(int damage);

	int GetHealth() const;
	
	int GetDamage() const;

	void SetHealth(int health);

	void SetDamage(int damage);

	void SetInvincibilityTime(float invincibilityTime);

	void SetInvincibilityDelay(float invincibilityDelay);

	float GetInvincibilityTime() const;

	float GetInvincibilityDelay() const;

	bool respawn = false;
	bool fired = false;

	Facing GetFacing() { return this->facing; }

protected:
	virtual void _defaultHandle(D3DXVECTOR3& movement, D3DXVECTOR3& direction);
	virtual void _HandleDirection(D3DXVECTOR3& direction);

	int health = 3;
	int damage = 1;

	float invincibilityTime = 0.0f;
	float invincibilityDelay = 0.5f;

	float frameTime;
	float frameDelay;

	float reloadTime = 0.0f;
	float reloadDelay = 0.5f;

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

	GameSprite* pDieLeft;
	GameSprite* pDieRight;

	/*GameSprite* pShootWR;
	GameSprite* pShootWL;
	GameSprite* pShootR;
	GameSprite* pShootL;*/

private:
	bool hasJumped = true;
};