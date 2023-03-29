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

	limits->left = x;
	limits->top = y;
	limits->right = x + width;
	limits->bottom = y + height;

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
					x = limits->left;
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
					y = limits->top;
				}
				else if (y + height / scaleFactors.y > limits->bottom)
				{
					y = limits->bottom - height / scaleFactors.y;
				}
			}
			this->position.y = y;
		}
	}

	//OutputDebugString(ConvertToLPCWSTR("" + std::to_string(position.x) + ", " + std::to_string(position.y) +"\n"));
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
		object->Draw(&relativePosition, &this->scaleFactors , gameTime);
	}
}

void Camera::Render(GameSprite* sprite, float gameTime)
{
	if (sprite->IsInitialized()) {
		D3DXVECTOR3 relativePosition = sprite->GetPosition() - this->position;
		sprite->Draw(&relativePosition);
	}
}