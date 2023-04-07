#pragma once

#include <DirectXMath.h>
#include "GameplayObject.h"

class Camera 
{
public:
	Camera(float x, float y, float z, int width, int height, float angle , D3DXVECTOR3 scaleFactors);
	~Camera();

	void Update(float dt);
	void Follow(GameplayObject* following);
	void Unfollow();
	bool IsFollowing() const;
	void Render(GameplayObject* object, float gameTime = 0);
	void Render(GameSprite* sprite, float gameTime = 0);
	void SetLimit(float x, float y, float width, float height);
	RECT GetBounds() const;

	D3DXMATRIX transformationMatrix;

private:
	float angle;
	int width;
	int height;
	D3DXVECTOR3 position;
	D3DXVECTOR3 scaleFactors;
	GameplayObject* following;
	RECT* limits;
	bool lockX, lockY;
};