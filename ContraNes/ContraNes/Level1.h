#pragma once
#include "Level.h"
#include "Camera.h"

class Level1 : public Level
{
public:
	Level1();
	virtual ~Level1();
	virtual bool Init(LPDIRECT3DDEVICE9 m_pDevice3D, Camera* camera);
	virtual void GetSurroundingObjects(D3DXVECTOR3 position, GameplayObject* &background, std::vector<StandableObject*> &platforms, std::list<Monster*> &monsters) override;
	virtual void Update(StandableObject* object) override;
	virtual void Update(Monster* monster) override;
	virtual void Remove(StandableObject* object) override;
	virtual void Remove(Monster* monster) override;
private:
	void InitPlatforms(std::unordered_map<int, std::unique_ptr<StandableObject>> &platforms);
	void InitMonsters(LPDIRECT3DDEVICE9 m_pDevice3D, std::list<std::unique_ptr<Monster>> &monsters);
};