#include "Shooter.h"
#include "d3dUtil.h"

Shooter::~Shooter()
{
	this->sprite = nullptr;
	this->prev = nullptr;
}

bool Shooter::Init(LPDIRECT3DDEVICE9 device, float frameDelay)
{
	this->status = ObjectStatus::ACTIVE;

	if (!this->pIdleLeft)
	{
		if (!CreateSprites(device, 3, "resources/enemy/shooter/", ".png", this->pIdleLeft, this->pIdleLeft, internalScale, 0, ""))
		{
			return false;
		}
	}

	if (!this->pIdleUpLeft)
	{
		if (!CreateSprites(device, 3, "resources/enemy/shooter/", ".png", this->pIdleUpLeft, this->pIdleLeft, internalScale, 3, ""))
		{
			return false;
		}
	}

	if (!this->pJumpLeft)
	{
		if (!CreateSprites(device, 3, "resources/enemy/shooter/", ".png", this->pJumpLeft, this->pJumpLeft, internalScale, 6, ""))
		{
			return false;
		}
	}

	if (!this->pJumpRight)
	{
		if (!CreateSprites(device, 3, "resources/enemy/shooter/", ".png", this->pJumpRight, this->pJumpRight, internalScale, 9, ""))
		{
			return false;
		}
	}

	if (!this->pIdleUpRight)
	{
		if (!CreateSprites(device, 3, "resources/enemy/shooter/", ".png", this->pIdleUpRight, this->pIdleUpRight, internalScale, 12, ""))
		{
			return false;
		}
	}

	if (!this->pIdleRight)
	{
		if (!CreateSprites(device, 3, "resources/enemy/shooter/", ".png", this->pIdleRight, this->pIdleRight, internalScale, 15, ""))
		{
			return false;
		}
	}

	if (!this->pMoveLeftDown)
	{
		if (!CreateSprites(device, 3, "resources/enemy/shooter/", ".png", this->pMoveLeftDown, this->pIdleLeft, internalScale, 18, ""))
		{
			return false;
		}
	}

	if (!this->pIdleDownLeft)
	{
		if (!CreateSprites(device, 3, "resources/enemy/shooter/", ".png", this->pIdleDownLeft, this->pIdleLeft, internalScale, 21, ""))
		{
			return false;
		}
	}

	if (!this->pIdleDownRight)
	{
		if (!CreateSprites(device, 3, "resources/enemy/shooter/", ".png", this->pIdleDownRight, this->pIdleDownRight, internalScale, 24, ""))
		{
			return false;
		}
	}

	if (!this->pMoveRightDown)
	{
		if (!CreateSprites(device, 3, "resources/enemy/shooter/", ".png", this->pMoveRightDown, this->pMoveRightDown, internalScale, 27, ""))
		{
			return false;
		}
	}

	this->moveType = MoveType::NONE;
	this->target = nullptr;
	this->reloadTime = this->reloadDelay;
	this->sprite = this->pIdleLeft;
	this->initialized = true;
	return true;
}

void Shooter::ApplyCollision(GameplayObject* const& object, float gameTime)
{
	// do nothing for now
}

void Shooter::ApplyCollision(Player* const& object, float gameTime)
{
	if (object->GetStatus() == ObjectStatus::ACTIVE)
	{
		object->TakeDamage(this->damage);
	}
}

void Shooter::_defaultHandle(D3DXVECTOR3& movement, D3DXVECTOR3& direction)
{
	if (this->status != ObjectStatus::ACTIVE || this->target == nullptr ) return;

	float target_x, target_y;
	target_x = this->target->GetPosition().x + this->target->GetSprite()->spriteWidth / 2;
	target_y = this->target->GetPosition().y + this->target->GetSprite()->spriteHeight / 2;

	float x, y;
	x = this->GetPosition().x + this->GetSprite()->spriteWidth / 2;
	y = this->GetPosition().y + this->GetSprite()->spriteHeight / 2;

	float angle = atan2(target_y - y, target_x - x);

	if (angle >= -D3DX_PI / 8 && angle <= D3DX_PI / 8)
	{
		direction.x = 1;
		direction.y = 0;
	}
	else if (angle >= D3DX_PI / 8 && angle <= 3 * D3DX_PI / 8)
	{
		direction.x = 1;
		direction.y = 1;
	}
	else if (angle >= 3 * D3DX_PI / 8 && angle <= 5 * D3DX_PI / 8)
	{
		direction.x = 0;
		direction.y = 1;
	}
	else if (angle >= 5 * D3DX_PI / 8 && angle <= 7 * D3DX_PI / 8)
	{
		direction.x = -1;
		direction.y = 1;
	}
	else if (angle >= 7 * D3DX_PI / 8 || angle <= -7 * D3DX_PI / 8)
	{
		direction.x = -1;
		direction.y = 0;
	}
	else if (angle >= -7 * D3DX_PI / 8 && angle <= -5 * D3DX_PI / 8)
	{
		direction.x = -1;
		direction.y = -1;
	}
	else if (angle >= -5 * D3DX_PI / 8 && angle <= -3 * D3DX_PI / 8)
	{
		direction.x = 0;
		direction.y = -1;
	}
	else if (angle >= -3 * D3DX_PI / 8 && angle <= -D3DX_PI / 8)
	{
		direction.x = 1;
		direction.y = -1;
	}
	direction.z = angle;
	if (this->reloadTime <= 0)
	{
		this->fired = true;
		this->reloadTime = this->reloadDelay;
	}

}

void Shooter::_HandleDirection(D3DXVECTOR3& directionVector)
{
	//OutputDebugString(L"monster exist\n");
	if (currentDirection != directionVector)
	{
		if (directionVector.x < 0)
		{
			this->facing = Facing::LEFT;
			if (directionVector.y < 0)
			{
				this->sprite = this->pIdleUpLeft;
			}
			else if (directionVector.y > 0)
			{
				this->sprite = this->pIdleDownLeft;
			}
			else
			{
				this->sprite = this->pIdleLeft;
			}
		}
		else if (directionVector.x > 0)
		{
			this->facing = Facing::RIGHT;
			if (directionVector.y < 0)
			{
				this->sprite = this->pIdleUpRight;
			}
			else if (directionVector.y > 0)
			{
				this->sprite = this->pIdleDownRight;
			}
			else
			{
				this->sprite = this->pIdleRight;
			}
		}
		else
		{
			if (directionVector.y < 0)
			{
				if (this->facing == Facing::LEFT)
				{
					this->sprite = this->pJumpLeft;
				}
				else
				{
					this->sprite = this->pJumpRight;
				}
			}
			else if (directionVector.y > 0)
			{
				if (this->facing == Facing::LEFT)
				{
					this->sprite = this->pMoveLeftDown;
				}
				else
				{
					this->sprite = this->pMoveRightDown;
				}
			}
			else
			{
				this->sprite = this->sprite->pNext;
			}
		}

		this->currentDirection = directionVector;
	}
	else
	{
		this->sprite = this->sprite->pNext;
	}
}
