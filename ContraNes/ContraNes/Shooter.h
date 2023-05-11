#pragma once
#include "Monster.h"

class Shooter : public Monster
{
public:
	using Monster::Monster;

	virtual ~Shooter();

	virtual bool Init(LPDIRECT3DDEVICE9 device, float frameDelay = 0) override;

	virtual void ApplyCollision(GameplayObject* const& object, float gameTime) override;

	virtual void ApplyCollision(Player* const& object, float gameTime) override;

protected:
	virtual void _defaultHandle(D3DXVECTOR3& movement, D3DXVECTOR3& direction) override;

	virtual void _HandleDirection(D3DXVECTOR3& direction) override;
};
