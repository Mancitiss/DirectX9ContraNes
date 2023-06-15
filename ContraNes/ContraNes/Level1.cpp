#include "Level1.h"
#include "Runner.h"
#include "Shooter.h"
#include "d3dUtil.h"

Level1::Level1()
{
}

Level1::~Level1()
{
}

bool Level1::Init(LPDIRECT3DDEVICE9 m_pDevice3D, Camera* camera)
{
	this->startPosition = D3DXVECTOR3(100, 5, 0);

	this->background = std::make_unique<GameplayObject>(0, 0, 1.0f, 0, 0, 0);
	if (!background->Init(m_pDevice3D, L"resources/l1.png", 6771, 480)) return false;
	camera->SetLimit(background->GetPosition().x, background->GetPosition().y, background->GetSprite()->spriteWidth * 1.0f, background->GetSprite()->spriteHeight * 1.0f);

	RECT rect = camera->GetBounds();
	sWidth = rect.right - rect.left;
	sHeight = rect.bottom - rect.top;

	long backgroundsWidth = background->GetSprite()->spriteWidth;
	long backgroundsHeight = background->GetSprite()->spriteHeight;

	// create sections
	for (int i = 0; i <= backgroundsHeight / sHeight; i++)
	{
		sections.push_back(std::vector<std::unique_ptr<Section>>());
		for (int j = 0; j <= backgroundsWidth / sWidth; j++)
		{
			sections[i].push_back(std::make_unique<Section>(sWidth *j, sHeight *i, sWidth, sHeight));
		}
	}
	
	InitPlatforms(platforms);
	InitMonsters(m_pDevice3D, monsters);

	return true;
}

void Level1::GetSurroundingObjects(D3DXVECTOR3 position, GameplayObject* &background, std::vector<StandableObject*> &platforms, std::list<Monster*> &monsters)
{
	long sWidth = sections[0][0]->GetBounds().right - sections[0][0]->GetBounds().left;
	long sHeight = sections[0][0]->GetBounds().bottom - sections[0][0]->GetBounds().top;
	int w_index = position.x / sWidth;
	int h_index = position.y / sHeight;
	background = this->background.get();
	platforms.clear();
	monsters.clear();
	keys.clear();
	monsterSet.clear();
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (h_index + i >= 0 && h_index + i < sections.size() && w_index + j >= 0 && w_index + j < sections[h_index + i].size())
			{
				sections[h_index + i][w_index + j]->ExportTo(keys);
				sections[h_index + i][w_index + j]->ExportTo(monsterSet);
			}
		}
	}
	OutputDebugString(ConvertToLPCWSTR("keys: " + std::to_string(keys.size()) + "\n"));
	for (auto& key : keys)
	{
		platforms.push_back(this->platforms[key].get());
	}
	for (auto& monster : monsterSet)
	{
		monsters.push_back(monster);
	}
}

void Level1::InitPlatforms(std::unordered_map<int, std::unique_ptr<StandableObject>> &platforms)
{
	platforms.insert_or_assign(0, std::make_unique<StandableObject>(-100, 480, 10000, 1000, false)); // 0
	platforms.insert_or_assign(1, std::make_unique<StandableObject>(50, 210, 1450, 50, true)); // 1
	platforms.insert_or_assign(2, std::make_unique<StandableObject>(300, 270, 200, 50, true));
	platforms.insert_or_assign(3, std::make_unique<StandableObject>(500, 340, 60, 50, true));
	platforms.insert_or_assign(4, std::make_unique<StandableObject>(560, 400, 130, 50, true));
	platforms.insert_or_assign(5, std::make_unique<StandableObject>(690, 340, 60, 50, true)); // 5
	platforms.insert_or_assign(6, std::make_unique<StandableObject>(800, 270, 140, 50, true));
	platforms.insert_or_assign(7, std::make_unique<StandableObject>(1190, 400, 140, 50, true));
	platforms.insert_or_assign(8, std::make_unique<StandableObject>(1250, 300, 190, 150, true));
	platforms.insert_or_assign(9, std::make_unique<StandableObject>(1750, 210, 320, 150, true));
	platforms.insert_or_assign(10, std::make_unique<StandableObject>(2320, 210, 500, 160, true)); // 10
	platforms.insert_or_assign(11, std::make_unique<StandableObject>(2700, 150, 1000, 250, true));
	platforms.insert_or_assign(12, std::make_unique<StandableObject>(2760, 390, 200, 30, true));
	platforms.insert_or_assign(13, std::make_unique<StandableObject>(2950, 300, 120, 90, true));
	platforms.insert_or_assign(14, std::make_unique<StandableObject>(3130, 270, 400, 80, true));
	platforms.insert_or_assign(15, std::make_unique<StandableObject>(3390, 390, 360, 30, true)); // 15
	platforms.insert_or_assign(16, std::make_unique<StandableObject>(3680, 200, 390, 50, true));
	platforms.insert_or_assign(17, std::make_unique<StandableObject>(3760, 330, 120, 50, true));
	platforms.insert_or_assign(18, std::make_unique<StandableObject>(3950, 330, 120, 50, true));
	platforms.insert_or_assign(19, std::make_unique<StandableObject>(4010, 140, 310, 50, true));
	platforms.insert_or_assign(20, std::make_unique<StandableObject>(4130, 300, 70, 50, true)); // 20
	platforms.insert_or_assign(21, std::make_unique<StandableObject>(4260, 270, 190, 50, true));
	platforms.insert_or_assign(22, std::make_unique<StandableObject>(4380, 210, 120, 50, true));
	platforms.insert_or_assign(23, std::make_unique<StandableObject>(4580, 270, 120, 50, true));
	platforms.insert_or_assign(24, std::make_unique<StandableObject>(4640, 210, 120, 50, true));
	platforms.insert_or_assign(25, std::make_unique<StandableObject>(4830, 270, 120, 50, true)); // 25
	platforms.insert_or_assign(26, std::make_unique<StandableObject>(4830, 390, 60, 50, true));
	platforms.insert_or_assign(27, std::make_unique<StandableObject>(4890, 330, 180, 50, true));
	platforms.insert_or_assign(28, std::make_unique<StandableObject>(5070, 210, 110, 50, true));
	platforms.insert_or_assign(29, std::make_unique<StandableObject>(5140, 390, 60, 50, true));
	platforms.insert_or_assign(30, std::make_unique<StandableObject>(5070, 140, 240, 50, true)); // 30
	platforms.insert_or_assign(31, std::make_unique<StandableObject>(5200, 300, 60, 50, true));
	platforms.insert_or_assign(32, std::make_unique<StandableObject>(5330, 210, 110, 50, true));
	platforms.insert_or_assign(33, std::make_unique<StandableObject>(5390, 270, 310, 50, true));
	platforms.insert_or_assign(34, std::make_unique<StandableObject>(5570, 390, 190, 50, true));
	platforms.insert_or_assign(35, std::make_unique<StandableObject>(5820, 330, 130, 50, true)); // 35
	platforms.insert_or_assign(36, std::make_unique<StandableObject>(6020, 270, 120, 50, true));
	platforms.insert_or_assign(37, std::make_unique<StandableObject>(6140, 210, 250, 50, true));
	platforms.insert_or_assign(38, std::make_unique<StandableObject>(6140, 390, 480, 50, true));
	platforms.insert_or_assign(39, std::make_unique<StandableObject>(6210, 300, 180, 50, true));
	platforms.insert_or_assign(40, std::make_unique<StandableObject>(6390, 270, 70, 50, true)); // 40
	platforms.insert_or_assign(41, std::make_unique<StandableObject>(6460, 330, 70, 50, true));
	platforms.insert_or_assign(42, std::make_unique<StandableObject>(6550, 70, 220, 370, false, false)); // door

	for (auto& platform : platforms)
	{
		int w_index = platform.second->GetPosition().x / sWidth;
		int h_index = platform.second->GetPosition().y / sHeight;
		int w_end = min((platform.second->GetPosition().x + platform.second->GetPlatformWidth()) / sWidth, sections[0].size() - 1);
		int h_end = min((platform.second->GetPosition().y + platform.second->GetPlatformHeight()) / sHeight, sections.size() - 1);
		for (int i = h_index; i <= h_end; i++)
		{
			for (int j = w_index; j <= w_end; j++)
			{
				sections[i][j]->Register(platform.first);
			}
		}
		platformKeys.insert_or_assign(platform.second.get(), platform.first);
	}
}

void Level1::InitMonsters(LPDIRECT3DDEVICE9 m_pDevice3D, std::list<std::unique_ptr<Monster>>& monsters)
{
	monsters.emplace_back(std::make_unique<Runner>(1450.0f, 140.0f, 0.0f, 200.0f, 200.0f));
	monsters.back()->Init(m_pDevice3D, 0.15f);

	monsters.emplace_back(std::make_unique<Shooter>(1000.0f, 140.0f, 0.0f, 0.0f, 0.0f));
	monsters.back()->Init(m_pDevice3D, 0.15f);
	monsters.emplace_back(std::make_unique<Shooter>(900.0f, 140.0f, 0.0f, 0.0f, 0.0f));
	monsters.back()->Init(m_pDevice3D, 0.15f);
	monsters.emplace_back(std::make_unique<Shooter>(1100.0f, 140.0f, 0.0f, 0.0f, 0.0f));
	monsters.back()->Init(m_pDevice3D, 0.15f);
	monsters.emplace_back(std::make_unique<Shooter>(1200.0f, 140.0f, 0.0f, 0.0f, 0.0f));
	monsters.back()->Init(m_pDevice3D, 0.15f);
	monsters.emplace_back(std::make_unique<Shooter>(1300.0f, 140.0f, 0.0f, 0.0f, 0.0f));
	monsters.back()->Init(m_pDevice3D, 0.15f);

	for (auto& monster : monsters)
	{
		monster->SetInvincibilityDelay(0);
	}

	for (auto& monster : monsters)
	{
		int w_index = monster->GetPosition().x / sWidth;
		int h_index = monster->GetPosition().y / sHeight;
		int w_end = min((monster->GetPosition().x + monster->GetSprite()->spriteWidth) / sWidth, sections[0].size() - 1);
		int h_end = min((monster->GetPosition().y + monster->GetSprite()->spriteHeight) / sHeight, sections.size() - 1);
		for (int i = h_index; i <= h_end; i++)
		{
			for (int j = w_index; j <= w_end; j++)
			{
				sections[i][j]->Register(monster.get());
			}
		}
	}
}

void Level1::Update(StandableObject* object)
{
	int w_index = object->GetPosition().x / sWidth;
	int h_index = object->GetPosition().y / sHeight;
	int w_end = min((object->GetPosition().x + object->GetPlatformWidth()) / sWidth, sections[0].size() - 1);
	int h_end = min((object->GetPosition().y + object->GetPlatformHeight()) / sHeight, sections.size() - 1);
	// check if object is in the same sections by comparing the index of start and end section
	if (sections[h_index][w_index]->Contain(platformKeys[object]) && sections[h_end][w_end]->Contain(platformKeys[object])) return;
	// if not, unregister the object from the previous sections and register it to the new sections
	for (int i = 0; i < sections.size(); i++)
	{
		for (int j = 0; j < sections[0].size(); j++)
		{
			sections[i][j]->Unregister(platformKeys[object]);
		}
	}

	for (int i = h_index; i <= h_end; i++)
	{
		for (int j = w_index; j <= w_end; j++)
		{
			sections[i][j]->Register(platformKeys[object]);
		}
	}

}

void Level1::Update(Monster* monster)
{
	int w_index = monster->GetPosition().x / sWidth;
	int h_index = monster->GetPosition().y / sHeight;
	int w_end = min((monster->GetPosition().x + monster->GetSprite()->spriteWidth) / sWidth, sections[0].size() - 1);
	int h_end = min((monster->GetPosition().y + monster->GetSprite()->spriteHeight) / sHeight, sections.size() - 1);
	// check if object is in the same sections by comparing the index of start and end section
	if (sections[h_index][w_index]->Contain(monster) && sections[h_end][w_end]->Contain(monster)) return;
	// if not, unregister the object from the previous sections and register it to the new sections
	for (int i = 0; i < sections.size(); i++)
	{
		for (int j = 0; j < sections[0].size(); j++)
		{	
			sections[i][j]->Unregister(monster);
		}
	}

	for (int i = h_index; i <= h_end; i++)
	{
		for (int j = w_index; j <= w_end; j++)
		{
			sections[i][j]->Register(monster);
		}
	}
}

void Level1::Remove(StandableObject* object)
{
	for (int i = 0; i < sections.size(); i++)
	{
		for (int j = 0; j < sections[0].size(); j++)
		{
			sections[i][j]->Unregister(platformKeys[object]);
		}
	}
	int key = this->platformKeys[object];
	this->platformKeys.erase(object);
	this->platforms.erase(key);
}

void Level1::Remove(Monster* monster)
{
	for (int i = 0; i < sections.size(); i++)
	{
		for (int j = 0; j < sections[0].size(); j++)
		{
			sections[i][j]->Unregister(monster);
		}
	}
	this->monsters.remove_if([monster](std::unique_ptr<Monster>& m) {return m.get() == monster; });
}