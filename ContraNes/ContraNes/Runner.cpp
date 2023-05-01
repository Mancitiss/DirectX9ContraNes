#include "Runner.h"
#include "d3dUtil.h"

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

	if (this->pIdleRight) {
		delete this->pIdleRight;
		this->pIdleRight = nullptr;
	}

	if (this->pIdleLeft) {
		delete this->pIdleLeft;
		this->pIdleLeft = nullptr;
	}

	DeleteCircularList(this->pMoveRight);
	DeleteCircularList(this->pMoveLeft);

	this->sprite = nullptr;
}

bool Runner::Init(LPDIRECT3DDEVICE9 device, float frameDelay)
{
	status = ObjectStatus::ACTIVE;

	if (!this->pIdleRight)
	{
		this->pIdleRight = new GameSprite();
		if (!this->pIdleRight->Init(device, L"resources/enemy/runner/7.png", 21, 45, 0, internalScale))
		{
			return false;
		}
		this->pIdleRight->pDefault = this->pIdleRight;
		this->pIdleRight->pNext = this->pIdleRight;
	}

	if (!this->pIdleLeft)
	{
		this->pIdleLeft = new GameSprite();
		if (!this->pIdleLeft->Init(device, L"resources/enemy/runner/10.png", 21, 45, 0, internalScale))
		{
			return false;
		}
		this->pIdleLeft->pDefault = this->pIdleLeft;
		this->pIdleLeft->pNext = this->pIdleLeft;
	}

	if (!this->pMoveRight)
	{
		if (!CreateSprites(device, 9, "resources/enemy/runner/", ".png", this->pMoveRight, this->pIdleRight, internalScale, 9, "")) 
		{ 
			return false; 
		}
	}

	if (!this->pMoveLeft)
	{
		if (!CreateSprites(device, 9, "resources/enemy/runner/", ".png", this->pMoveLeft, this->pIdleLeft, internalScale, 0, ""))
		{
			return false;
		}
	}

	this->sprite = this->pIdleRight;

	this->frameDelay = frameDelay;
	this->frameTime = frameDelay;

	this->initialized = true;

	this->health = 1;
	this->damage = 1;

	return true;
}

void Runner::ApplyCollision(GameplayObject* const& object, float gameTime)
{
	// do nothing for now
}

void Runner::ApplyCollision(Player* const& object, float gameTime)
{
	object->TakeDamage(this->damage);
}