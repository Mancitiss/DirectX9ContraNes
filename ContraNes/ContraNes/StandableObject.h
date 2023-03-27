#pragma once
#include "Character.h"

class StandableObject : public GameplayObject
{
public:
	StandableObject(float x, float y, float width, float height, bool fallThrough = true, bool jumpThrough = true, float forgivingWidth = 10, float rotation = 0, float speed = 0, float maxSpeed = 0);
	~StandableObject();
	void ApplyCollision(Character* const& object, float gameTime);
	float GetPlatformWidth() { return platformWidth; }
	float GetPlatformHeight() { return platformHeight; }
	RECT GetBounds() { return bounds; }
	void SetJumpThrough(bool jumpThrough) { this->jumpThrough = jumpThrough; }
	void SetFallThrough(bool fallThrough) { this->fallThrough = fallThrough; }
protected:
	D3DXVECTOR3 platformPosition;
	float forgivingWidth; // the width of the platform that the player can still stand on even if they are not directly on the platform
	bool jumpThrough = true;
	bool fallThrough = true;
	float platformWidth, platformHeight;
	RECT bounds;
};
