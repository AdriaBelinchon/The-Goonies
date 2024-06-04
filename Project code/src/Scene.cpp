#include "Scene.h"
#include <stdio.h>
#include "Globals.h"

Scene::Scene()
{
	currentStage = 1;
	player = nullptr;
    level = nullptr;
	enemies = nullptr;
	font = nullptr;
	
	camera.target = { 0, 0 };				//Center of the screen
	camera.offset = { 0, MARGIN_GUI_Y };	//Offset from the target (center of the screen)
	camera.rotation = 0.0f;					//No rotation
	camera.zoom = 1.0f;						//Default zoom

	debug = DebugMode::OFF;
}
Scene::~Scene()
{
	if (player != nullptr)
	{
		player->Release();
		delete player;
		player = nullptr;
	}
    if (level != nullptr)
    {
		level->Release();
        delete level;
        level = nullptr;
    }
	for (Entity* obj : objects)
	{
		delete obj;
	}
	objects.clear();

	if (enemies != nullptr)
	{
		enemies->Release();
		delete enemies;
		enemies = nullptr;
	}
	if (font != nullptr)
	{
		delete font;
		font = nullptr;
	}
}
AppStatus Scene::Init()
{
	//Create player
	player = new Player({ 0,0 }, State::IDLE, Look::RIGHT);
	if (player == nullptr)
	{
		LOG("Failed to allocate memory for Player");
		return AppStatus::ERROR;
	}
	//Initialise player
	if (player->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Player");
		return AppStatus::ERROR;
	}

	//Create enemy manager
	enemies = new EnemyManager();
	if (enemies == nullptr)
	{
		LOG("Failed to allocate memory for Enemy Manager");
		return AppStatus::ERROR;
	}
	//Initialise enemy manager
	if (enemies->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Enemy Manager");
		return AppStatus::ERROR;
	}

	//Create text font 1
	font = new Text();
	if (font == nullptr)
	{
		LOG("Failed to allocate memory for font");
		return AppStatus::ERROR;
	}
	//Initialise text font 1
	if (font->Initialise(Resource::IMG_FONT, "images/Font.png", ' ', 8) != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}

	//Create level 
    level = new TileMap();
    if (level == nullptr)
    {
        LOG("Failed to allocate memory for Level");
        return AppStatus::ERROR;
    }
	//Initialise level
	if (level->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	//Load level
	if (LoadLevel(1) != AppStatus::OK)
	{
		LOG("Failed to load Level 1");
		return AppStatus::ERROR;
	}
	//Assign the tile map reference to the player to check collisions while navigating
	player->SetTileMap(level);

	enemies->SetTileMap(level);

	

    return AppStatus::OK;
}
AppStatus Scene::LoadLevel(int stage)
{
	int size;
	int x, y, i;
	Tile tile;
	Point pos;
	int *map = nullptr;
	Object *obj;
	AABB hitbox, area;
	
	ClearLevel();

	size = LEVEL_WIDTH * LEVEL_HEIGHT;
	if (stage == 1)
	{
		currentStage = 1;
		map = new int[size] {
		    11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,
			12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,
			11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,
			12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,	 11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,
			25,  14,  15,  16,  17,  14,  15,  16,  17,  14,  15,  16,  17,  14,  15,  16,  17,  15,  14,  16,  17,	 15,  14,  24,  25,  14,  15,  16,  17,  14,  15,  16,
			19,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  18,  19,   0,   0,   0,   0,   0,   0,   0,
			21,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,  20,  21,   0,   0,   0,   0,   0,   0,   0,
			19,   0,   0,   0,   0,   0,   0,   0,  53,  54,  55,   0,   0,  -1,   0,   0,   0,   0,   0,   0,   0,	  0,   0,  18,  19,   0,   0,   0,   0,   0,   0,   0,
			19,   0,   0,   0,   0,   0,   0,   0,  56, 163,  57,   0,   0, 200,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			21,   0,   0,   2,   1,   1,   1,   1,   6,   1,   7,   1,   1,   1,   1,   1,   1,   1,   1,   1,   3,	  0,   0,   0,   0,   0,   0,   2,   1,   1,   1,   1,
			23,   0,   0,  14,  13,  80,  15,  16,  17,  15,  14,  16,  17,  15,  14,  16,  17,  15,  80,  13,  14,	  0,   0,   0,   0,   0,   0,  14,  13,  80,  15,  24,
			19,   0,   0,   0,   0,  81,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  81,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,  81,   0,  20,
			19,   0,   0,   0,   0,  81,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  81,   0,   0,	  0,   0, 112, 113, 114,   0,   0,   0,  81,   0,  18,
			21,   0,   0,   0,   0,  81,   0,   0,   0,   0,  53,  54,  55,   0,   0,   0,  -1,   0,  81,   0,   0,	  0,   0, 115, 116, 117,   0,   0,   0,  81,   0,  20,
			19,   0,   0,   0,   0,  81,   0,   0,   0,   0,  56, 162,  57,   0,   0,   0, 300,   0,  81,   0,   0,	  0, 160, 118, 119, 120,   0, 161,   0,  81,   0,  18,
			27,   1,   1,   1,   1,  82,   1,   1,   1,   1,   6,   1,   7,   1,   1,   1,   1,   1,  82,   1,   1,	  1,   1, 121, 122, 123,   1,   1,   1,  82,   1,  26,
			11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,	 12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,
			12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,	 11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,
			11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,	 12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,
			12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11
		};

		if (player->GetScore() == 0) player->InitScore();
		
	}
	else if (stage == 2)
	{
		currentStage = 2;
		map = new int[size] {
			25,  14,  15,  16,  17,  14,  14,  16,  17,  14,  15,  16,  17,  14,  15,  16,  17,  14,  15,  16,  17,	 14,  15,  16,  17,  14,  15,  16,  17,  14,  15,  16,
			19,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  58,  59,  60,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			21,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  61,  62,  63,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			19,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  53,  54,  55,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 300,   0,   0,  56, 162,  57,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   2,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   6,   1,   7,   1,   1,   1,   1,   1,
			 0,   0,   0,   0,   0,   0,   0,   0,  17,  14,  80,  16,  17,  14,  15,  16,  17,  14,  15,  16,  17,  14,  15,  16,  17,  14,  15,  16,  17,  14,  15,  16,  
			 0,  -1,   0,   0,   0,  -1,   0,   0,   0,   0,  81,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			 0, 200,   0,   0,   0, 300,   0,   0,   0,   0,  81,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			 1,   1,   1,   1,   1,   1,   1,   1,   1,   1,  82,   3,   4,   5,   5,   5,   5,   5,   5,   5,   5,	  5,   5,   5,   5,   5,   5,   4,   2,   1,   1,   1,
			25,  14,  15,  16,  17,  14,  15,  16,  17,  14,  15,  28,  33,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  33,  29,  14,  15,  16,
			19,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			21,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			19,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			19,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			21,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,
			52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,
			51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,
			52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51,  52,  51
		};
	}
	else if (stage == 3)
	{
		currentStage = 3;
		map = new int[size] {
			17,  15,  14,  16,  17,  15,  14,  16,  17,  14,  15,  16,  17,  14,  15,  16,  17,  14,  15,  16,  17,  14,  15,  16,  17,  14,  15,  16,  17,  14,  15,  16,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			 0,  -1,   0,   0,   0,  53,  54,  55,   0,   0,   0,   0,   0,   0,   0,   0,   2,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
			 0, 200,   0,   0,   0,  56,   0,  57,   0,   0,   0,   0,   0,   0,   0,   0,  17,  14,  15,  16,  17,  80,  15,  16,  17,  14,  15,  16,  17,  80,  15,  16,
			 1,   1,   1,   1,   1,   6,   1,   7,   1,   1,   1,   1,   3,   0,   0,   0,   0, 150, 151,   0,   0,  81,   0,   0,   0,   0,   0,   0,   0,  81,   0,   0,
			17,  14,  15,  16,  17,  14,  15,  16,  17,  15,  80,  13,  14,   0,   0,   0,   0,  39,  40,   0,   0,  81,   0,   0,   0,   0,   0,   0,   0,  81,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  81,   0,   0,   0,   0,   0,   0,  41,  42,   0,   0,  81,   0,   0,   0,   0,   0,   0,   0,  81,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  81, 300,   0,   0,   0,   0,   0,  43,  44,   0,   0,  81,   0,   0,   0,   0,   0,   0,   0,  81,   0,   0,
			 1,   1,   1,   1,   1,   1,   1,   1,   1,   1,  82,   1,   1,   1,   1,   3,   0,   0,   0,   2,   1,  82,   1,   1,   1,   1,   1,   1,   1,  82,   1,   1,
			25,  14,  15,  16,  17,  14,  15,  16,  17,  15,  14,  16,  17,  80,  14,  16,   0,   0,   0,  14,  13,  14,  15,  16,  17,  15,  80,  16,  17,  15,  14,  16,
			19,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  81,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  81,   0,   0,   0,   0,   0,
			19,   0,   0, 112, 113, 114,   0,   0,   0,   0,   0,   0,   0,  81,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  81,   0,   0,   0,   0,   0,
			21,   0,   0, 115, 116, 117,   0,   0,   0,   0,   0,   0,   0,  81,   0,   0,   0,   0,   0,  53,  54,  55,   0,   0,   0,   0,  81,   0,   0,   0,   0,   0,
			19,   0, 160, 118, 119, 120,   0,   0,   0, 300,   0,   0,   0,  81,   0,   0,   0,   0,   0,  56, 162,  57, 300,   0,   0,   0,  81,   0,   0,   0,   0,   0,
			27,   1,   1, 121, 122, 123,   1,   1,   1,   1,   1,   1,   1,  82,   1,   1,   1,   1,   1,   6,   1,   7,   1,   1,   1,   1,  82,   1,   1,   1,   1,   1,
			11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,
			12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,
			11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,
			12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11
		};
	}
	else if (stage == 4)
	{
		currentStage = 4;
		map = new int[size] {
			17,  15,  14,  16,  17,  15,  14,  16,  17,  14,  15,  16,  17,  14,  15,  16,  17,  14,  15,  16,  17,  14,  15,  16,  17,  15,  14,  16,  17,  15,  14,  24,   
			 0,  -1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  20,
			 0, 200,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 140, 141, 142,   0,   0,   0,   0,   0,   0,   0,   0, 140, 141, 142,   0,   0,   0,   0,  18,
			 1,   1,   1,   1,   1,   1,   1,   3,   0,   0,   0,   0,   0, 143, 144, 145,   0,   0,   0,   0,   0,   0,   0,   0, 143, 144, 145,   0,   0,   0,   0,  20,
			17,  15,  80,  16,  17,  15,  14,  16,   0,   0,   0,   0,   0, 146,   0, 147,   0, 300,   0,   0,   0,   0,   0,   0, 146,   0, 147,   0,   0,   0,   0,  18,
			 0,   0,  81,   0,   0,   0,   0,   0,   0,   0,   0,   2,   1, 148,   1, 149,   1,   1,   1,   1,   1,   1,   1,   1, 148,   1, 149,   1,   3,   0,   0,  20,
			 0,   0,  81,   0,   0,   0,   0,   0,   0,   0,   0,  14,  13,  14,  15,  16,  17,  14,  80,  16,  17,  14,  15,  16,  17,  15,  14,  13,  14,   0,   0,  22,
			 0,   0,  81,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  81,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  20,
			 0,   0,  81,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  81,   0,   0,   0,   0,   0,   0,   0,   0,   0, 112, 113, 114,  18,
			 1,   1,  82,   1,   1,   1,   1,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  81,   0,   0,   0,   0,   0,   0,   0,   0, 160, 115, 116, 117,  20,
			17,  15,  14,  16,  17,  80,  14,  16,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  81, 300,   0,   0,   0,   0,   0,   0,   0, 160, 118, 119, 120,  18,
			 0,   0,   0,   0,   0,  81,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   1,  82,   1,   1,   1,   1,   1,   1,   1,   1,   1, 121, 122, 123,  26,
			 0,   0,   0,   0,   0,  81,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  17,  15,  14,  16,  17,  80,  14,  24,  11,  12,  11,  12,  11,  12,  11,  12,
			 0,   0,   0,   0,   0,  81,   0,   0,  53,  54,  55,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  81,   0,  20,  12,  11,  12,  11,  12,  11,  12,  11,
			 0,   0,   0,   0,   0,  81,   0,   0,  56, 162,  57, 300,   0,   0,   0,   0,   0,   0,   0,   0,   0,  81,   0,  18,  11,  12,  11,  12,  11,  12,  11,  12,
			 1,   1,   1,   1,   1,  82,   1,   1,   6,   1,   7,   1,   1,   1,   1,   3,   0,   0,   0,   0,   0,  81,   0,  20,  12,  11,  12,  11,  12,  11,  12,  11,
			25,  14,  15,  16,  17,  14,  15,  16,  17,  15,  14,  16,  17,  80,  14,  16,   0,   0,   0,   0,   0,  81,   0,  18,  11,  12,  11,  12,  11,  12,  11,  12,
			19,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  81,   0,   0,   0,   0,   0,   0,   0,  81,   0,  20,  12,  11,  12,  11,  12,  11,  12,  11,
			21,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  81,   0,   0,   0,   0,   0,   0,   0,  81,   0,  18,  11,  12,  11,  12,  11,  12,  11,  12,
			27,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,  82,   1,   1,   1,   1,   1,   1,   1,  82,   1,  26,  12,  11,  12,  11,  12,  11,  12,  11,
			
			

		};
	}

	else
	{
		//Error level doesn't exist or incorrect level number
		LOG("Failed to load level, stage %d doesn't exist", stage);
		return AppStatus::ERROR;	
	}

	//Entities and objects
	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)map[i];
			if (tile == Tile::EMPTY)
			{
				map[i] = 0;
			}
			else if (tile == Tile::PLAYER)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				player->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::ITEM_POTION)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE;
				obj = new Object(pos, ObjectType::POTION);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::ITEM_KEY)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE;
				obj = new Object(pos, ObjectType::KEY);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::ITEM_BAG)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE;
				obj = new Object(pos, ObjectType::BAG);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::SKULL)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				hitbox = enemies->GetEnemyHitBox(pos, EnemyType::SKULL);
				area = level->GetSweptAreaX(hitbox);
				enemies->Add(pos, EnemyType::SKULL, area);
			}
			++i;
		}

		
	}
	//Tile map
	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);

	return AppStatus::OK;
}
void Scene::Update()
{
	Point p1, p2;
	AABB box;

	//Switch between the different debug modes: off, on (sprites & hitboxes), on (hitboxes) 
	if (IsKeyPressed(KEY_F1))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);
	}
	//Debug levels instantly
	if (IsKeyPressed(KEY_ONE))	LoadLevel(1);
	else if (IsKeyPressed(KEY_TWO))   LoadLevel(2);
	else if (IsKeyPressed(KEY_THREE))	LoadLevel(3);
	else if (IsKeyPressed(KEY_FOUR))	LoadLevel(4);

	level->Update();
	player->Update();
	enemies->Update(box);
	CheckCollisions();
	UpdateBackground(currentStage);


}
void Scene::UpdateBackground(int s)
{
	int x = player->GetPosX();
	int y = player->GetPosY();
	switch (s)
	{
	case 1:

		if (x + PLAYER_PHYSICAL_WIDTH >= LEVEL_WIDTH * TILE_SIZE - 6) {
			
			LoadLevel(2);
			player->SetPos({ 8, y });
			break;
		}
	case 2:
		if (x < 0) {
			
			LoadLevel(1);
			player->SetPos({ (LEVEL_WIDTH * TILE_SIZE) - 8, y });
			break;
		}
		else if (x + PLAYER_PHYSICAL_WIDTH >= LEVEL_WIDTH * TILE_SIZE - 6) {
			
			LoadLevel(3);
			player->SetPos({ 8, y });
			break;
		}
	case 3:
		if (x < 0) {
			
			LoadLevel(2);
			player->SetPos({ (LEVEL_WIDTH * TILE_SIZE) - 8, y });
			break;
		}
		else if (x + PLAYER_PHYSICAL_WIDTH >= LEVEL_WIDTH * TILE_SIZE - 6) {
			
			LoadLevel(4);
			player->SetPos({ 8, y });
			break;
		}
	case 4:
		if (x < 0) {
			
			LoadLevel(3);
			player->SetPos({ (LEVEL_WIDTH * TILE_SIZE) - 8, y });
			break;
		}
	}
}
void Scene::Render()
{
	BeginMode2D(camera);

    level->Render();
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		RenderObjects(); 
		enemies->Draw();
		player->DrawP();
	}
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		player->DrawDebug(GREEN);
		enemies->DrawDebug();
	}

	EndMode2D();

	RenderGUI();
}
void Scene::Release()
{
    level->Release();
	player->Release();
	ClearLevel();
}
void Scene::CheckCollisions()
{
	AABB player_box, obj_box, en_box;
	
	player_box = player->GetHitbox();
	auto it = objects.begin();

	
	while (it != objects.end())
	{
		obj_box = (*it)->GetHitbox();
		if(player_box.TestAABB(obj_box))
		{
			
			(*it)->SoundItem();
			player->IncrScore((*it)->Points());
			player->ModExperience((*it)->Experience());
			player->ModVitality((*it)->Vitality());
			//Delete the object
			delete* it; 
			//Erase the object from the vector and get the iterator to the next valid element
			it = objects.erase(it); 
		}
		else
		{
			//Move to the next object
			++it; 
		}
	}
}
void Scene::ClearLevel()
{
	for (Object* obj : objects)
	{
		delete obj;
	}
	objects.clear();
	enemies->Release();
}
void Scene::RenderObjects() const
{
	for (Object* obj : objects)
	{
		obj->Draw();
	}
}
void Scene::RenderObjectsDebug(const Color& col) const
{
	for (Object* obj : objects)
	{
		obj->DrawDebug(col);
	}
}
void Scene::RenderGUI() const
{
	//Temporal approach
	
	font->Draw(7, 0, TextFormat("HI-%d", player->GetScore()));
	font->Draw(7, 8, TextFormat("1P-%d", player->GetScore()));
	font->Draw(90, 0, TextFormat("VIT-%d", player->GetVitality()));
	font->Draw(90, 8, TextFormat("EXP-%d", player->GetExperience()));
	font->Draw(208, 0, TextFormat("SCENE"));
	font->Draw(208, 8, TextFormat("01-"));

	if (currentStage == 1) font->Draw(232, 8, TextFormat("01"));
	if (currentStage == 2) font->Draw(232, 8, TextFormat("02"));
	if (currentStage == 3) font->Draw(232, 8, TextFormat("03"));
	if (currentStage == 4) font->Draw(232, 8, TextFormat("04"));
	
	/*DrawText(TextFormat("SCORE - %d", player->GetScore()), 10, 5, 8, LIGHTGRAY);*/
	/*DrawText(TextFormat("VIT - %d", player->GetVitality()), 180, 5, 8, BLUE);
	DrawText(TextFormat("EXP - %d", player->GetExperience()), 180, 15, 8, GREEN);*/
}
