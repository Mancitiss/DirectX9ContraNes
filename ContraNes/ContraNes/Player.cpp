#include "Player.h"
#include "Keyboard.h"
#include "d3dUtil.h"
#include "GameSprite.h"
#include "d3dUtil.h"

Player::Player(float x, float y, D3DXVECTOR3 internalScale) : Character(x, y, 0, 0, 0, 300.0f, internalScale)
{
	gravitationalAcceleration = 2000;
	jumpVelocity = 0;
	facing = Facing::RIGHT;
	lockYFacing = true;
	maxJump = 1;
	jumpCount = 0;
}

Player::Player(float x, float y, float rotation, float speed, float maxSpeed, D3DXVECTOR3 internalScale) : Character(x, y, rotation, speed, maxSpeed, internalScale)
{
	gravitationalAcceleration = 2000;
	jumpVelocity = 0;
	facing = Facing::RIGHT;
	lockYFacing = true;
	maxJump = 1;
	jumpCount = 0;
}

Player::Player(float x, float y, float z, float rotation, float speed, float maxSpeed, D3DXVECTOR3 internalScale) : Character(x, y, z, rotation, speed, maxSpeed, internalScale)
{
	gravitationalAcceleration = 2000;
	jumpVelocity = 0;
	facing = Facing::RIGHT;
	lockYFacing = true;
	maxJump = 1;
	jumpCount = 0;
}

Player::~Player()
{
	DeleteCircularList(pMoveRight);
	DeleteCircularList(pMoveLeft);
	DeleteCircularList(pMoveUp);
	DeleteCircularList(pMoveRightUp);
	DeleteCircularList(pMoveRightDown);
	DeleteCircularList(pMoveLeftUp);
	DeleteCircularList(pMoveLeftDown);
	DeleteCircularList(pJumpRight);
	DeleteCircularList(pJumpLeft);
	DeleteCircularList(pJumpUp);
	DeleteCircularList(pIdleRight);
	DeleteCircularList(pIdleLeft);
	DeleteCircularList(pIdleUp);
	DeleteCircularList(pIdleDownLeft);
	DeleteCircularList(pIdleDownRight);
	DeleteCircularList(pIdleUpLeft);
	DeleteCircularList(pIdleUpRight);

	this->sprite = nullptr;
}

bool Player::Init(LPDIRECT3DDEVICE9 device, float frameDelay, D3DXVECTOR3 internalScale)
{
	status = ObjectStatus::ACTIVE;

	if (!this->pIdleRight) 
	{
		this->pIdleRight = new GameSprite();
		if (!this->pIdleRight->Init(device, L"resources/player/R.png", 24, 34, 0, internalScale))
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

	if (!this->pIdleUpLeft) 
	{
		this->pIdleUpLeft = new GameSprite();
		if (!this->pIdleUpLeft->Init(device, L"resources/player/W-L.png", 14, 46, 0, internalScale))
		{
			return false;
		}
		this->pIdleUpLeft->pDefault = this->pIdleLeft;
		this->pIdleUpLeft->pNext = this->pIdleUpLeft;
	}

	if (!this->pIdleUpRight)
	{
		this->pIdleUpRight = new GameSprite();
		if (!this->pIdleUpRight->Init(device, L"resources/player/W-R.png", 14, 46, 0, internalScale))
		{
			return false;
		}
		this->pIdleUpRight->pDefault = this->pIdleRight;
		this->pIdleUpRight->pNext = this->pIdleUpRight;
	}

	if (!this->pIdleDownLeft)
	{
		this->pIdleDownLeft = new GameSprite();
		if (!this->pIdleDownLeft->Init(device, L"resources/player/S-L.png", 34, 17, 0, internalScale))
		{
			return false;
		}
		this->pIdleDownLeft->pDefault = this->pIdleLeft;
		this->pIdleDownLeft->pNext = this->pIdleDownLeft;
	}

	if (!this->pIdleDownRight)
	{
		this->pIdleDownRight = new GameSprite();
		if (!this->pIdleDownRight->Init(device, L"resources/player/S-R.png", 34, 17, 0, internalScale))
		{
			return false;
		}
		this->pIdleDownRight->pDefault = this->pIdleRight;
		this->pIdleDownRight->pNext = this->pIdleDownRight;
	}

	if (!this->pIdleUp)
	{
		this->pIdleUp = new GameSprite();
		if (!this->pIdleUp->Init(device, L"resources/player/40.png", 19, 45, 0, internalScale))
		{
			return false;
		}
		this->pIdleUp->pDefault = this->pIdleUp;
		this->pIdleUp->pNext = this->pIdleUp;
	}

	if (!this->pMoveRight)
	{
		if (!CreateSprites(device, 5, "resources/player/R", ".png", this->pMoveRight, this->pIdleRight, internalScale))
		{
			return false;
		}
	}

	if (!this->pMoveLeft)
	{
		if (!CreateSprites(device, 5, "resources/player/L", ".png", this->pMoveLeft, this->pIdleLeft, internalScale))
		{
			return false;
		}
	}

	if (!this->pMoveUp)
	{
		if (!CreateSprites(device, 2, "resources/player/W", ".png", this->pMoveUp, this->pIdleUp, internalScale))
		{
			return false;
		
		}
	}

	if (!this->pJumpRight)
	{
		if (!CreateSprites(device, 4, "resources/player/JR", ".png", this->pJumpRight, this->pIdleRight, internalScale))
		{
			return false;
		}
	}

	if (!this->pJumpLeft)
	{
		if (!CreateSprites(device, 4, "resources/player/JL", ".png", this->pJumpLeft, this->pIdleLeft, internalScale))
		{
			return false;
		}
	}

	if (!this->pMoveRightUp)
	{
		if (!CreateSprites(device, 3, "resources/player/WR", ".png", this->pMoveRightUp, this->pIdleRight, internalScale))
		{
			return false;
		}
	}

	if (!this->pMoveLeftUp)
	{
		if (!CreateSprites(device, 3, "resources/player/WL", ".png", this->pMoveLeftUp, this->pIdleLeft, internalScale))
		{
			return false;
		}
	}

	if (!this->pMoveRightDown)
	{
		if (!CreateSprites(device, 3, "resources/player/SR", ".png", this->pMoveRightDown, this->pIdleRight, internalScale))
		{
			return false;
		}
	}

	if (!this->pMoveLeftDown)
	{
		if (!CreateSprites(device, 3, "resources/player/SL", ".png", this->pMoveLeftDown, this->pIdleLeft, internalScale))
		{
			return false;
		}
	}
	this->sprite = this->pIdleRight;

	this->frameDelay = frameDelay;
	this->frameTime = frameDelay;

	this->initialized = true;
	return true;
}

void Player::_defaultHandle(D3DXVECTOR3& movement, D3DXVECTOR3& direction)
{
	// if the down arrow and space are pressed at the same time, move the sprite down
	if (GetAsyncKeyState(Keyboard::DOWN) & 0x8000 && GetAsyncKeyState(Keyboard::JUMP) & 0x8000)
	{
		if (!hasJumped && jumpCount > 0)
		{
			hasJumped = true;

			jumpCount -= 1;
			jumpVelocity = 0;
			this->jumpDown = true;
		}
		movement.y += 1;
	}

	if (SHORT s = GetAsyncKeyState(Keyboard::JUMP)) // equal sign on purpose, not a typo
	{
		if (!hasJumped && jumpCount > 0 && ((s & 0x8001) == 0x8001))
		{
			hasJumped = true;
			jumpCount -= 1;
			jumpVelocity = baseJumpVelocity;
		}
		movement.y -= 1;
	}
	else hasJumped = false;

	// if the up arrow is pressed, move the sprite up
	if (GetAsyncKeyState(Keyboard::UP) & 0x8000) {
		direction.y -= 1;
	}

	if (GetAsyncKeyState(Keyboard::DOWN) & 0x8000) {
		direction.y += 1;
	}

	// if the left arrow is pressed, move the sprite left
	if (GetAsyncKeyState(Keyboard::LEFT) & 0x8000)
	{
		movement.x -= 1;
		direction.x -= 1;
	}

	// if the right arrow is pressed, move the sprite right
	if (GetAsyncKeyState(Keyboard::RIGHT) & 0x8000)
	{
		movement.x += 1;
		direction.x += 1;
	}

	// if the F key is pressed, fire a bullet
	if (GetAsyncKeyState(Keyboard::SHOOT) & 0x8000)
	{
	}
}

void Player::HandleInput(float gameTime)
{
	D3DXVECTOR3 movementVector(0, 0, 0);
	D3DXVECTOR3 directionVector(0, 0, 0);
	
	_defaultHandle(movementVector, directionVector);

	this->movementVector = movementVector;

	/*if (movementVector.x != 0 || movementVector.y != 0)
	{
		rotation = atan2(movementVector.y, movementVector.x);
	}*/
	currentJerk = min(currentJerk + jerkIncrementPerSecond3 * gameTime, maxSpeed / (gameTime * gameTime));
	currentAcceleration = min(currentAcceleration + currentJerk * gameTime, maxSpeed / gameTime);
	currentVelocity = min(currentVelocity + currentAcceleration * gameTime, maxSpeed);
	velocity.x = movementVector.x * currentVelocity;
	//if (jumpCount < maxJump) {
		if (movementVector.y >= 0 && jumpVelocity > 0) jumpVelocity = - gravitationalAcceleration * gameTime;
		velocity.y = - jumpVelocity;
		jumpVelocity -= gravitationalAcceleration * gameTime;
	//}
	//OutputDebugString(ConvertToLPCWSTR(std::to_string(gravitationalAcceleration) + " " + std::to_string(gameTime) + " " + std::to_string(jumpVelocity) + ", " + std::to_string(velocity.y) + ", " + std::to_string(0 + (jumpStatus == JumpStatus::JUMPING)) + "\n"));

	this->prev = this->sprite;

	if (this->frameTime <= 0)
	{
		this->frameTime = this->frameDelay;
	}
	else
	{
		this->frameTime -= gameTime;
		return;
	}

	if (directionVector != currentDirection) {

		if (directionVector.x > 0)
		{
			if (!lockXFacing) facing = Facing::RIGHT;
			if (directionVector.y == 0) 
			{
				this->sprite = this->pMoveRight;
			}
			else if (directionVector.y > 0)
			{
				this->sprite = this->pMoveRightDown;
			}
			else if (directionVector.y < 0)
			{
				this->sprite = this->pMoveRightUp;
			}
		}
		else if (directionVector.x < 0)
		{
			if (!lockXFacing) facing = Facing::LEFT;
			if (directionVector.y == 0)
			{
				this->sprite = this->pMoveLeft;
			}
			else if (directionVector.y > 0)
			{
				this->sprite = this->pMoveLeftDown;
			}
			else if (directionVector.y < 0)
			{
				this->sprite = this->pMoveLeftUp;
			}
		}
		else
		{
			if (!lockYFacing) facing = Facing::UP;
			if (directionVector.y == 0)
			{
				this->sprite = this->sprite->pDefault;
			}
			else if (directionVector.y > 0)
			{
				if (facing == Facing::LEFT) this->sprite = this->pIdleDownLeft;
				else if (facing == Facing::RIGHT) this->sprite = this->pIdleDownRight;
				else this->sprite = this->pIdleUp;
			}
			else if (directionVector.y < 0)
			{
				if (facing == Facing::LEFT) this->sprite = this->pIdleUpLeft;
				else if (facing == Facing::RIGHT) this->sprite = this->pIdleUpRight;
				else this->sprite = this->pIdleUp;
			}
		}
		//OutputDebugString(ConvertToLPCWSTR(std::to_string(directionVector.x) + " " + std::to_string(directionVector.y) + "\n"));

		this->currentDirection = directionVector;
	}
	else 
	{
		this->sprite = this->sprite->pNext;
	}


}

void Player::Update(float gameTime) {
	if (status == ObjectStatus::ACTIVE)
	{
		position.x += velocity.x * gameTime;
		position.y += velocity.y * gameTime;

		position.x += this->prev->spriteWidth / 2 - this->sprite->spriteWidth / 2;
		position.y += this->prev->spriteHeight - this->sprite->spriteHeight;
	}
}

void Player::SetPositionX(float x)
{
	this->position.x = x;
}

void Player::SetPositionY(float y)
{
	this->position.y = y;
}

void Player::SetPositionZ(float z)
{
	this->position.z = z;
}

D3DXVECTOR3 Player::GetVelocity() const
{
	return velocity;
}

D3DXVECTOR3 Player::GetDirection() const
{
	return this->currentDirection;
}

D3DXVECTOR3 Player::GetMovementVector() const
{
	return this->movementVector;
}

void Player::SetVelocityX(float x)
{
	this->velocity.x = x;
}

void Player::SetVelocityY(float y)
{
	this->velocity.y = y;
}

void Player::SetVelocityZ(float z)
{
	this->velocity.z = z;
}

void Player::SetGravitationalAcceleration(float gravitationalAcceleration)
{
	this->gravitationalAcceleration = gravitationalAcceleration;
}

void Player::SetJumpVelocity(float jumpVelocity)
{
	this->jumpVelocity = jumpVelocity;
}

void Player::ResetJumpCount()
{
	this->jumpCount = maxJump;
}

int Player::GetJumpCount() const
{
	return jumpCount;
}

int Player::GetMaxJumpCount() const
{
	return maxJump;
}

void Player::SetBaseJumpVelocity(float baseJumpVelocity)
{
	this->baseJumpVelocity = baseJumpVelocity;
}

void Player::SetJumpDown(bool b)
{
	this->jumpDown = b;
}

bool Player::GetJumpDown() const
{
	return this->jumpDown;
}
