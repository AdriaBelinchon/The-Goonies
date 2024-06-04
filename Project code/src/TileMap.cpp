#include "TileMap.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <cstring>

TileMap::TileMap()
{
	map = nullptr;
	width = 0;
	height = 0;
	/*laser = nullptr;*/
	img_tiles = nullptr;

	InitTileDictionary();
}
TileMap::~TileMap()
{
	if (map != nullptr)
	{
		delete[] map;
		map = nullptr;
	}
	//if (laser != nullptr)
	//{
	//	laser->Release();
	//	delete laser;
	//	laser = nullptr;
	//}
}
void TileMap::InitTileDictionary()
{
	const int n = TILE_SIZE;

	dict_rect[(int)Tile::FLOOR] = { 0,  0, n, n };
	dict_rect[(int)Tile::FLOOR_L] = { n,  0, n, n };
	dict_rect[(int)Tile::FLOOR_R] = { 2 * n, 0, n, n };
	dict_rect[(int)Tile::FLOOR_GRASS_CONNECT] = { 3 * n, 0, n, n };
	dict_rect[(int)Tile::FLOOR_GRASS] = { 4 * n,  0, n, n };
	dict_rect[(int)Tile::FLOOR_DOOR_STONE_L] = { 5 * n, 0, n, n };
	dict_rect[(int)Tile::FLOOR_DOOR_STONE_R] = { 6 * n, 0, n, n };
	dict_rect[(int)Tile::FLOOR_LADDER] = { 7 * n, 0, n, n };
	dict_rect[(int)Tile::FLOOR_DOOR_SKULL_L] = { 8 * n, 0, n, n };
	dict_rect[(int)Tile::FLOOR_DOOR_SKULL_R] = { 9 * n, 0, n, n };
	dict_rect[(int)Tile::BLOCK_SQUARE1] = { 10 * n, 0, n, n };
	dict_rect[(int)Tile::BLOCK_SQUARE2] = { 11 * n, 0, n, n };
	dict_rect[(int)Tile::STALACTITES1] = { 12 * n, 0, n, n };
	dict_rect[(int)Tile::STALACTITES2] = { 13 * n, 0, n, n };
	dict_rect[(int)Tile::STALACTITES3] = { 14 * n, 0, n, n };
	dict_rect[(int)Tile::STALACTITE_BIG_L] = { 15 * n, 0, n, n };
	dict_rect[(int)Tile::STALACTITE_BIG_R] = { 16 * n, 0, n, n };
	dict_rect[(int)Tile::VERT1_L] = { 17 * n, 0, n, n };
	dict_rect[(int)Tile::VERT1_R] = { 18 * n, 0, n, n };
	dict_rect[(int)Tile::VERT2_L] = { 19 * n, 0, n, n };
	dict_rect[(int)Tile::VERT2_R] = { 20 * n, 0, n, n };
	dict_rect[(int)Tile::VERT3_L] = { 21 * n, 0, n, n };
	dict_rect[(int)Tile::VERT3_R] = { 22 * n, 0, n, n };
	dict_rect[(int)Tile::VERT_TOP_L] = { 23 * n, 0, n, n };
	dict_rect[(int)Tile::VERT_TOP_R] = { 24 * n, 0, n, n };
	dict_rect[(int)Tile::VERT_BOTTOM_L] = { 25 * n, 0, n, n };
	dict_rect[(int)Tile::VERT_BOTTOM_R] = { 26 * n, 0, n, n };
	dict_rect[(int)Tile::STALACTITE_LITTLE_L] = { 27 * n, 0, n, n };
	dict_rect[(int)Tile::STALACTITE_LITTLE_R] = { 28 * n, 0, n, n };

	dict_rect[(int)Tile::BEACH1] = { 0, n, n, n };
	dict_rect[(int)Tile::BEACH2] = { n, n, n, n };
	dict_rect[(int)Tile::ROOF_GRASS_CONNECT] = { 2 * n, n, n, n };
	dict_rect[(int)Tile::ROOF_GRASS] = { 3 * n, n, n, n };
	dict_rect[(int)Tile::TUB_HOR] = { 4 * n, n, n, n };
	dict_rect[(int)Tile::TUB_VER] = { 5 * n, n, n, n };
	dict_rect[(int)Tile::TUB_VER2] = { 6 * n, n, n, n };
	dict_rect[(int)Tile::TUB_CROSS] = { 7 * n, n, n, n };
	dict_rect[(int)Tile::FLOOR_TUB] = { 8 * n, n, n, n };
	dict_rect[(int)Tile::FLOOR_DOOR_BLUE_CLOSE_L] = { 9 * n, n, n, n };
	dict_rect[(int)Tile::FLOOR_DOOR_BLUE_CLOSE_M] = { 10 * n, n, n, n };
	dict_rect[(int)Tile::FLOOR_DOOR_BLUE_CLOSE_R] = { 11 * n, n, n, n };
	dict_rect[(int)Tile::FLOOR_DOOR_BLUE_OPEN_L] = { 12 * n, n, n, n };
	dict_rect[(int)Tile::FLOOR_DOOR_BLUE_OPEN_R] = { 13 * n, n, n, n };
	dict_rect[(int)Tile::FLOOR_DOOR_GREEN_OPEN_L] = { 14 * n, n, n, n };
	dict_rect[(int)Tile::FLOOR_DOOR_GREEN_CLOSED_L] = { 15 * n, n, n, n };
	dict_rect[(int)Tile::FLOOR_DOOR_GREEN_CLOSED_M] = { 16 * n, n, n, n };
	dict_rect[(int)Tile::FLOOR_DOOR_GREEN_CLOSED_R] = { 17 * n, n, n, n };
	dict_rect[(int)Tile::FLOOR_DOOR_GREEN_OPEN_R] = { 18 * n, n, n, n };
	
	dict_rect[(int)Tile::WATER1] = { 0, 2 * n, n, n };
	dict_rect[(int)Tile::WATER2] = { n, 2 * n, n, n };
	dict_rect[(int)Tile::TOP_DOOR_STONE_L] = { 2 * n, 2 * n, n, n };
	dict_rect[(int)Tile::TOP_DOOR_STONE_M] = { 3 * n, 2 * n, n, n };
	dict_rect[(int)Tile::TOP_DOOR_STONE_R] = { 4 * n, 2 * n, n, n };
	dict_rect[(int)Tile::MIDDLE_DOOR_STONE_L] = { 5 * n, 2 * n, n, n };
	dict_rect[(int)Tile::MIDDLE_DOOR_STONE_R] = { 6 * n, 2 * n, n, n };
	dict_rect[(int)Tile::TOP_APERTURE_L] = { 7 * n, 2 * n, n, n };
	dict_rect[(int)Tile::TOP_APERTURE_M] = { 8 * n, 2 * n, n, n };
	dict_rect[(int)Tile::TOP_APERTURE_R] = { 9 * n, 2 * n, n, n };
	dict_rect[(int)Tile::BOTTOM_APERTURE_L] = { 10 * n, 2 * n, n, n };
	dict_rect[(int)Tile::BOTTOM_APERTURE_M] = { 11 * n, 2 * n, n, n };
	dict_rect[(int)Tile::BOTTOM_APERTURE_R] = { 12 * n, 2 * n, n, n };
	dict_rect[(int)Tile::TOP_DOOR_SKULL_L] = { 13 * n, 2 * n, n, n };
	dict_rect[(int)Tile::TOP_DOOR_SKULL_M] = { 14 * n, 2 * n, n, n };
	dict_rect[(int)Tile::TOP_DOOR_SKULL_R] = { 15 * n, 2 * n, n, n };
	dict_rect[(int)Tile::PRETOP_DOOR_SKULL_L] = { 16 * n, 2 * n, n, n };
	dict_rect[(int)Tile::PRETOP_DOOR_SKULL_M] = { 17 * n, 2 * n, n, n };
	dict_rect[(int)Tile::PRETOP_DOOR_SKULL_R] = { 18 * n, 2 * n, n, n };
	dict_rect[(int)Tile::PREBOTTOM_DOOR_SKULL_L] = { 19 * n, 2 * n, n, n };
	dict_rect[(int)Tile::PREBOTTOM_DOOR_SKULL_R] = { 20 * n, 2 * n, n, n };

	dict_rect[(int)Tile::TOP_LADDER] = { 21 * n, 2 * n, n, n };
	dict_rect[(int)Tile::MIDDLE_LADDER] = { 22 * n, 2 * n, n, n };
	/*dict_rect[(int)Tile::LADDER_L] = { 2 * n, 2 * n, n, n };
	dict_rect[(int)Tile::LADDER_R] = { 3 * n, 2 * n, n, n };
	dict_rect[(int)Tile::LADDER_TOP_L] = { 4 * n, 2 * n, n, n };
	dict_rect[(int)Tile::LADDER_TOP_R] = { 5 * n, 2 * n, n, n };*/

	dict_rect[(int)Tile::TOP_DOOR_BLUE_CLOSE_L] = { 0, 3 * n, n, n };
	dict_rect[(int)Tile::TOP_DOOR_BLUE_CLOSE_M] = { n, 3 * n, n, n };
	dict_rect[(int)Tile::TOP_DOOR_BLUE_CLOSE_R] = { 2 * n, 3 * n, n, n };
	dict_rect[(int)Tile::PRETOP_DOOR_BLUE_CLOSE_L] = { 3 * n, 3 * n, n, n };
	dict_rect[(int)Tile::PRETOP_DOOR_BLUE_CLOSE_M] = { 4 * n, 3 * n, n, n };
	dict_rect[(int)Tile::PRETOP_DOOR_BLUE_CLOSE_R] = { 5 * n, 3 * n, n, n };
	dict_rect[(int)Tile::PREBOTTOM_DOOR_BLUE_CLOSE_L] = { 6 * n, 3 * n, n, n };
	dict_rect[(int)Tile::PREBOTTOM_DOOR_BLUE_CLOSE_M] = { 7 * n, 3 * n, n, n };
	dict_rect[(int)Tile::PREBOTTOM_DOOR_BLUE_CLOSE_R] = { 8 * n, 3 * n, n, n };
	dict_rect[(int)Tile::TOP_DOOR_GREEN_CLOSE_L] = { 9 * n, 3 * n, n, n };
	dict_rect[(int)Tile::TOP_DOOR_GREEN_CLOSE_M] = { 10 * n, 3 * n, n, n };
	dict_rect[(int)Tile::TOP_DOOR_GREEN_CLOSE_R] = { 11 * n, 3 * n, n, n };
	dict_rect[(int)Tile::PRETOP_DOOR_GREEN_CLOSE_L] = { 12 * n, 3 * n, n, n };
	dict_rect[(int)Tile::PRETOP_DOOR_GREEN_CLOSE_M] = { 13 * n, 3 * n, n, n };
	dict_rect[(int)Tile::PRETOP_DOOR_GREEN_CLOSE_R] = { 14 * n, 3 * n, n, n };
	dict_rect[(int)Tile::PREBOTTOM_DOOR_GREEN_CLOSE_L] = { 15 * n, 3 * n, n, n };
	dict_rect[(int)Tile::PREBOTTOM_DOOR_GREEN_CLOSE_M] = { 16 * n, 3 * n, n, n };
	dict_rect[(int)Tile::PREBOTTOM_DOOR_GREEN_CLOSE_R] = { 17 * n, 3 * n, n, n };

	dict_rect[(int)Tile::TOP_DOOR_BLUE_OPEN_L] = { 0, 4 * n, n, n };
	dict_rect[(int)Tile::TOP_DOOR_BLUE_OPEN_M] = { n, 4 * n, n, n };
	dict_rect[(int)Tile::TOP_DOOR_BLUE_OPEN_R] = { 2 * n, 4 * n, n, n };
	dict_rect[(int)Tile::PRETOP_DOOR_BLUE_OPEN_L] = { 3 * n, 4 * n, n, n };
	dict_rect[(int)Tile::PRETOP_DOOR_BLUE_OPEN_R] = { 4 * n, 4 * n, n, n };
	dict_rect[(int)Tile::PREBOTTOM_DOOR_BLUE_OPEN_L] = { 5 * n, 4 * n, n, n };
	dict_rect[(int)Tile::PREBOTTOM_DOOR_BLUE_OPEN_R] = { 6 * n, 4 * n, n, n };
	dict_rect[(int)Tile::PRETOP_DOOR_GREEN_OPEN_L] = { 7 * n, 4 * n, n, n };
	dict_rect[(int)Tile::PRETOP_DOOR_GREEN_OPEN_M] = { 8 * n, 4 * n, n, n };
	dict_rect[(int)Tile::PRETOP_DOOR_GREEN_OPEN_R] = { 9 * n, 4 * n, n, n };
	dict_rect[(int)Tile::PREBOTTOM_DOOR_GREEN_OPEN_L] = { 10 * n, 4 * n, n, n };
	dict_rect[(int)Tile::PREBOTTOM_DOOR_GREEN_OPEN_R] = { 11 * n, 4 * n, n, n };
	dict_rect[(int)Tile::ROCK_CHAIN_L] = { 12 * n, 4 * n, n, n };
	dict_rect[(int)Tile::ROCK_CHAIN_R] = { 13 * n, 4 * n, n, n };
	dict_rect[(int)Tile::TOP_ROCK_L] = { 14 * n, 4 * n, n, n };
	dict_rect[(int)Tile::TOP_ROCK_R] = { 15 * n, 4 * n, n, n };
	dict_rect[(int)Tile::MIDDLE_ROCK_L] = { 16 * n, 4 * n, n, n };
	dict_rect[(int)Tile::MIDDLE_ROCK_R] = { 17 * n, 4 * n, n, n };
	dict_rect[(int)Tile::BOTTOM_ROCK_L] = { 18 * n, 4 * n, n, n };
	dict_rect[(int)Tile::BOTTOM_ROCK_R] = { 19 * n, 4 * n, n, n };
	dict_rect[(int)Tile::STALACTITE_FALL_L] = { 20 * n, 4 * n, n, n };
	dict_rect[(int)Tile::STALACTITE_FALL_R] = { 21 * n, 4 * n, n, n };
	dict_rect[(int)Tile::FLOOR_STALACTITE_FALL_L] = { 22 * n, 4 * n, n, n };
	dict_rect[(int)Tile::FLOOR_STALACTITE_FALL_R] = { 23 * n, 4 * n, n, n };
	dict_rect[(int)Tile::FLOOR_DROP] = { 24 * n, 4 * n, n, n };
	dict_rect[(int)Tile::LOCK_KEY] = { 25 * n, 4 * n, n, n };

	dict_rect[(int)Tile::TOP_WATERFALL_L] = { 0, 5 * n, n, n };
	dict_rect[(int)Tile::TOP_WATERFALL_M] = { n, 5 * n, n, n };
	dict_rect[(int)Tile::TOP_WATERFALL_R] = { 2 * n, 5 * n, n, n };
	dict_rect[(int)Tile::PRETOP_WATERFALL_L] = { 3 * n, 5 * n, n, n };
	dict_rect[(int)Tile::PRETOP_WATERFALL_M] = { 4 * n, 5 * n, n, n };
	dict_rect[(int)Tile::PRETOP_WATERFALL_R] = { 5 * n, 5 * n, n, n };
	dict_rect[(int)Tile::BOTTOM_WATERFALL_L] = { 6 * n, 5 * n, n, n };
	dict_rect[(int)Tile::BOTTOM_WATERFALL_M] = { 7 * n, 5 * n, n, n };
	dict_rect[(int)Tile::BOTTOM_WATERFALL_R] = { 8 * n, 5 * n, n, n };
	dict_rect[(int)Tile::FALL_WATERFALL_L] = { 9 * n, 5 * n, n, n };
	dict_rect[(int)Tile::FALL_WATERFALL_M] = { 10 * n, 5 * n, n, n };
	dict_rect[(int)Tile::FALL_WATERFALL_R] = { 11 * n, 5 * n, n, n };
	








	/*dict_rect[(int)Tile::LOCK_RED] = { 6 * n, 2 * n, n, n };
	dict_rect[(int)Tile::LOCK_YELLOW] = { 7 * n, 2 * n, n, n };

	dict_rect[(int)Tile::LASER_L] = { 0, 6 * n, n, n };
	dict_rect[(int)Tile::LASER_R] = { 4 * n, 6 * n, n, n };
	
	dict_rect[(int)Tile::LASER_FRAME0] = { 1 * n, 6 * n, n, n };
	dict_rect[(int)Tile::LASER_FRAME1] = { 2 * n, 6 * n, n, n };
	dict_rect[(int)Tile::LASER_FRAME2] = { 3 * n, 6 * n, n, n };*/

}
AppStatus TileMap::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(Resource::IMG_TILES, "images/tileset6.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles = data.GetTexture(Resource::IMG_TILES);

	/*laser = new Sprite(img_tiles);
	if (laser == nullptr)
	{
		LOG("Failed to allocate memory for laser sprite");
		return AppStatus::ERROR;
	}
	laser->SetNumberAnimations(1);
	laser->SetAnimationDelay(0, ANIM_DELAY);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME0]);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME1]);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME2]);
	laser->SetAnimation(0);*/

	return AppStatus::OK;
}
AppStatus TileMap::Load(int data[], int w, int h)
{
	size = w*h;
	width = w;
	height = h;

	if (map != nullptr)	delete[] map;

	map = new Tile[size];
	if (map == nullptr)	
	{
		LOG("Failed to allocate memory for tile map");
		return AppStatus::ERROR;
	}
	memcpy(map, data, size * sizeof(int));

	return AppStatus::OK;
}
void TileMap::ClearObjectEntityPositions()
{
	int i;
	Tile tile;
	for (i = 0; i < size; ++i)
	{
		tile = map[i];
		if (IsTileEntity(tile) || IsTileObject(tile) || tile == Tile::EMPTY)
			map[i] = Tile::AIR;
	}
}
void TileMap::Update()
{
	/*laser->Update();*/
}
Tile TileMap::GetTileIndex(int x, int y) const
{
	int idx = x + y*width;
	if(idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y)
		return Tile::AIR;
	}
	return map[x + y * width];
}
bool TileMap::IsTileObject(Tile tile) const
{
	return Tile::OBJECT_FIRST <= tile && tile <= Tile::OBJECT_LAST;
}
bool TileMap::IsTileEntity(Tile tile) const
{
	return Tile::ENTITY_FIRST <= tile && tile <= Tile::ENTITY_LAST;
}
bool TileMap::IsTileSolid(Tile tile) const
{
	return (Tile::SOLID_FIRST <= tile && tile <= Tile::SOLID_LAST);
}
bool TileMap::IsTileLadderTop(Tile tile) const
{
	return tile == Tile::TOP_LADDER;
}
bool TileMap::IsTileLadderBottom(Tile tile) const
{
	return tile == Tile::FLOOR_LADDER;
}
bool TileMap::IsTileLadder(Tile tile) const
{
	bool b1, b2;
	b1 = tile == Tile::MIDDLE_LADDER;
	b2 = tile == Tile::FLOOR_LADDER;
	return b1 || b2;
}
bool TileMap::TestCollisionWallLeft(const AABB& box) const
{
	return CollisionX(box.pos, box.height);
}
bool TileMap::TestCollisionWallRight(const AABB& box) const
{
	return CollisionX(box.pos + Point(box.width - 1, 0), box.height);
}
bool TileMap::TestCollisionGround(const AABB& box, int *py) const
{
	Point p(box.pos.x, *py);	//control point
	int tile_y;

	if (CollisionY(p, box.width))
	{
		tile_y = p.y / TILE_SIZE;

		*py = tile_y * TILE_SIZE - 1;
		return true;
	}
	return false;
}
bool TileMap::TestFalling(const AABB& box) const
{
	return !CollisionY(box.pos + Point(0, box.height), box.width);
}
bool TileMap::CollisionX(const Point& p, int distance) const
{
	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;
	
	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		//One solid tile is sufficient
		if (IsTileSolid(GetTileIndex(x, y)))
			return true;
	}
	return false;
}
bool TileMap::CollisionY(const Point& p, int distance) const
{
	int x, y, x0, x1;
	Tile tile;

	//Calculate the tile coordinates and the range of tiles to check for collision
	y = p.y / TILE_SIZE;
	x0 = p.x / TILE_SIZE;
	x1 = (p.x + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the horizontal range
	for (x = x0; x <= x1; ++x)
	{
		tile = GetTileIndex(x, y);

		//One solid or laddertop tile is sufficient
		if (IsTileSolid(tile) || IsTileLadderTop(tile))
			return true;
	}
	return false;
}
bool TileMap::TestOnLadder(const AABB& box, int* px) const
{
	int left, right, bottom;
	int tx1, tx2, ty;
	Tile tile1;
	//tile2

	//Control points
	left = box.pos.x;
	right = box.pos.x + box.width-1;
	bottom = box.pos.y + box.height-1;

	//Calculate the tile coordinates
	tx1 = left / TILE_SIZE;
	tx2 = right / TILE_SIZE;
	ty = bottom / TILE_SIZE;

	//To be able to climb up or down, both control points must be on ladder
	tile1 = GetTileIndex(tx1, ty);
	/*tile2 = GetTileIndex(tx2, ty);*/
	if (IsTileLadder(tile1) /*&& IsTileLadder(tile2)*/)
	{
		*px = GetLadderCenterPos(left, bottom) - box.width/2;
		return true;
	}
	return false;
}
bool TileMap::TestOnLadderTop(const AABB& box, int* px) const
{
	int left, right, bottom;
	int tx1, tx2, ty;
	Tile tile1;
	//tile2

	//Control points
	left = box.pos.x;
	right = box.pos.x + box.width - 1;
	bottom = box.pos.y + box.height - 1;

	//Calculate the tile coordinates
	tx1 = left / TILE_SIZE;
	tx2 = right / TILE_SIZE;
	ty = bottom / TILE_SIZE;

	//To be able to climb up or down, both control points must be on ladder
	tile1 = GetTileIndex(tx1, ty);
	/*tile2 = GetTileIndex(tx2, ty);*/
	if (IsTileLadderTop(tile1) /*&& IsTileLadderTop(tile2)*/)
	{
		*px = GetLadderCenterPos(left, bottom) - box.width / 2;
		return true;
	}
	return false;
}
bool TileMap::TestOnLadderBottom(const AABB& box, int* px) const
{
	int left, right, bottom;
	int tx1, tx2, ty;
	Tile tile1;
	//tile2

	//Control points
	left = box.pos.x;
	right = box.pos.x + box.width - 1;
	bottom = box.pos.y + box.height - 1;

	//Calculate the tile coordinates
	tx1 = left / TILE_SIZE;
	tx2 = right / TILE_SIZE;
	ty = bottom / TILE_SIZE;

	//To be able to climb up or down, both control points must be on ladder
	tile1 = GetTileIndex(tx1, ty);
	/*tile2 = GetTileIndex(tx2, ty);*/
	if (IsTileLadderBottom(tile1) /*&& IsTileLadder(tile2)*/)
	{
		*px = GetLadderCenterPos(left, bottom) - box.width / 2;
		return true;
	}
	return false;
}
int TileMap::GetLadderCenterPos(int pixel_x, int pixel_y) const
{
	int tx, ty;
	
	tx = pixel_x / TILE_SIZE;
	ty = pixel_y / TILE_SIZE;
	Tile tile = GetTileIndex(tx, ty);

	if (tile == Tile::FLOOR_LADDER || tile == Tile::MIDDLE_LADDER || tile == Tile::TOP_LADDER)		return tx * TILE_SIZE + TILE_SIZE;
	
	else
	{
		LOG("Internal error, tile should be a LADDER, coord: (%d,%d), tile type: %d", pixel_x, pixel_y, (int)tile);
		return 0;
	}
}
AABB TileMap::GetSweptAreaX(const AABB& hitbox) const
{
	AABB box;
	int column, x, y, y0, y1;
	bool collision;

	box.pos.y = hitbox.pos.y;
	box.height = hitbox.height;

	column = hitbox.pos.x / TILE_SIZE;
	y0 = hitbox.pos.y / TILE_SIZE;
	y1 = (hitbox.pos.y + hitbox.height - 1) / TILE_SIZE;

	//Compute left tile index
	collision = false;
	x = column - 1;
	while (!collision && x >= 0)
	{
		//Iterate over the tiles within the vertical range
		for (y = y0; y <= y1; ++y)
		{
			//One solid tile is sufficient
			if (IsTileSolid(GetTileIndex(x, y)))
			{
				collision = true;
				break;
			}
		}
		if (!collision) x--;
	}
	box.pos.x = (x + 1) * TILE_SIZE;

	//Compute right tile index
	collision = false;
	x = column + 1;
	while (!collision && x < LEVEL_WIDTH)
	{
		//Iterate over the tiles within the vertical range
		for (y = y0; y <= y1; ++y)
		{
			//One solid tile is sufficient
			if (IsTileSolid(GetTileIndex(x, y)))
			{
				collision = true;
				break;
			}
		}
		if (!collision) x++;
	}
	box.width = x * TILE_SIZE - box.pos.x;

	return box;
}

void TileMap::Render()
{
	Tile tile;
	Rectangle rc;
	Vector2 pos;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			tile = map[i * width + j];
			if (tile != Tile::AIR)
			{
				pos.x = (float)j * TILE_SIZE;
				pos.y = (float)i * TILE_SIZE;

				rc = dict_rect[(int)tile];
				DrawTextureRec(*img_tiles, rc, pos, WHITE);
				

			}
		}
	}
}
void TileMap::Release()
{
	ResourceManager& data = ResourceManager::Instance(); 
	data.ReleaseTexture(Resource::IMG_TILES);

	/*laser->Release();*/

	dict_rect.clear();
}