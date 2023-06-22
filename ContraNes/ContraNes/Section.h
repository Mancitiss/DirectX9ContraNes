#pragma once
#include <unordered_set>

#include "GameplayObject.h"
#include "Monster.h"
#include "StandableObject.h"
#include <set>

class Section
{
public:
	Section();
	Section(RECT bounds);
	Section(long x, long y, long w, long h);
	virtual ~Section();

	RECT GetBounds();

	void Register(Monster* monster);
	void Register(int key);
	void Unregister(Monster* monster);
	void Unregister(int key);

	std::unordered_set<Monster*> GetMonsters();
	std::unordered_set<int> GetPlatformKeys();

	void ExportTo(std::unordered_set<Monster*>&);
	void ExportTo(std::unordered_set<int>& platforms);

	bool Contain(int key);
	bool Contain(Monster* monster);

protected:
	RECT bounds;
	std::unordered_set<Monster*> monsters;
	std::unordered_set<int> platformKeys;
};