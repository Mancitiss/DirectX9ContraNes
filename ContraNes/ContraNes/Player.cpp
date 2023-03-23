#include "Player.h"
#include "Keyboard.h"
#include "d3dUtil.h"

Player::Player(float x, float y) : GameplayObject(x, y, 0, 0, 0, 300.0f)
{
	gravitationalAcceleration = 2000;
	jumpVelocity = 0;
	jumpStatus = JumpStatus::JUMPING;
}

Player::Player(float x, float y, float rotation, float speed, float maxSpeed) : GameplayObject(x, y, rotation, speed, maxSpeed)
{
	gravitationalAcceleration = 2000;
	jumpVelocity = 0;
	jumpStatus = JumpStatus::JUMPING;
}

Player::Player(float x, float y, float z, float rotation, float speed, float maxSpeed) : GameplayObject(x, y, z, rotation, speed, maxSpeed)
{
	gravitationalAcceleration = 2000;
	jumpVelocity = 0;
	jumpStatus = JumpStatus::JUMPING;
}

Player::~Player()
{
}

void Player::HandleInput(float gameTime)
{
	D3DXVECTOR3 movementVector(0, 0, 0);

	// if the up arrow is pressed, move the sprite up
	if (GetAsyncKeyState(Keyboard::UP))
	{
		if (jumpStatus == JumpStatus::IDLE)
		{
			jumpStatus = JumpStatus::JUMPING;
			jumpVelocity = 1000;
		}
		movementVector.y -= 1;
	}

	// if the down arrow is pressed, move the sprite down
	if (GetAsyncKeyState(Keyboard::DOWN))
	{
		if (jumpStatus == JumpStatus::IDLE)
		{
			jumpStatus = JumpStatus::JUMPING;
			jumpVelocity = 0;
		}
		movementVector.y += 1;
	}

	// if the left arrow is pressed, move the sprite left
	if (GetAsyncKeyState(Keyboard::LEFT))
	{
		movementVector.x -= 1;
	}

	// if the right arrow is pressed, move the sprite right
	if (GetAsyncKeyState(Keyboard::RIGHT))
	{
		movementVector.x += 1;
	}

	if (movementVector.x != 0 || movementVector.y != 0)
	{
		rotation = atan2(movementVector.y, movementVector.x);
	}
	currentJerk = min(currentJerk + jerkIncrementPerSecond3 * gameTime, maxSpeed / (gameTime * gameTime));
	currentAcceleration = min(currentAcceleration + currentJerk * gameTime, maxSpeed / gameTime);
	currentVelocity = min(currentVelocity + currentAcceleration * gameTime, maxSpeed);
	//velocity.x = cos(rotation) * currentVelocity;
	// velocity.y = sin(rotation) * currentVelocity;
	velocity.x = movementVector.x * currentVelocity;
	if (jumpStatus == JumpStatus::JUMPING) {
		if (movementVector.y >= 0 && jumpVelocity > 0) jumpVelocity = - gravitationalAcceleration * gameTime;
		velocity.y = - jumpVelocity;
		jumpVelocity -= gravitationalAcceleration * gameTime;
		//jumpVelocity -= jumpVelocity * gameTime;
	}
	OutputDebugString(ConvertToLPCWSTR(std::to_string(gravitationalAcceleration) + " " + std::to_string(gameTime) + " " + std::to_string(jumpVelocity) + ", " + std::to_string(velocity.y) + ", " + std::to_string(0 + (jumpStatus == JumpStatus::JUMPING)) + "\n"));
	/*}
	else
	{
		velocity.x = 0;
		velocity.y = 0;
		currentVelocity = 0;
		currentAcceleration = 0;
		currentJerk = 0;
	}*/
}

void Player::Update(float gameTime) {
	if (status == ObjectStatus::ACTIVE)
	{
		position.x += velocity.x * gameTime;
		if (/*(position.y > 480 - this->sprite->spriteHeight) && */jumpStatus == JumpStatus::JUMPING)
		{
			position.y += velocity.y * gameTime;
			if (position.y > 480 - this->sprite->spriteHeight)
			{
				position.y = 480 - this->sprite->spriteHeight;
				jumpStatus = JumpStatus::IDLE;
			}
			//OutputDebugString(ConvertToLPCWSTR("pos: " + std::to_string(position.x) + ", " + std::to_string(position.y) + "," + std::to_string(position.z) + "\n"));
		}
		else 
		{
			jumpVelocity = 0;
			velocity.y = 0;
		}
	}
}

void Player::SetGravitationalAcceleration(float gravitationalAcceleration)
{
	this->gravitationalAcceleration = gravitationalAcceleration;
}

void Player::SetJumpVelocity(float jumpVelocity)
{
	this->jumpVelocity = jumpVelocity;
}