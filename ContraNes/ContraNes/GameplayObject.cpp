

#include "GameplayObject.h"


GameplayObject::GameplayObject(float x, float y, float rotation, float speed, float maxSpeed)
{
	float two_pi = (float)(M_PI * 2.0f);
	this->position = D3DXVECTOR3(x, y, 0);
	this->rotation = (rotation > two_pi)? rotation - two_pi : rotation;
	this->maxSpeed = maxSpeed;
	if (0 <= speed && speed <= maxSpeed)
	{
		this->speed = speed;
	}
	else
	{
		this->speed = 0;
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
		this->speed = speed;
	}
	else
	{
		this->speed = 0;
	}

	velocity.x = cos(rotation) * speed;
	velocity.y = sin(rotation) * speed;
	velocity.z = 0;
}

GameplayObject::~GameplayObject()
{
	if (sprite)
	{
		delete sprite;
		sprite = nullptr;
	}
}

bool GameplayObject::Init(LPDIRECT3DDEVICE9 device, LPCTSTR file, int width, int height)
{
	status = ObjectStatus::Active;
	if (!sprite) {
		sprite = new GameSprite();
		if (!sprite->Init(device, file, width, height))
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
		sprite->Draw(dt, position);
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
		this->speed = speed;
		velocity.x = cos(rotation) * speed;
		velocity.y = sin(rotation) * speed;
		velocity.z = 0;
	}
}

GameSprite* GameplayObject::GetSprite() const
{
	return sprite;
}

D3DXVECTOR3* GameplayObject::GetPosition() const
{
	return (D3DXVECTOR3*)&position;
}



void GameplayObject::HandleInput()
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
		float angle = atan2(movementVector.y, movementVector.x);

		velocity.x = cos(angle) * speed;
		velocity.y = sin(angle) * speed;
	}
	else
	{
		velocity.x = 0;
		velocity.y = 0;
	}
}