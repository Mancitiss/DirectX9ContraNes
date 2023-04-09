#pragma once
#include "Player.h"

class Monster : public Player
{
public:
	using Player::Player;

	virtual ~Monster() {
		this->target = nullptr;
	}

	virtual void ApplyCollision(GameplayObject* const& object, float gameTime) = 0;

	virtual void ApplyCollision(Player* const& object, float gameTime) = 0;

	void SetMoveType(MoveType moveType) { this->moveType = moveType; }

	MoveType GetMoveType() const { return this->moveType; };

protected:
	MoveType moveType = MoveType::NONE;
	GameplayObject* target;

	float invincibilityDelay = 0;
};