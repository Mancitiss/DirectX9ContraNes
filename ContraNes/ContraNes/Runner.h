#pragma once
#include "Monster.h"

class Runner : public Monster
{
public:
	using Monster::Monster;
	
	virtual ~Runner();

	virtual bool Init(LPDIRECT3DDEVICE9 device, float frameDelay = 0, D3DXVECTOR3 internalScale = D3DXVECTOR3(1, 1, 1)) override;

	virtual void ApplyCollision(GameplayObject* const& object, float gameTime) override;

	virtual void ApplyCollision(Player* const& object, float gameTime) override;

protected:
	virtual void _defaultHandle(D3DXVECTOR3& movement, D3DXVECTOR3& direction) override;
};