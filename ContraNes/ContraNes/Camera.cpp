#include "Camera.h"
#include "ContraApp.h"
#include "d3dUtil.h"

Camera::Camera(float x, float y, float z, int width, int height, float angle, D3DXVECTOR3 scaleFactors)
{
	this->width = width;
	this->height = height;
	this->angle = angle;
	this->scaleFactors = scaleFactors;
	this->position = D3DXVECTOR3(x, y, z);
}

Camera::~Camera()
{
	if (limits != nullptr)
	{
		delete limits;
		limits = nullptr;
	}
}

void Camera::SetLimit(float x, float y, float width, float height)
{
	if (limits == nullptr)
	{
		limits = new RECT();
	}

	limits->left = static_cast<LONG>(x);
	limits->top = static_cast<LONG>(y);
	limits->right = static_cast<LONG>( x + width );
	limits->bottom = static_cast<LONG>( y + height );

	if (width * scaleFactors.x <= this->width) {
		this->lockX = true;
		this->position.x = x + width / 2 - this->width / (2 * scaleFactors.x);
	}
	else this->lockX = false;

	if (height * scaleFactors.y <= this->height) {
		this->lockY = true;
		this->position.y = y + height / 2 - this->height / (2 * scaleFactors.y);
	}
	else this->lockY = false;
}

void Camera::Update(float dt)
{
	if (this->following && this->following->IsInitialized()) {
		if (!this->lockX) {
			float x = this->following->GetPosition().x - width / (2 * scaleFactors.x); 
			if (limits != nullptr)
			{
				if (x < limits->left)
				{
					x = static_cast<float>(limits->left);
				}
				else if (x + width / scaleFactors.x > limits->right)
				{
					x = limits->right - width / scaleFactors.x;
				}
			}
			this->position.x = x;
		}

		if (!this->lockY) {
			float y = this->following->GetPosition().y - height / (2 * scaleFactors.y);
			if (limits != nullptr)
			{
				if (y < limits->top)
				{
					y = static_cast<float>(limits->top);
				}
				else if (y + height / scaleFactors.y > limits->bottom)
				{
					y = limits->bottom - height / scaleFactors.y;
				}
			}
			this->position.y = y;
		}
	}
}

void Camera::Follow(GameplayObject* following)
{
	this->following = following;
}

void Camera::Unfollow()
{
	this->following = nullptr;
}

bool Camera::IsFollowing() const
{
	return this->following != nullptr;
}

void Camera::Render(GameplayObject* object, float gameTime)
{
	if (object->IsInitialized()) {
		D3DXVECTOR3 relativePosition = object->GetPosition() - this->position;
		object->Draw(&relativePosition, &this->scaleFactors, gameTime);
	}
}

void Camera::Render2(GameplayObject* object, float gameTime)
{
	if (object->IsInitialized()) {
		RECT obj = object->GetBounds();
		RECT cam = this->GetBounds();
		if (!CheckIntersection(&obj, &cam)) return;
		RECT intersection = RECT();
		FindIntersection(&obj, &cam, &intersection);
		D3DXVECTOR3 relativePosition = object->GetPosition() - this->position;
		D3DXVECTOR3 drawPosition = D3DXVECTOR3(intersection.left - this->position.x, intersection.top - this->position.y, relativePosition.z);
		intersection.left = intersection.left - obj.left;
		intersection.top = intersection.top - obj.top;
		intersection.right = intersection.right - obj.left;
		intersection.bottom = intersection.bottom - obj.top;
		object->Draw(&relativePosition, &drawPosition, intersection, &this->scaleFactors, gameTime);
	}
}

void Camera::Render(GameSprite* sprite, float gameTime)
{
	if (sprite->IsInitialized()) {
		D3DXVECTOR3 relativePosition = sprite->GetPosition() - this->position;
		sprite->Draw(&relativePosition);
	}
}

RECT Camera::GetBounds() const
{
	RECT bounds;
	bounds.left = static_cast<LONG>(this->position.x);
	bounds.top = static_cast<LONG>(this->position.y);
	bounds.right = static_cast<LONG>( this->position.x + this->width / scaleFactors.x );
	bounds.bottom = static_cast<LONG>( this->position.y + this->height / scaleFactors.y );
	return bounds;
}