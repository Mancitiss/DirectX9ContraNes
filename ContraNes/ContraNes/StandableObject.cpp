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
	bounds.left = x;
	bounds.top = y;
	bounds.right = x + width;
	bounds.bottom = y + height;
}

StandableObject::~StandableObject()
{
}

void StandableObject::ApplyCollision(Character* const& object, float gameTime)
{
	// object position is the top left corner of the object
	// platform position is the top left corner of the platform
	// platform width and height are the width and height of the platform
	// forgiving width is the width of the platform that the player can still stand on even if they are not directly on the platform
	// object width and height are the width and height of the object
	// object velocity is the velocity of the object

	// if this function is called, the object will certainly collide with the platform

	// if the object is above the platform, then the object will be standing on the platform
	// if the object is below the platform, then the object will be falling through the platform

	float changedX = object->GetVelocity().x * gameTime;
	float changedY = object->GetVelocity().y * gameTime;

	if (object->GetJumpState() == JumpStatus::JUMPING && object->GetPosition().x + object->GetSprite()->spriteWidth - changedX > platformPosition.x - forgivingWidth
		|| object->GetPosition().x - changedX < platformPosition.x + platformWidth + forgivingWidth)
	{
		if (object->GetPosition().y + object->GetSprite()->spriteHeight - changedY <= platformPosition.y)
		{
			// the object is above the platform
			// the object will be standing on the platform
			object->SetPositionY(platformPosition.y - object->GetSprite()->spriteHeight );
			object->SetVelocityY(0);
			object->SetJumpVelocity(0);
			object->SetJumpState(JumpStatus::IDLE);
		}
		else if (object->GetPosition().y + object->GetSprite()->spriteHeight - changedY > platformPosition.y && object->GetPosition().y > platformPosition.y - platformHeight)
		{
			// the object is below the platform
			// the object will be falling through the platform
			if (!jumpThrough && object->GetVelocity().y < 0)
			{
				object->SetPositionY(platformPosition.y + platformHeight); 
				object->SetVelocityY(0);
				object->SetJumpVelocity(0);
			}
			else if (!fallThrough && object->GetVelocity().y > 0)
			{
				object->SetPositionY(platformPosition.y - object->GetSprite()->spriteHeight);
			}
		}
	}
	else {
		if (object->GetPosition().x - changedX > platformPosition.x + platformWidth + forgivingWidth)
		{
			object->SetPositionX(platformPosition.x + platformWidth);
		}
		else
		{
			object->SetPositionX(platformPosition.x - object->GetSprite()->spriteWidth);
		}
	}
}