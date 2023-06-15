#include "StandableObject.h"

StandableObject::StandableObject(float x, float y, float width, float height, bool fallThrough, bool jumpThrough, float forgivingWidth, float rotation, float speed, float maxSpeed) : GameplayObject(x, y, rotation, speed, maxSpeed)
{
	platformPosition = D3DXVECTOR3(x, y, 0);
	this->jumpThrough = jumpThrough;
	this->fallThrough = fallThrough;
	this->forgivingWidth = forgivingWidth;
	this->platformWidth = width;
	this->platformHeight = height;
	bounds = RECT();
	bounds.left = (LONG)x;
	bounds.top = (LONG)y;
	bounds.right = (LONG)(x + width);
	bounds.bottom = (LONG)(y + height);
}

StandableObject::~StandableObject()
{
}

void StandableObject::Update(float gametime)
{
	GameplayObject::Update(gametime);
	platformPosition.x += velocity.x * gametime;
	platformPosition.y += velocity.y * gametime;
	bounds.left = (LONG)platformPosition.x;
	bounds.top = (LONG)platformPosition.y;
	bounds.right = (LONG)(platformPosition.x + platformWidth);
	bounds.bottom = (LONG)(platformPosition.y + platformHeight);
}

void StandableObject::ApplyCollision(Character* const& object, float gameTime)
{
	if (object->GetJumpDown() && object->prev_position.y + object->GetSprite()->spriteHeight - 2 < platformPosition.y) { // object is above platform
		object->ignore.insert(this);
		object->SetJumpDown(false);
		object->SetPositionY(object->GetPosition().y + 1);
		//OutputDebugString(L"ignored");
		//return;
	}

	bool ignore = !(object->ignore.find(this) == object->ignore.end());

	if (!ignore /* && !object->GetJumpDown()*/ && object->prev_position.y + object->GetSprite()->spriteHeight - 2 < platformPosition.y) // object i
	{

		object->SetPositionY(platformPosition.y - object->GetSprite()->spriteHeight + 1);
		object->SetVelocityX(object->GetVelocity().x + this->velocity.x);
		object->SetVelocityY(0);
		object->SetJumpVelocity(0);
		object->ResetJumpCount();
		//object->SetJumpState(JumpStatus::IDLE);
		object->SetJumpDown(false);
		//OutputDebugString(L" Up ");
	}
	else // object->GetPosition().y + object->GetSprite()->spriteHeight - changedY > platformPosition.y
	{
		if (object->GetVelocity().y <= 0) {
			if (!jumpThrough) {
				if (!ignore && object->prev_position.x > platformPosition.x + platformWidth)
				{
					object->SetPositionX(platformPosition.x + platformWidth);
					object->SetVelocityX(0);

				}
				else if (!ignore && object->prev_position.x < platformPosition.x)
				{
					object->SetPositionX(platformPosition.x - object->GetSprite()->spriteWidth);
					object->SetVelocityX(0);
				}
				else
				{
					object->SetPositionY(platformPosition.y + platformHeight);
					object->SetVelocityY(0);
					object->SetJumpVelocity(0);
				}
			}
		}
		else { // velocity.y > 0
			//object->ignore.insert(this);
			//object->SetJumpDown(false);
			if (!fallThrough)
			{
				if (!ignore && object->prev_position.x > platformPosition.x + platformWidth)
				{
					object->SetPositionX(platformPosition.x + platformWidth);
					object->SetVelocityX(0);

				}
				else if (!ignore && object->prev_position.x < platformPosition.x)
				{
					object->SetPositionX(platformPosition.x - object->GetSprite()->spriteWidth);
					object->SetVelocityX(0);
				}
				else
				{
					object->SetPositionY(platformPosition.y - object->GetSprite()->spriteHeight + 1);
					object->prev_position.y = object->GetPosition().y;
					object->SetVelocityX(object->GetVelocity().x + this->velocity.x);
					object->SetVelocityY(0);
					object->SetJumpVelocity(0);
					object->ResetJumpCount();
					object->SetJumpDown(false);
				}
			}
		}
	}
}