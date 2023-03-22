#include "Camera.h"
#include "ContraApp.h"
#include "d3dUtil.h"

Camera::Camera(int x, int y, int z, int width, int height, float angle, DirectX::XMFLOAT3 scaleFactors)
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

	if (width <= this->width) {
		this->lockX = true;
		this->position.x = x + width / 2 - this->width / 2;
	}
	else this->lockX = false;

	if (height <= this->height) {
		this->lockY = true;
		this->position.y = y + height / 2 - this->height / 2;
	}
	else this->lockY = false;
}

void Camera::Update(float dt)
{
	if (this->following && this->following->IsInitialized()) {
		if (!this->lockX) {
			float x = this->following->GetPosition().x - width / 2; 
			if (limits != nullptr)
			{
				if (x < limits->left)
				{
					x = limits->left;
				}
				else if (x + width > limits->right)
				{
					x = limits->right - width;
				}
			}
			this->position.x = x;
		}

		if (!this->lockY) {
			float y = this->following->GetPosition().y - height / 2;
			if (limits != nullptr)
			{
				if (y < limits->top)
				{
					y = limits->top;
				}
				else if (y + height > limits->bottom)
				{
					y = limits->bottom - height;
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
		object->Draw(&relativePosition, gameTime);
	}
}

void Camera::Render(GameSprite* sprite, float gameTime)
{
	if (sprite->IsInitialized()) {
		D3DXVECTOR3 relativePosition = sprite->GetPosition() - this->position;
		sprite->Draw(&relativePosition);
	}
}