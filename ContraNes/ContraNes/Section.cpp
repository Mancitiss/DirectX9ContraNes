#include "Section.h"
#include <set>
#include "d3dUtil.h"

Section::Section()
{
	this->bounds.left = 0;
	this->bounds.top = 0;
	this->bounds.right = 0;
	this->bounds.bottom = 0;
}

Section::Section(RECT bounds)
{
	this->bounds = bounds;
}

Section::Section(long x, long y, long w, long h)
{
	this->bounds.left = x;
	this->bounds.top = y;
	this->bounds.right = x + w;
	this->bounds.bottom = y + h;
}

Section::~Section()
{
}

RECT Section::GetBounds()
{
	return this->bounds;
}

void Section::Register(Monster* monster)
{
	this->monsters.insert(monster);
}

void Section::Register(int key)
{
	this->platformKeys.insert(key);
}

void Section::Unregister(Monster* monster)
{
	this->monsters.erase(monster);
}

void Section::Unregister(int key)
{
	this->platformKeys.erase(key);
}

std::unordered_set<Monster*> Section::GetMonsters()
{
	return this->monsters;
}

std::unordered_set<int> Section::GetPlatformKeys()
{
	return this->platformKeys;
}

void Section::ExportTo(std::unordered_set<Monster*>& monsters)
{
	monsters.insert(this->monsters.begin(), this->monsters.end());
	//OutputDebugString(ConvertToLPCWSTR("| size: " + std::to_string(this->monsters.size())));
}

void Section::ExportTo(std::unordered_set<int>& platforms)
{
	for (auto key : this->platformKeys)
	{
		platforms.insert(key);
	}
}

bool Section::Contain(int key)
{
	return this->platformKeys.find(key) != this->platformKeys.end();
}

bool Section::Contain(Monster* monster)
{
	return this->monsters.find(monster) != this->monsters.end();
}