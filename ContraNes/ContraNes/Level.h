#pragma once

#include <memory>
#include <unordered_map>
#include "GameplayObject.h"
#include "StandableObject.h"
#include "Section.h"
#include "Camera.h"

class Level
{
public:
	Level() {};
	virtual ~Level()
	{
		platformKeys.clear();

		platforms.clear();
		monsters.clear();
		sections.clear();
	};
	virtual bool Init(LPDIRECT3DDEVICE9 m_pDevice3D, Camera* camera) = 0;
	virtual void GetSurroundingObjects(D3DXVECTOR3 position, GameplayObject* &background, std::vector<StandableObject*> &platforms, std::list<Monster*> &monsters) = 0;
	virtual void Update(StandableObject* object) = 0;
	virtual void Update(Monster* monster) = 0;
	virtual void Remove(StandableObject* object) = 0;
	virtual void Remove(Monster* monster) = 0;
	virtual D3DXVECTOR3 GetStartPosition() 
	{
		return startPosition;
	}
protected:
	long sWidth, sHeight;
	D3DXVECTOR3 startPosition;
	std::unique_ptr<GameplayObject> background;
	std::unordered_map<int, std::unique_ptr<StandableObject>> platforms;
	std::unordered_map<StandableObject*, int> platformKeys;
	std::list<std::unique_ptr<Monster>> monsters;
	std::vector<std::vector<std::unique_ptr<Section>>> sections;

	std::set<int> keys;
	std::unordered_set<Monster*> monsterSet;
};