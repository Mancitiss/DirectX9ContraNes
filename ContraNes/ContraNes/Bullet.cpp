#include "Bullet.h";

Bullet::Bullet(float x, float y, float width, float height, float rotation, float speed, float maxSpeed) : GameplayObject(x, y, rotation, speed, maxSpeed)
{
	this->width = width;
	this->height = height;
	this->bounds = { (LONG)x, (LONG)y, (LONG)(x + width), (LONG)(y + height) };
}

Bullet::~Bullet()
{
}

void Bullet::Init(PDIRECT3DDEVICE9 device, LPCTSTR file, D3DCOLOR backColor, D3DCOLOR displayColor)
{
	GameplayObject::Init(device, file, this->width, this->height, this->rotation, backColor, displayColor);
}

void Bullet::ApplyCollision(Player* const& object, float gameTime)
{
	object->TakeDamage(this->GetDamage());
	this->status = ObjectStatus::DYING;
}

void Bullet::Update(float gameTime)
{
	GameplayObject::Update(gameTime);
	this->bounds = { (LONG)position.x, (LONG)position.y, (LONG)(position.x + width), (LONG)(position.y + height) };
}