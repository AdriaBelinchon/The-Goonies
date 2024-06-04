#pragma once
#include <raylib.h>
#include "Sprite.h"
#include "Point.h"
#include "AABB.h"
#include "Globals.h"
#include <unordered_map>

enum class Tile {

	// -1: area covered by an entity
	EMPTY = -1,
	//  0: air tile
	AIR = 0,

	// 0 < id < 100: static tiles
	FLOOR = 1, FLOOR_L, FLOOR_R, FLOOR_GRASS_CONNECT, FLOOR_GRASS, 
	FLOOR_DOOR_STONE_L, FLOOR_DOOR_STONE_R, 
	
	BLOCK_SQUARE1 = 11, BLOCK_SQUARE2, STALACTITES1, STALACTITES2, STALACTITES3,
	STALACTITE_BIG_R, STALACTITE_BIG_L, VERT1_R, VERT1_L, VERT2_R, VERT2_L, VERT3_R, VERT3_L,
	VERT_TOP_R, VERT_TOP_L, VERT_BOTTOM_R, VERT_BOTTOM_L, STALACTITE_LITTLE_L, STALACTITE_LITTLE_R,
	BEACH1, BEACH2, ROOF_GRASS_CONNECT, ROOF_GRASS, TUB_HOR, TUB_VER, TUB_VER2, TUB_CROSS, FLOOR_TUB,
	TOP_ROCK_L, TOP_ROCK_R, MIDDLE_ROCK_L, MIDDLE_ROCK_R, BOTTOM_ROCK_L, BOTTOM_ROCK_R,

	WATER1 = 51, WATER2, TOP_DOOR_STONE_L, TOP_DOOR_STONE_M, TOP_DOOR_STONE_R, MIDDLE_DOOR_STONE_L, MIDDLE_DOOR_STONE_R,
	TOP_APERTURE_L, TOP_APERTURE_M, TOP_APERTURE_R, BOTTOM_APERTURE_L, BOTTOM_APERTURE_M, BOTTOM_APERTURE_R,
	
	TOP_LADDER = 80, MIDDLE_LADDER, FLOOR_LADDER,

	// 100 <= id < 200: special tiles
	TOP_DOOR_BLUE_CLOSE_L = 100, TOP_DOOR_BLUE_CLOSE_M, TOP_DOOR_BLUE_CLOSE_R, PRETOP_DOOR_BLUE_CLOSE_L, PRETOP_DOOR_BLUE_CLOSE_M,
	PRETOP_DOOR_BLUE_CLOSE_R, PREBOTTOM_DOOR_BLUE_CLOSE_L, PREBOTTOM_DOOR_BLUE_CLOSE_M, PREBOTTOM_DOOR_BLUE_CLOSE_R,
	FLOOR_DOOR_BLUE_CLOSE_L, FLOOR_DOOR_BLUE_CLOSE_M, FLOOR_DOOR_BLUE_CLOSE_R,

	TOP_DOOR_GREEN_CLOSE_L, TOP_DOOR_GREEN_CLOSE_M, TOP_DOOR_GREEN_CLOSE_R, PRETOP_DOOR_GREEN_CLOSE_L, PRETOP_DOOR_GREEN_CLOSE_M, PRETOP_DOOR_GREEN_CLOSE_R,
	PREBOTTOM_DOOR_GREEN_CLOSE_L, PREBOTTOM_DOOR_GREEN_CLOSE_M, PREBOTTOM_DOOR_GREEN_CLOSE_R, FLOOR_DOOR_GREEN_CLOSED_L, FLOOR_DOOR_GREEN_CLOSED_M, FLOOR_DOOR_GREEN_CLOSED_R,
	
	TOP_DOOR_BLUE_OPEN_L, TOP_DOOR_BLUE_OPEN_M, TOP_DOOR_BLUE_OPEN_R, PRETOP_DOOR_BLUE_OPEN_L, PRETOP_DOOR_BLUE_OPEN_R,
	PREBOTTOM_DOOR_BLUE_OPEN_L, PREBOTTOM_DOOR_BLUE_OPEN_R, FLOOR_DOOR_BLUE_OPEN_L, FLOOR_DOOR_BLUE_OPEN_R,
	
	PRETOP_DOOR_GREEN_OPEN_L, PRETOP_DOOR_GREEN_OPEN_M, PRETOP_DOOR_GREEN_OPEN_R, 
	PREBOTTOM_DOOR_GREEN_OPEN_L, PREBOTTOM_DOOR_GREEN_OPEN_R, FLOOR_DOOR_GREEN_OPEN_L, FLOOR_DOOR_GREEN_OPEN_R,
	
	TOP_DOOR_SKULL_L, TOP_DOOR_SKULL_M, TOP_DOOR_SKULL_R, PRETOP_DOOR_SKULL_L, PRETOP_DOOR_SKULL_M, PRETOP_DOOR_SKULL_R,
	PREBOTTOM_DOOR_SKULL_L, PREBOTTOM_DOOR_SKULL_R, FLOOR_DOOR_SKULL_L, FLOOR_DOOR_SKULL_R,

	ROCK_CHAIN_L = 150, ROCK_CHAIN_R, 
	STALACTITE_FALL_L = 160, STALACTITE_FALL_R, FLOOR_STALACTITE_FALL_L, FLOOR_STALACTITE_FALL_R,

	LOCK_KEY = 160, ITEM_POTION, ITEM_KEY, ITEM_BAG,
	FLOOR_DROP = 170,
	TOP_WATERFALL_L = 180, TOP_WATERFALL_M, TOP_WATERFALL_R, PRETOP_WATERFALL_L, PRETOP_WATERFALL_M, PRETOP_WATERFALL_R, 
	BOTTOM_WATERFALL_L, BOTTOM_WATERFALL_M, BOTTOM_WATERFALL_R, FALL_WATERFALL_L, FALL_WATERFALL_M, FALL_WATERFALL_R,



	

	// id >= 200: entities' initial locations
	PLAYER = 200,
	SKULL = 300,

	//Intervals
	STATIC_FIRST = FLOOR,
	STATIC_LAST = BOTTOM_APERTURE_R,
	SOLID_FIRST = BLOCK_SQUARE1,
	SOLID_LAST = BOTTOM_ROCK_R,
	SPECIAL_FIRST = TOP_LADDER,
	SPECIAL_LAST = FALL_WATERFALL_R,
	OBJECT_FIRST = LOCK_KEY,
	OBJECT_LAST = ITEM_BAG,
	ENTITY_FIRST = PLAYER,
	ENTITY_LAST = SKULL
};

class TileMap
{
public:
	TileMap();
	~TileMap();

	AppStatus Initialise();
	AppStatus Load(int data[], int w, int h);
	void ClearObjectEntityPositions();

	void Update();
	void Render();
	void Release();

	bool IsTileObject(Tile tile) const;
	bool IsTileEntity(Tile tile) const;

	//Test for collisions with walls
	bool TestCollisionWallLeft(const AABB& box) const;
	bool TestCollisionWallRight(const AABB& box) const;
	
	//Test collision with the ground and update 'py' with the maximum y-position to prevent
	//penetration of the grounded tile, that is, the pixel y-position above the grounded tile.
	//Grounded tile = solid tile (blocks) or ladder tops.
	bool TestCollisionGround(const AABB& box, int *py) const;
	
	//Test if there is a ground tile one pixel below the given box
	bool TestFalling(const AABB& box) const;
	
	//Test if box is on ladder and update 'px' with the x-center position of the ladder
	bool TestOnLadder(const AABB& box, int* px) const;
	
	//Test if box is on ladder top and update 'px' with the x-center position of the ladder
	bool TestOnLadderTop(const AABB& box, int* px) const;

	bool TestOnLadderBottom(const AABB& box, int* px) const;

	//Given a hitbox, computes the maximum swept box model along the X-axis without solid tiles
	AABB GetSweptAreaX(const AABB& hitboxbox) const;

private:
	void InitTileDictionary();

	Tile GetTileIndex(int x, int y) const;
	bool IsTileSolid(Tile tile) const;
	bool IsTileLadderTop(Tile tile) const;
	bool IsTileLadder(Tile tile) const;
	bool IsTileLadderBottom(Tile tile) const;
	bool CollisionX(const Point& p, int distance) const;
	bool CollisionY(const Point& p, int distance) const;
	int GetLadderCenterPos(int pixel_x, int pixel_y) const;

	//Tile map
	Tile *map;

	//Size of the tile map
	int size, width, height;
	
	//Dictionary of tile frames
	std::unordered_map<int, Rectangle> dict_rect;

	/*Sprite *laser;*/
	
	//Tile sheet
	const Texture2D *img_tiles;
};

