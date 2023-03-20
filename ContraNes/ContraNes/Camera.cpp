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
}

void Camera::Update(float dt)
{
	if (this->following && this->following->IsInitialized()) {
		this->position.x = this->following->GetPosition().x - width / 2;
		this->position.y = this->following->GetPosition().y - height / 2;
	}

	OutputDebugString(ConvertToLPCWSTR("" + std::to_string(position.x) + ", " + std::to_string(position.y) +"\n"));
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

void Camera::Render(GameplayObject* object)
{
	if (object->IsInitialized()) {
		D3DXVECTOR3 relativePosition = object->GetPosition() - this->position;
		object->GetSprite()->Draw(&relativePosition);
	}
}

void Camera::Render(GameSprite* sprite)
{
	if (sprite->IsInitialized()) {
		D3DXVECTOR3 relativePosition = sprite->GetPosition() - this->position;
		sprite->Draw(&relativePosition);
	}
}