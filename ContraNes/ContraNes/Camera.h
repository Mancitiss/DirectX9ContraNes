#pragma once

#include <DirectXMath.h>
#include "GameplayObject.h"

class Camera 
{
public:
	Camera(int x, int y, int z, int width, int height, float angle , DirectX::XMFLOAT3 scaleFactors);
	~Camera();

	void Update(float dt);
	void Follow(GameplayObject* following);
	void Unfollow();
	bool IsFollowing() const;
	void Render(GameplayObject* object, float gameTime = 0);
	void Render(GameSprite* sprite, float gameTime = 0);
	void SetLimit(float x, float y, float width, float height);

	D3DXMATRIX transformationMatrix;

private:
	float angle;
	int width;
	int height;
	D3DXVECTOR3 position;
	DirectX::XMFLOAT3 scaleFactors;
	GameplayObject* following;
	RECT* limits;
	bool lockX, lockY;
};