#include "EnemyManager.h"
#include "Skull.h"
#include "Stalactite.h"
#include "Drop.h"
#include "Yellow_Fratelli.h"

EnemyManager::EnemyManager()
{
	shots = nullptr;
}
EnemyManager::~EnemyManager()
{
	Release();
}
AppStatus EnemyManager::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ENEMIES, "images/Enemies.png") != AppStatus::OK)
	{
		LOG("Failed to load enemies sprite texture");
		return AppStatus::ERROR;
	}

	return AppStatus::OK;
}
void EnemyManager::SetShotManager(ShotManager* shots)
{
	this->shots = shots;
}
void EnemyManager::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
void EnemyManager::Add(const Point& pos, EnemyType type, const AABB& area, Look look)
{
	Enemy* enemy;

	if (type == EnemyType::SKULL)
	{
		enemy = new Skull(pos, SKULL_PHYSICAL_WIDTH, SKULL_PHYSICAL_HEIGHT, SKULL_FRAME_SIZE, SKULL_FRAME_SIZE);
	}
	/*else if (type == EnemyType::STALACTITE)
	{
		enemy = new Stalactite(pos, STAL_PHYSICAL_WIDTH, STAL_PHYSICAL_HEIGHT, STAL_FRAME_SIZE, STAL_FRAME_SIZE);
	}
	else if (type == EnemyType::DROP)
	{
		enemy = new Drop(pos, DROP_PHYSICAL_WIDTH, DROP_PHYSICAL_HEIGHT, DROP_FRAME_SIZE, DROP_FRAME_SIZE);
	}*/
	else
	{
		LOG("Internal error: trying to add a new enemy with invalid type");
		return;
	}

	enemy->Initialise(look, area);
	enemies.push_back(enemy);
}
AABB EnemyManager::GetEnemyHitBox(const Point& pos, EnemyType type) const
{
	int width, height;
	if (type == EnemyType::SKULL)
	{
		width = SKULL_PHYSICAL_WIDTH;
		height = SKULL_PHYSICAL_HEIGHT;
	}
	/*else if (type == EnemyType::TURRET)
	{
		width = TURRET_PHYSICAL_WIDTH;
		height = TURRET_PHYSICAL_HEIGHT;
	}*/
	else
	{
		LOG("Internal error while computing hitbox for an invalid enemy type");
		return {};
	}
	Point p(pos.x, pos.y - (height - 1));
	AABB hitbox(p, width, height);
	return hitbox;
}
void EnemyManager::Update(const AABB& player_hitbox)
{
	bool shoot;
	Point p, d;

	for (Enemy* enemy : enemies)
	{
		shoot = enemy->Update(player_hitbox);
		/*if (shoot)
		{
			enemy->GetShootingPosDir(&p, &d);
			shots->Add(p, d);
		}*/
	}
}
void EnemyManager::Draw() const
{
	for (const Enemy* enemy : enemies)
		enemy->Draw();
}
void EnemyManager::DrawDebug() const
{
	for (const Enemy* enemy : enemies)
	{
		enemy->DrawVisibilityArea(DARKGRAY);
		enemy->DrawHitbox(RED);
	}
}
void EnemyManager::Release()
{
	for (Enemy* enemy : enemies)
		delete enemy;
	enemies.clear();
}