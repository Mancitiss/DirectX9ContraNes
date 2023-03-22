

#include "GameplayObject.h"


GameplayObject::GameplayObject(float x, float y, float rotation, float speed, float maxSpeed)
{
	float two_pi = (float)(M_PI * 2.0f);
	this->position = D3DXVECTOR3(x, y, 0);
	this->rotation = (rotation > two_pi)? rotation - two_pi : rotation;
	this->maxSpeed = maxSpeed;
	if (0 <= speed && speed <= maxSpeed)
	{
		this->currentVelocity = speed;
	}
	else
	{
		this->currentVelocity = maxSpeed;
	}

	velocity.x = cos(rotation) * speed;
	velocity.y = sin(rotation) * speed;
	velocity.z = 0;
	
}

GameplayObject::GameplayObject(float x, float y, float z, float rotation, float speed, float maxSpeed)
{
	float two_pi = (float)(M_PI * 2.0f);
	this->position = D3DXVECTOR3(x, y, z);
	this->rotation = (rotation > two_pi) ? rotation - two_pi : rotation;
	this->maxSpeed = maxSpeed;
	if (0 <= speed && speed <= maxSpeed)
	{
		this->currentVelocity = speed;
	}
	else
	{
		this->currentVelocity = maxSpeed;
	}

	velocity.x = cos(rotation) * speed;
	velocity.y = sin(rotation) * speed;
	velocity.z = 0;

	currentAcceleration = 0;
	currentJerk = 0;
}

GameplayObject::~GameplayObject()
{
	if (sprite)
	{
		delete sprite;
		sprite = nullptr;
	}
}

bool GameplayObject::Init(LPDIRECT3DDEVICE9 device, LPCTSTR file, int width, int height, float baseZRotation, D3DCOLOR backColor, D3DCOLOR displayColor)
{
	status = ObjectStatus::Active;
	if (!sprite) {
		sprite = new GameSprite();
		if (!sprite->Init(device, file, width, height, baseZRotation))
		{
			return false;
		}
	}

	initialized = true;
	return true;
}

bool GameplayObject::IsInitialized() const
{
	return initialized;
}

void GameplayObject::Update(float gameTime)
{
	if (status == ObjectStatus::Active)
	{
		position.x += velocity.x * gameTime;
		position.y += velocity.y * gameTime;
	}
}

void GameplayObject::Draw(float dt)
{
	if (sprite) {
		
		sprite->Draw(&position);
	}
}

void GameplayObject::Draw(D3DXVECTOR3* position, float dt)
{
	if (sprite) {

		sprite->Draw(position, rotation);
	}
}

ObjectStatus GameplayObject::GetStatus() const
{
	return status;
}

void GameplayObject::SetSpeed(float speed)
{
	if (0 <= speed && speed <= maxSpeed)
	{
		this->currentVelocity = speed;
		this->currentJerk = 0;
		this->currentAcceleration = 0;
		velocity.x = cos(rotation) * speed;
		velocity.y = sin(rotation) * speed;
		velocity.z = 0;
	}
}

GameSprite* GameplayObject::GetSprite() const
{
	return sprite;
}

D3DXVECTOR3 GameplayObject::GetPosition() const
{
	return position;
}

void GameplayObject::HandleInput(float gameTime)
{
	D3DXVECTOR3 movementVector(0, 0, 0);

	// if the up arrow is pressed, move the sprite up
	if (GetAsyncKeyState(VK_UP))
	{
		movementVector.y -= 1;
	}

	// if the down arrow is pressed, move the sprite down
	if (GetAsyncKeyState(VK_DOWN))
	{
		movementVector.y += 1;
	}

	// if the left arrow is pressed, move the sprite left
	if (GetAsyncKeyState(VK_LEFT))
	{
		movementVector.x -= 1;
	}

	// if the right arrow is pressed, move the sprite right
	if (GetAsyncKeyState(VK_RIGHT))
	{
		movementVector.x += 1;
	}

	if (movementVector.x != 0 || movementVector.y != 0)
	{
		rotation = atan2(movementVector.y, movementVector.x);
		currentJerk = min(currentJerk + jerkIncrementPerSecond3 * gameTime, maxSpeed/(gameTime*gameTime));
		currentAcceleration = min(currentAcceleration + currentJerk * gameTime,  maxSpeed/gameTime);
		currentVelocity = min(currentVelocity + currentAcceleration * gameTime, maxSpeed);
		velocity.x = cos(rotation) * currentVelocity;
		velocity.y = sin(rotation) * currentVelocity;
	}
	else
	{
		velocity.x = 0;
		velocity.y = 0;
		currentVelocity = 0;
		currentAcceleration = 0;
		currentJerk = 0;
	}
}

void GameplayObject::SetJerkIncrementPerSecond3(float jerk)
{
	this->jerkIncrementPerSecond3 = jerk;
}