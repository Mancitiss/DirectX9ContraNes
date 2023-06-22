#include "Bullet.h"
#include "d3dUtil.h"

GameSprite* Bullet::prev = nullptr;
GameSprite* Bullet::pNormal = nullptr;
GameSprite* Bullet::pDying = nullptr;

Bullet::Bullet(float x, float y, float rotation, float speed, float maxSpeed, float width, float height,  D3DXVECTOR3 internalScale) : GameplayObject(x, y, rotation, speed, maxSpeed, internalScale)
{
	this->width = width;
	this->height = height;
	this->bounds = { (LONG)x, (LONG)y, (LONG)(x + width), (LONG)(y + height) };
}

Bullet::~Bullet()
{
	/*GameSprite* pSprite = pDying;
	while (pSprite)
	{
		GameSprite* pNext = nullptr;
		if (pSprite->pNext != pSprite) pNext = pSprite->pNext;
		delete pSprite;
		pSprite = pNext;
	}

	if (pNormal)
	{
		delete pNormal;
		pNormal = nullptr;
	}*/

	sprite = nullptr;
}

bool Bullet::Init(PDIRECT3DDEVICE9 device, float frameDelay, LPCTSTR file, D3DCOLOR backColor, D3DCOLOR displayColor)
{
	if (!Bullet::pNormal)
	{
		Bullet::pNormal = new GameSprite();
		if (!Bullet::pNormal->Init(device, file, this->width, this->height, this->rotation, this->internalScale, backColor, displayColor))
		{
			return false;
		}
		Bullet::pNormal->pNext = Bullet::pNormal;
		Bullet::pNormal->pDefault = Bullet::pNormal;
	}

	if (!Bullet::pDying)
	{
		if (!CreateSprites(device, 5, "resources/bullet", ".png", Bullet::pDying, Bullet::pDying, this->internalScale, 0))
		{
			return false;
		}
		GameSprite* pSprite = this->pDying;
		while (pSprite->pNext != pDying)
		{
			pSprite = pSprite->pNext;
		}
		pSprite->pNext = pSprite;
		pSprite->pDefault = pSprite;
	}

	this->sprite = pNormal;

	this->frameDelay = frameDelay;
	this->frameTime = frameDelay;
	this->initialized = true;

	this->status = ObjectStatus::ACTIVE;
	return true;
}

void Bullet::ApplyCollision(Player* const& object)
{
	object->TakeDamage(this->GetDamage());
	this->status = ObjectStatus::DYING;
	this->sprite->pNext = Bullet::pDying;
}

void Bullet::Update(float gameTime)
{
	if (this->status == ObjectStatus::ACTIVE) 
	{
		GameplayObject::Update(gameTime);
		this->bounds = { (LONG)position.x, (LONG)position.y, (LONG)(position.x + width), (LONG)(position.y + height) };
	}
	else if (this->status == ObjectStatus::DYING)
	{
		if (this->frameTime <= 0)
		{
			this->frameTime = this->frameDelay;
		}
		else
		{
			this->frameTime -= gameTime;
			return;
		}
		if (this->sprite->pNext == this->sprite) {
			this->status = ObjectStatus::DEAD;
		}
		else {
			this->sprite = this->sprite->pNext;
		}
	}
}