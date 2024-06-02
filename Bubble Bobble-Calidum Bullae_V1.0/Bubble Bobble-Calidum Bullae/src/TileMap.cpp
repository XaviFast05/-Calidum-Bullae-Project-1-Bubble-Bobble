#include "TileMap.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <cstring>
#include "Player.h"

TileMap::TileMap()
{
	map = nullptr;
	width = 0;
	height = 0;
	
	img_tiles = nullptr;
	img_items = nullptr;

	InitTileDictionary();
}
TileMap::~TileMap()
{
	if (map != nullptr)
	{
		delete[] map;
		map = nullptr;
	}
	
}
void TileMap::InitTileDictionary()
{
	const int n = TILE_SIZE;

	dict_rect[(int)Tile::SLAB_1] = { 10 * n,  n, n, n };
	dict_rect[(int)Tile::SLAB_2] = { 10 * n,  0, n, n };
	dict_rect[(int)Tile::SLAB_3] = { 11 * n,  0, n, n };
	dict_rect[(int)Tile::SLAB_4] = { 11 * n,  n, n, n };
	dict_rect[(int)Tile::SLAB_BOSS] = { 12 * n,  0, n, n };

	dict_rect[(int)Tile::ROOF_2] = { 10 * n,  0, n, n };
	dict_rect[(int)Tile::ROOF_3] = { 11 * n,  0, n, n };
	dict_rect[(int)Tile::ROOF_4] = { 11 * n,  n, n, n };
	dict_rect[(int)Tile::ROOF_B] = { 12 * n,  0, n, n };

	dict_rect[(int)Tile::PARET1_LVL1] = { 0,  0, n, n };

	dict_rect[(int)Tile::PARET1_LVL2] = { 2 * n,  0, n, n };
	dict_rect[(int)Tile::PARET2_LVL2] = { 3 * n,  0, n, n };
	dict_rect[(int)Tile::PARET3_LVL2] = { 2 * n,  n, n, n };
	dict_rect[(int)Tile::PARET4_LVL2] = { 3 * n,  n, n, n };

	dict_rect[(int)Tile::PARET1_LVL3] = { 4 * n,  0, n, n };
	dict_rect[(int)Tile::PARET2_LVL3] = { 5 * n,  0, n, n };
	dict_rect[(int)Tile::PARET3_LVL3] = { 4 * n,  n, n, n };
	dict_rect[(int)Tile::PARET4_LVL3] = { 5 * n,  n, n, n };
	dict_rect[(int)Tile::PARET5_LVL3] = { 11 * n,  0, n, n };

	dict_rect[(int)Tile::PARET1_LVL4] = { 6 * n,  0, n, n };
	dict_rect[(int)Tile::PARET2_LVL4] = { 7 * n,  0, n, n };
	dict_rect[(int)Tile::PARET3_LVL4] = { 6 * n,  n, n, n };
	dict_rect[(int)Tile::PARET4_LVL4] = { 7 * n,  n, n, n };
	dict_rect[(int)Tile::PARET5_LVL4] = { 11 * n,  n, n, n };

	dict_rect[(int)Tile::PARET1_LVLB] = { 8 * n,  0, n, n };

	dict_rect[(int)Tile::S_FINAL_LATERAL1] = { 0,  2 * n, n, n };
	dict_rect[(int)Tile::S_LATERAL_1] = { n,  2 * n, n, n };
	dict_rect[(int)Tile::S_FINAL_INFERIOR1] = { 0,  3 * n, n, n };
	dict_rect[(int)Tile::S_INFERIOR1] = { n,  3 * n, n, n };
	dict_rect[(int)Tile::S_CANTO_INT1] = { 2 * n,  2 * n, n, n };
	dict_rect[(int)Tile::S_CANTO_EXT1] = { 2 * n,  3 * n, n, n };

	dict_rect[(int)Tile::S_FINAL_LATERAL2] = { 3 * n,   2 * n, n, n };
	dict_rect[(int)Tile::S_LATERAL_2] = { 4 * n,   2 * n, n, n };
	dict_rect[(int)Tile::S_FINAL_INFERIOR2] = { 3 * n,  3 * n, n, n };
	dict_rect[(int)Tile::S_INFERIOR2] = { 4 * n,  3 * n, n, n };
	dict_rect[(int)Tile::S_CANTO_INT2] = { 5 * n,  2 * n, n, n };
	dict_rect[(int)Tile::S_CANTO_EXT2] = { 5 * n,  3 * n, n, n };

	dict_rect[(int)Tile::S_FINAL_LATERAL3] = { 6 * n,   2 * n, n, n };
	dict_rect[(int)Tile::S_LATERAL_3] = { 7 * n,   2 * n, n, n };
	dict_rect[(int)Tile::S_FINAL_INFERIOR3] = { 6 * n,   3 * n, n, n };
	dict_rect[(int)Tile::S_INFERIOR3] = { 7 * n,   3 * n, n, n };
	dict_rect[(int)Tile::S_CANTO_INT3] = { 8 * n,   2 * n, n, n };
	dict_rect[(int)Tile::S_CANTO_EXT3] = { 8 * n,   3 * n, n, n };
}
AppStatus TileMap::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(Resource::IMG_TILES, "images/tiles.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles = data.GetTexture(Resource::IMG_TILES);

	if (data.LoadTexture(Resource::IMG_ITEMS, "images/items.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_items = data.GetTexture(Resource::IMG_ITEMS);
	return AppStatus::OK;
}
AppStatus TileMap::Load(int data[], int w, int h)
{
	size = w * h;
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
void TileMap::Update()
{

}
Tile TileMap::GetTileIndex(int x, int y) const
{
	int idx = x + y * width;
	if (idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y)
			return Tile::AIR;
	}
	return map[x + y * width];
}
bool TileMap::IsTileSolid(Tile tile) const
{
	return (Tile::SOLID_FIRST <= tile && tile <= Tile::SOLID_LAST);
}

bool TileMap::IsTileSlab(Tile tile) const
{
	return(Tile::SLAB_FIRST <= tile && tile <= Tile::SLAB_LAST);
}

bool TileMap::TestInsideSolid(const Point& p, int distance) const
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

		//One solid or slab tile is sufficient
		if (IsTileSolid(tile))
			return true;
	}
	return false;
}

bool TileMap::TestCollisionWallLeft(const AABB& box) const
{
	return CollisionX(box.pos, box.height);
}
bool TileMap::TestCollisionWallRight(const AABB& box) const
{
	return CollisionX(box.pos + Point(box.width - 1, 0), box.height);
}
bool TileMap::TestCollisionTop(const AABB& box, int* py) const
{
	Point p(box.pos.x, *py - box.height);	//control point
	int tile_y;

	if (CollisionTop(p, box.width))
	{
		tile_y = p.y / TILE_SIZE;

		*py = tile_y * TILE_SIZE + 1;
		return true;
	}
	return false;
}
bool TileMap::TestCollisionGround(const AABB& box, int* py) const
{
	Point p(box.pos.x, *py);	//control point
	int tile_y;

	if (CollisionY(p, PLAYER_GROUNDCHECK_WIDTH))
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

		//One solid or slab tile is sufficient
		if (IsTileSolid(tile)||IsTileSlab(tile))
			return true;
	}
	return false;
}
bool TileMap::CollisionTop(const Point& p, int distance) const
{
	int x, y, x0, x1;
	Tile tile;

	//Calculate the tile coordinates and the range of tiles to check for collision
	y = (p.y + 1) / TILE_SIZE;
	x0 = p.x / TILE_SIZE;
	x1 = (p.x + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the horizontal range
	for (x = x0; x <= x1; ++x)
	{
		tile = GetTileIndex(x, y);

		//One solid or laddertop tile is sufficient
		if (IsTileSolid(tile) || IsTileSlab(tile))
			return true;
	}
	return false;
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


	dict_rect.clear();
}