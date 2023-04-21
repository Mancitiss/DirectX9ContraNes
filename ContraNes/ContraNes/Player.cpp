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
	
	// pDieLeft and pDieRight are not circular list but are linked list so we need to delete them differently
	GameSprite* pSprite = pDieLeft;
	while (pSprite)
	{
		GameSprite* pNext = nullptr;
		if (pSprite->pNext != pSprite) pNext = pSprite->pNext;
		delete pSprite;
		pSprite = pNext;
	}

	pSprite = pDieRight;
	while (pSprite)
	{
		GameSprite* pNext = nullptr;
		if (pSprite->pNext != pSprite) pNext = pSprite->pNext;
		delete pSprite;
		pSprite = pNext;
	}

	this->sprite = nullptr;
}

bool Player::Init(LPDIRECT3DDEVICE9 device, float frameDelay)
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

	if (!this->pDieLeft)
	{
		if (!CreateSprites(device, 5, "resources/player/DyingL", ".png", this->pDieLeft, this->pIdleRight, internalScale))
		{
			return false;
		}
	}

	GameSprite* pSprite = this->pDieLeft;
	while (pSprite->pNext != this->pDieLeft)
	{
		pSprite = pSprite->pNext;
	}
	pSprite->pNext = pSprite;

	if (!this->pDieRight)
	{
		if (!CreateSprites(device, 5, "resources/player/DyingR", ".png", this->pDieRight, this->pIdleLeft, internalScale))
		{
			return false;
		}
	}

	pSprite = this->pDieRight;
	while (pSprite->pNext != this->pDieRight)
	{
		pSprite = pSprite->pNext;
	}
	pSprite->pNext = pSprite;

	this->sprite = this->pIdleRight;

	this->frameDelay = frameDelay;
	this->frameTime = frameDelay;

	this->initialized = true;
	return true;
}

void Player::_defaultHandle(D3DXVECTOR3& movement, D3DXVECTOR3& direction)
{
	if (this->status != ObjectStatus::ACTIVE) return;
	SHORT UP, DOWN, LEFT, RIGHT, JUMP, SHOOT;
	UP = GetAsyncKeyState(Keyboard::UP);
	DOWN = GetAsyncKeyState(Keyboard::DOWN);
	LEFT = GetAsyncKeyState(Keyboard::LEFT);
	RIGHT = GetAsyncKeyState(Keyboard::RIGHT);
	JUMP = GetAsyncKeyState(Keyboard::JUMP);
	SHOOT = GetAsyncKeyState(Keyboard::SHOOT);
	// if the down arrow and space are pressed at the same time, move the sprite down
	if (DOWN & 0x8000 && (JUMP & 0x8001) == 0x8001)
	{
		if (!hasJumped && jumpCount > 0)
		{
			//OutputDebugString(L"Jumping Down\n");
			hasJumped = true;

			jumpCount -= 1;
			jumpVelocity = 0.0f;
			this->jumpDown = true;
		}
		movement.y += 1;
	}
	else if (JUMP) 
	{
		if (!hasJumped && jumpCount > 0 && ((JUMP & 0x8001) == 0x8001))
		{
			//OutputDebugString(L"Jumping\n");
			hasJumped = true;
			jumpCount -= 1;
			jumpVelocity = baseJumpVelocity;
		}
		movement.y -= 1;
	}
	else hasJumped = false;

	// if the up arrow is pressed, move the sprite up
	if (UP & 0x8000) {
		direction.y -= 1;
	}

	if (DOWN & 0x8000) {
		direction.y += 1;
		//OutputDebugString(L"Down\n");
	}

	// if the left arrow is pressed, move the sprite left
	if (LEFT & 0x8000)
	{
		movement.x -= 1;
		direction.x -= 1;
	}

	// if the right arrow is pressed, move the sprite right
	if (RIGHT & 0x8000)
	{
		movement.x += 1;
		direction.x += 1;
	}

	// if the F key is pressed, fire a bullet
	if (SHOOT & 0x8000)
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
		if (movementVector.y >= 0 && jumpVelocity >= 0) jumpVelocity = - gravitationalAcceleration * gameTime;
		velocity.y = - jumpVelocity;
		jumpVelocity -= gravitationalAcceleration * gameTime;
	//}
	//OutputDebugString(ConvertToLPCWSTR(std::to_string(gravitationalAcceleration) + " " + std::to_string(gameTime) + " " + std::to_string(jumpVelocity) + ", " + std::to_string(velocity.y) + ", " + std::to_string(0 + (jumpStatus == JumpStatus::JUMPING)) + "\n"));

	this->prev = this->sprite;

	if (status != ObjectStatus::ACTIVE) return;

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

void Player::Update(float gameTime) 
{
	//if (status == ObjectStatus::ACTIVE)
	//{
	if (this->invincibilityTime > 0) {
		this->invincibilityTime -= gameTime;
	}

	position.x += velocity.x * gameTime;
	position.y += velocity.y * gameTime;

	position.x += this->prev->spriteWidth / 2 - this->sprite->spriteWidth / 2;
	position.y += this->prev->spriteHeight - this->sprite->spriteHeight;
	//}
	//else 

	if (status == ObjectStatus::DEAD && health > 0) 
	{
		this->status = ObjectStatus::ACTIVE;
		this->invincibilityTime = 1;
		this->prev = this->sprite;
		this->sprite = this->pIdleRight;
		this->respawn = true;
	}
	else if (status == ObjectStatus::DYING)
	{
		/*if (this->invincibilityTime > 0) {
			this->invincibilityTime -= gameTime;
		}*/
		if (this->frameTime <= 0)
		{
			this->frameTime = this->frameDelay;
		}
		else
		{
			this->frameTime -= gameTime;
			return;
		}
		if (this->sprite->pNext == this->sprite && this->invincibilityTime <= 0) {
			this->status = ObjectStatus::DEAD;
		}
		else {
			this->sprite = this->sprite->pNext;
		}
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

GameSprite* Player::GetPrev() const
{
	return this->prev;
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

void Player::TakeDamage(int damage)
{
	if (this->status == ObjectStatus::ACTIVE && this->invincibilityTime <= 0)
	{
		this->health -= damage;
		this->invincibilityTime = this->invincibilityDelay;
		if (this->health <= 0)
		{
			this->status = ObjectStatus::DEAD;
		}
		else
		{
			this->status = ObjectStatus::DYING;
			this->invincibilityTime = 1;
			if (facing == Facing::LEFT) this->sprite = this->pDieLeft;
			else this->sprite = this->pDieRight;
		}
	}
}

void Player::SetHealth(int health)
{
	this->health = health;
}

int Player::GetHealth() const
{
	return this->health;
}

void Player::SetDamage(int damage)
{
	this->damage = damage;
}

int Player::GetDamage() const
{
	return this->damage;
}

void Player::SetInvincibilityTime(float invincibilityTime)
{
	this->invincibilityTime = invincibilityTime;
}

float Player::GetInvincibilityTime() const
{
	return this->invincibilityTime;
}

void Player::SetInvincibilityDelay(float invincibilityDelay)
{
	this->invincibilityDelay = invincibilityDelay;
}

float Player::GetInvincibilityDelay() const
{
	return this->invincibilityDelay;
}