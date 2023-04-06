#include "Runner.h"

void Runner::_defaultHandle(D3DXVECTOR3& movement, D3DXVECTOR3& direction)
{
	switch (this->moveType) {
	case MoveType::NONE: 
		break;
	case MoveType::LEFT: 
		movement.x = -1;
		direction.x = -1;
		break;
	case MoveType::RIGHT:
		movement.x = 1;
		direction.x = 1;
		break;
	case MoveType::FOLLOW:
		if (this->GetPosition().x > this->target->GetPosition().x) {
			movement.x = -1;
			direction.x = -1;
		}
		else {
			movement.x = 1;
			direction.x = 1;
		}
		break;
	case MoveType::RANDOM:
		if (rand() % 2 == 0) {
			movement.x = -1;
			direction.x = -1;
		}
		else {
			movement.x = 1;
			direction.x = 1;
		}
	}
}

Runner::~Runner()
{
	this->target = nullptr;
}

bool Runner::Init(LPDIRECT3DDEVICE9 device, float frameDelay, D3DXVECTOR3 internalScale)
{
	status = ObjectStatus::ACTIVE;

	if (!this->pIdleRight)
	{
		this->pIdleRight = new GameSprite();
		if (!this->pIdleRight->Init(device, L"resources/enemy/runner/7.png", 24, 34, 0, internalScale))
		{
			return false;
		}
		this->pIdleRight->pDefault = this->pIdleRight;
		this->pIdleRight->pNext = this->pIdleRight;
	}

	if (!this->pIdleLeft)
	{
		this->pIdleLeft = new GameSprite();
		if (!this->pIdleLeft->Init(device, L"resources/player/L.png", 24, 34, 0, internalScale))
		{
			return false;
		}
		this->pIdleLeft->pDefault = this->pIdleLeft;
		this->pIdleLeft->pNext = this->pIdleLeft;
	}
}