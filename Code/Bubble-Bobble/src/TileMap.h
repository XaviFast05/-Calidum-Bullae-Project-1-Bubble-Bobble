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

	// 0 < id < 50: static tiles
	//SLABS
	SLAB_1 = 1, SLAB_2, SLAB_3, SLAB_4, SLAB_BOSS,

	//ROOF
	ROOF_2 = 6, ROOF_3, ROOF_4, ROOF_B,

	//PARETS
	PARET1_LVL1 = 10,
	PARET1_LVL2 = 20, PARET2_LVL2, PARET3_LVL2, PARET4_LVL2,
	PARET1_LVL3 = 30, PARET2_LVL3, PARET3_LVL3, PARET4_LVL3, PARET5_LVL3 = 35,
	PARET1_LVL4 = 40, PARET2_LVL4, PARET3_LVL4, PARET4_LVL4, PARET5_LVL4 = 45,
	PARET1_LVLB = 50,

	//SOMBRES
	S_FINAL_LATERAL1 = 60, S_LATERAL_1, S_FINAL_INFERIOR1, S_INFERIOR1, S_CANTO_INT1, S_CANTO_EXT1,
	S_FINAL_LATERAL2 = 70, S_LATERAL_2, S_FINAL_INFERIOR2, S_INFERIOR2, S_CANTO_INT2, S_CANTO_EXT2,
	S_FINAL_LATERAL3 = 80, S_LATERAL_3, S_FINAL_INFERIOR3, S_INFERIOR3, S_CANTO_INT3, S_CANTO_EXT3,

	// id >= 100: entities' initial locations
	PLAYER = 100,

	BUSTER = 101,
	SKELMON = 102,

	ITEM_MUSHROOM = 90, ITEM_BANANA, ITEM_ICE_CREAM, ITEM_FLAN, ITEM_CAKE, ITEM_CHERRY,

	//Intervals
	SLAB_FIRST = SLAB_1,
	SLAB_LAST = SLAB_BOSS,
	SOLID_FIRST = PARET1_LVL1,
	SOLID_LAST = PARET1_LVLB,
	ENTITY_FIRST = PLAYER,
	ENTITY_LAST = SKELMON

};

class TileMap
{
public:
	TileMap();
	~TileMap();

	AppStatus Initialise();
	AppStatus Load(int data[], int w, int h);
	void Update();
	void Render();
	void Release();


	bool TestInsideSolid(const Point& p, int distance) const;

	//Test for collisions with walls
	bool TestCollisionWallLeft(const AABB& box) const;
	bool TestCollisionWallRight(const AABB& box) const;

	//Test collision with the ground and update 'py' with the maximum y-position to prevent
	//penetration of the grounded tile, that is, the pixel y-position above the grounded tile.
	//Grounded tile = solid tile (blocks) or ladder tops.
	bool TestCollisionGround(const AABB& box, int* py) const;

	//Test if there is a ground tile one pixel below the given box
	bool TestFalling(const AABB& box) const;



private:
	void InitTileDictionary();

	Tile GetTileIndex(int x, int y) const;
	bool IsTileSolid(Tile tile) const;
	bool IsTileSlab(Tile tile) const;
	bool CollisionX(const Point& p, int distance) const;
	bool CollisionY(const Point& p, int distance) const;

	//Tile map
	Tile* map;

	//Size of the tile map
	int size, width, height;

	//Dictionary of tile frames
	std::unordered_map<int, Rectangle> dict_rect;

	Sprite* laser;

	//Tile sheet
	const Texture2D* img_tiles;
	const Texture2D* img_items;
};