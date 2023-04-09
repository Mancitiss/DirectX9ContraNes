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

	float changedX = object->GetVelocity().x * gameTime + object->GetPrev()->spriteWidth / 2 - object->GetSprite()->spriteWidth / 2 ;
	float changedY = object->GetVelocity().y * gameTime + object->GetPrev()->spriteHeight / 2 - object->GetSprite()->spriteHeight / 2;

	/*if (object->GetPosition().x + object->GetSprite()->spriteWidth - changedX > platformPosition.x - forgivingWidth
		|| object->GetPosition().x - changedX < platformPosition.x + platformWidth + forgivingWidth)
	{*/
		if (!object->GetJumpDown() && object->GetPosition().y + object->GetSprite()->spriteHeight - changedY - 2 < platformPosition.y)
		{
			// the object is above the platform
			// the object will be standing on the platform
			object->SetPositionY(platformPosition.y - object->GetSprite()->spriteHeight + 1 );
			object->SetVelocityY(0);
			object->SetJumpVelocity(0);
			object->ResetJumpCount();
			//object->SetJumpState(JumpStatus::IDLE);
			object->SetJumpDown(false);
		}
		else // object->GetPosition().y + object->GetSprite()->spriteHeight - changedY > platformPosition.y
		{
			if (object->GetVelocity().y <= 0) {
				if (!jumpThrough) {
					object->SetPositionY(platformPosition.y + platformHeight);
					object->SetVelocityY(0);
					object->SetJumpVelocity(0);
				}
			}
			else { // velocity.y > 0

				object->SetJumpDown(false);
				object->SetPositionY(object->GetPosition().y + 2);
				if (!fallThrough )
				{
					object->SetPositionY(platformPosition.y - object->GetSprite()->spriteHeight + 1);
					object->SetVelocityY(0);
					object->SetJumpVelocity(0);
					object->ResetJumpCount();
					object->SetJumpDown(false);
				}
				//if (fallThrough && (object->GetJumpDown() || object->GetPosition().y > platformPosition.y )) {
				//	
				//}
				//else //if (!fallThrough || (!object->GetJumpDown()))// && object->GetPosition().y <= platformPosition.y ) 
				//{
				//	object->SetPositionY(platformPosition.y - object->GetSprite()->spriteHeight + 1);
				//	object->SetVelocityY(0);
				//	object->SetJumpVelocity(0);
				//	object->ResetJumpCount();
				//	//object->SetJumpDown(false);
				//}
			}
		}
	/*}
	else {*/
		//OutputDebugString(L"alo");
		/*if (object->GetPosition().x - changedX > platformPosition.x + platformWidth)
		{
			object->SetPositionX(platformPosition.x + platformWidth + 10);
			object->SetVelocityX(0);

		}
		else if (object->GetPosition().x - changedX < platformPosition.x)
		{
			object->SetPositionX(platformPosition.x - 30 - 10);
			object->SetVelocityX(0);
		}*/
	//}
}