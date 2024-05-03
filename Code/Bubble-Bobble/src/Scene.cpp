#include "Scene.h"
#include <stdio.h>
#include "Globals.h"

//arreglao

Scene::Scene()
{
	player = nullptr;
    level = nullptr;
	
	camera.target = { 0, 0 };				 //Center of the screen
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

	for (Entity* enem : enemies)
	{
		delete enem;
	}
	enemies.clear();
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

	font1 = new Text();
	if (font1 == nullptr)
	{
		LOG("Failed to allocate memory for font 1");
		return AppStatus::ERROR;
	}
	//Initialise text font 1
	if (font1->Initialise(Resource::IMG_FONT, "images/font8x8.png", ' ', 8) != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}

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
	Enemy *enem;
	
	ClearLevel();
	player->Stop();
	size = LEVEL_WIDTH * LEVEL_HEIGHT;
	if (stage == 1)
	{
		map = new int[size] {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			10, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 10, 10,
			10, 10, 74, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 10, 10,
			10, 10, 71, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 10,
			10, 10, 71, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 10,
			10, 10, 71, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 10,
			10, 10, 71, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 10,
			10, 10, 71, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 10,
			10, 10, 71, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 10,
			10, 10, 71, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 10,
			10, 10, 71, 101, 0, 0, 0, 0, 93, 0, 0, 0, 0, 0, 0, 94, 0, 0, 0, 0, 0, 95, 0, 0, 0, 0, 0, 0, 102, 0, 10, 10,
			10, 10, 1, 1, 70, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 70, 0, 0, 1, 1, 10, 10,
			10, 10, 74, 73, 75, 0, 0, 72, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 75, 0, 0, 72, 73, 10, 10,
			10, 10, 71, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 10,
			10, 10, 71, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 10,
			10, 10, 71, 0, 0, 0, 0, 0, 90, 0, 0, 0, 0, 0, 0, 91, 0, 0, 0, 0, 0, 0, 92, 0, 0, 0, 0, 0, 0, 0, 10, 10,
			10, 10, 1, 1, 70, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 70, 0, 0, 1, 1, 10, 10,
			10, 10, 74, 73, 75, 0, 0, 72, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 75, 0, 0, 72, 73, 10, 10,
			10, 10, 71, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 10,
			10, 10, 71, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 10,
			10, 10, 71, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 10,
			10, 10, 1, 1, 70, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 70, 0, 0, 1, 1, 10, 10,
			10, 10, 74, 73, 75, 0, 0, 72, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 75, 0, 0, 72, 73, 10, 10,
			10, 10, 71, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 10,
			10, 10, 71, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 10,
			10, 10, 71, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 10,
			10, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 10, 10

		};
		player->InitScore();
	}
	else if (stage == 2)
	{
		map = new int[size] {
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				30, 31, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 30, 31,
				32, 33, 84, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 32, 33,
				30, 31, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 31,
				32, 33, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 33,
				30, 31, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 94, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 31,
				32, 33, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 33,
				30, 31, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 83, 83, 83, 83, 83, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 31,
				32, 33, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 33,
				30, 31, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 31,
				32, 33, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 33,
				30, 31, 81, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 80, 0, 2, 2, 2, 2, 2, 80, 0, 0, 0, 0, 0, 0, 0, 30, 31,
				32, 33, 81, 0, 0, 0, 0, 0, 0, 0, 82, 83, 83, 83, 83, 85, 0, 82, 83, 83, 83, 83, 85, 0, 0, 0, 0, 0, 0, 0, 32, 33,
				30, 31, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 31,
				32, 33, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 33,
				30, 31, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 102, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 31,
				32, 33, 81, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 80, 0, 0, 0, 0, 32, 33,
				30, 31, 81, 0, 0, 0, 0, 82, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 85, 0, 0, 0, 0, 30, 31,
				32, 33, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 33,
				30, 31, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 31,
				32, 33, 81, 0, 0, 0, 0, 0, 91, 0, 0, 0, 0, 0, 0, 0, 101, 0, 0, 0, 0, 0, 0, 0, 0, 95, 0, 0, 0, 0, 32, 33,
				30, 31, 81, 0, 2, 2, 2, 2, 2, 2, 2, 80, 0, 2, 2, 2, 2, 2, 2, 80, 0, 2, 2, 2, 2, 2, 2, 2, 80, 0, 30, 31,
				32, 33, 81, 0, 82, 83, 83, 83, 83, 83, 83, 85, 0, 82, 83, 83, 83, 83, 83, 85, 0, 82, 83, 83, 83, 83, 83, 83, 85, 0, 32, 33,
				30, 31, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 31,
				32, 33, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 33,
				30, 31, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 31,
				32, 33, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 32, 33
		};
	}
	else if (stage == 3)
	{
		map = new int[size] {
			0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0,
			0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0,
				20, 21, 7, 7, 7, 7, 7, 7, 7, 81, 0, 0, 0, 7, 7, 7, 7, 7, 7, 81, 0, 0, 0, 7, 7, 7, 7, 7, 7, 7, 20, 21,
				20, 21, 84, 83, 83, 83, 83, 83, 83, 85, 0, 0, 0, 82, 83, 83, 83, 83, 83, 85, 0, 0, 0, 82, 83, 83, 83, 83, 83, 83, 20, 21,
				20, 21, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 21,
				20, 21, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 21,
				20, 21, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 21,
				20, 21, 81, 0, 0, 35, 3, 3, 3, 3, 3, 3, 3, 80, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 35, 80, 0, 0, 20, 21,
				20, 21, 81, 0, 0, 35, 84, 83, 83, 83, 83, 83, 83, 85, 0, 0, 0, 0, 0, 82, 83, 83, 83, 83, 83, 83, 35, 81, 0, 0, 20, 21,
				20, 21, 81, 0, 0, 35, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 81, 0, 0, 20, 21,
				20, 21, 81, 0, 0, 35, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 81, 0, 0, 20, 21,
				20, 21, 81, 0, 0, 35, 81, 101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 101, 35, 81, 0, 0, 20, 21, 
				20, 21, 81, 0, 0, 35, 3, 3, 3, 3, 3, 3, 3, 3, 80, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 35, 81, 0, 0, 20, 21,
				20, 21, 81, 0, 0, 35, 84, 83, 83, 83, 83, 83, 83, 83, 85, 0, 0, 0, 82, 83, 83, 83, 83, 83, 83, 83, 35, 81, 0, 0, 20, 21,
				20, 21, 81, 0, 0, 35, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 81, 0, 0, 20, 21,
				20, 21, 81, 0, 0, 35, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 81, 0, 0, 20, 21,
				20, 21, 81, 0, 0, 35, 81, 102, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 102, 35, 81, 0, 0, 20, 21,
				20, 21, 81, 0, 0, 35, 3, 3, 3, 3, 3, 3, 3, 3, 3, 80, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 81, 0, 0, 20, 21,
				20, 21, 81, 0, 0, 82, 83, 83, 83, 83, 83, 83, 83, 83, 83, 85, 0, 82, 83, 83, 83, 83, 83, 83, 83, 83, 83, 85, 0, 0, 20, 21,
				20, 21, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 21,
				20, 21, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 21,
				20, 21, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 21,
				20, 21, 3, 3, 3, 3, 3, 80, 0, 3, 3, 3, 80, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 80, 0, 3, 3, 3, 3, 3, 20, 21,
				20, 21, 84, 83, 83, 83, 83, 85, 0, 82, 83, 83, 85, 0, 0, 0, 0, 0, 0, 0, 82, 83, 83, 85, 0, 82, 83, 83, 83, 83, 20, 21,
				20, 21, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 21,
				20, 21, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 21,
				20, 21, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 21,
				20, 21, 3, 3, 3, 3, 3, 3, 35, 80, 0, 0, 0, 35, 3, 3, 3, 3, 35, 80, 0, 0, 0, 35, 3, 3, 3, 3, 3, 3, 20, 21,

				
		};
	}
	else if (stage == 4)
	{
	map = new int[size] {
			0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0,
			0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0,
			40, 41, 8, 8, 8, 8, 8, 8, 45, 61, 0, 0, 0, 8, 8, 8, 8, 8, 8, 60, 0, 0, 0, 45, 8, 8, 8, 8, 8, 8, 40, 41,
			42, 43, 64, 63, 63, 63, 63, 63, 45, 61, 0, 0, 0, 62, 63, 63, 63, 63, 63, 65, 0, 0, 0, 45, 64, 63, 63, 63, 63, 63, 42, 43,
			40, 41, 61, 0, 0, 0, 0, 0, 4, 4, 4, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 61, 0, 0, 0, 0, 0, 40, 41,
			42, 43, 61, 0, 0, 0, 0, 0, 62, 63, 63, 65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 63, 63, 65, 0, 0, 0, 0, 0, 42, 43,
			40, 41, 61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 41,
			40, 41, 61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 41,
			42, 43, 61, 0, 0, 45, 4, 4, 4, 4, 4, 4, 4, 45, 60, 0, 0, 0, 45, 4, 4, 4, 4, 4, 4, 4, 45, 60, 0, 0, 42, 43,
			40, 41, 61, 0, 0, 45, 64, 63, 63, 63, 63, 63, 45, 45, 61, 0, 0, 0, 45, 45, 64, 63, 63, 63, 63, 63, 45, 61, 0, 0, 40, 41,
			42, 43, 61, 0, 0, 45, 61, 0, 0, 0, 0, 0, 45, 45, 61, 0, 0, 0, 45, 45, 61, 0, 0, 0, 0, 0, 45, 61, 0, 0, 42, 43,
			40, 41, 61, 0, 0, 45, 61, 0, 0, 0, 0, 0, 45, 45, 61, 0, 0, 0, 45, 45, 61, 0, 0, 0, 0, 0, 45, 61, 0, 0, 40, 41,
			42, 43, 61, 0, 0, 4, 4, 4, 4, 60, 0, 0, 45, 45, 61, 0, 0, 0, 45, 45, 61, 0, 0, 4, 4, 4, 4, 61, 0, 0, 42, 43,
			40, 41, 61, 0, 0, 62, 63, 63, 63, 65, 0, 0, 45, 45, 61, 0, 0, 0, 45, 45, 61, 0, 0, 62, 63, 63, 63, 65, 0, 0, 40, 41,
			42, 43, 61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, 45, 61, 0, 0, 0, 45, 45, 61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42, 43,
			40, 41, 61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, 45, 61, 0, 0, 0, 45, 45, 61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 41,
			42, 43, 61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, 45, 61, 0, 0, 0, 45, 45, 61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42, 43,
			42, 43, 61, 0, 0, 45, 4, 4, 4, 60, 0, 0, 62, 63, 65, 0, 0, 0, 62, 63, 65, 0, 0, 4, 4, 4, 45, 60, 0, 0, 42, 43,
			40, 41, 61, 0, 0, 45, 64, 63, 63, 65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 63, 63, 45, 61, 0, 0, 40, 41,
			42, 43, 61, 0, 0, 45, 61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, 61, 0, 0, 42, 43,
			40, 41, 61, 0, 0, 45, 61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, 61, 0, 0, 40, 41,
			42, 43, 61, 0, 0, 45, 61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, 61, 0, 0, 42, 43,
			40, 41, 61, 0, 0, 4, 4, 4, 4, 4, 4, 4, 60, 0, 0, 4, 4, 60, 0, 0, 4, 4, 4, 4, 4, 4, 4, 61, 0, 0, 40, 41,
			42, 43, 61, 0, 0, 62, 63, 63, 63, 63, 63, 63, 65, 0, 0, 62, 63, 65, 0, 0, 62, 63, 63, 63, 63, 63, 63, 65, 0, 0, 42, 43,
			40, 41, 61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 41,
			42, 43, 61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42, 43,
			40, 41, 61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 41,
			42, 43, 4, 4, 4, 4, 4, 4, 4, 60, 0, 0, 0, 4, 4, 4, 4, 4, 4, 60, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 42, 43,
			


	};
	}
	else if (stage == 5)
	{
	map = new int[size] {
		0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0,
		0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0,
			50, 50, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 50, 50,
			50, 50, 84, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 50, 50,
			50, 50, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 50,
			50, 50, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 50,
			50, 50, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 50,
			50, 50, 81, 0, 0, 0, 0, 0, 0, 0, 5, 5, 80, 0, 0, 0, 0, 0, 0, 0, 5, 5, 80, 0, 0, 0, 0, 0, 0, 0, 50, 50,
			50, 50, 81, 0, 0, 0, 0, 0, 0, 0, 82, 83, 85, 0, 0, 0, 0, 0, 0, 0, 82, 83, 85, 0, 0, 0, 0, 0, 0, 0, 50, 50,
			50, 50, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 50,
			50, 50, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 50,
			50, 50, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 50,
			50, 50, 81, 0, 0, 0, 5, 5, 80, 0, 0, 0, 0, 0, 0, 5, 5, 80, 0, 0, 0, 0, 0, 0, 5, 5, 80, 0, 0, 0, 50, 50,
			50, 50, 81, 0, 0, 0, 82, 83, 85, 0, 0, 0, 0, 0, 0, 82, 83, 85, 0, 0, 0, 0, 0, 0, 82, 83, 85, 0, 0, 0, 50, 50,
			50, 50, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 50,
			50, 50, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 50,
			50, 50, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 50,
			50, 50, 81, 0, 0, 0, 0, 0, 0, 0, 5, 5, 80, 0, 0, 0, 0, 0, 0, 0, 5, 5, 80, 0, 0, 0, 0, 0, 0, 0, 50, 50,
			50, 50, 81, 0, 0, 0, 0, 0, 0, 0, 82, 83, 85, 0, 0, 0, 0, 0, 0, 0, 82, 83, 85, 0, 0, 0, 0, 0, 0, 0, 50, 50,
			50, 50, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 50,
			50, 50, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 50,
			50, 50, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 50,
			50, 50, 81, 0, 0, 0, 5, 5, 80, 0, 0, 0, 0, 0, 0, 5, 5, 80, 0, 0, 0, 0, 0, 0, 5, 5, 80, 0, 0, 0, 50, 50,
			50, 50, 81, 0, 0, 0, 82, 83, 85, 0, 0, 0, 0, 0, 0, 82, 83, 85, 0, 0, 0, 0, 0, 0, 82, 83, 85, 0, 0, 0, 50, 50,
			50, 50, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 50,
			50, 50, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 50,
			50, 50, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 50,
			50, 50, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 50, 50,
			


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
			else if (tile == Tile::ITEM_MUSHROOM)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::MUSHROOM);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::ITEM_BANANA)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::BANANA);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::ITEM_ICE_CREAM)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::ICE_CREAM);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::ITEM_FLAN)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::FLAN);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::ITEM_CAKE)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::CAKE);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::ITEM_CHERRY)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::CHERRY);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::BUSTER)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				enem = new Enemy(pos, E_State::IDLE, E_Look::RIGHT, E_Type::BUSTER);
				enem -> Initialise();
				enem->SetTileMap(level);
				enemies.push_back(enem);
				map[i] = 0;
			}
			else if (tile == Tile::SKELMON)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				enem = new Enemy(pos, E_State::IDLE, E_Look::RIGHT, E_Type::SKELMON);
				enem->Initialise();
				enem->SetTileMap(level);
				enemies.push_back(enem);
				map[i] = 0;
			}
			++i;
		}
	}
	pos.x = 4 * TILE_SIZE;
	pos.y = 27 * TILE_SIZE;
	player->SetPos(pos);
	//Tile map
	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);
	delete[] map;

	return AppStatus::OK;
}
void Scene::BubbleShoot()
{
	BubbleTime += GetFrameTime();

	if (IsKeyPressed(KEY_F) && BubbleTime >= .3)
	{
		if (player->IsLookingLeft())
		{
			Bubble* buble = new Bubble(player->GetPos(), Directions::LEFT);
			buble->Initialise();
			bubbles.push_back(buble);
		}
		else
		{
			Bubble* buble = new Bubble(player->GetPos(), Directions::RIGHT);
			buble->Initialise();
			bubbles.push_back(buble);
		}
		BubbleTime = 0;

	}


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
	if (IsKeyPressed(KEY_ONE))		LoadLevel(1);
	else if (IsKeyPressed(KEY_TWO))	LoadLevel(2);
	else if (IsKeyPressed(KEY_THREE))	LoadLevel(3);
	else if (IsKeyPressed(KEY_FOUR))	LoadLevel(4);
	else if (IsKeyPressed(KEY_FIVE))	LoadLevel(5);

	for (Bubble* buble : bubbles)
	{
		buble->SetPlayer(player);
	}

	level->Update();
	player->Update();
	EnemyUpdate();
	UpdateBubbles();
	CheckCollisions();
	BubbleShoot();
}
void Scene::Render()
{
	BeginMode2D(camera);

    level->Render();
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		RenderObjects(); 
		player->Draw();
		RenderEnemies();
	}
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		player->DrawDebug(GREEN);
		RenderEnemiesDebug(RED);
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
	AABB player_box, obj_box, enemy_box, bubble_box;
	
	player_box = player->GetHitbox();
	auto it = objects.begin();
	while (it != objects.end())
	{
		obj_box = (*it)->GetHitbox();
		if(player_box.TestAABB(obj_box))
		{
			player->IncrScore((*it)->Points());
			
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
	auto en = enemies.begin();
	while (en != enemies.end())
	{
		enemy_box = (*en)->GetHitbox();
		if (player_box.TestAABB(enemy_box))
		{
			player->GetHit();
			delete* en;
			//Erase the object from the vector and get the iterator to the next valid element
			en = enemies.erase(en);
		}
		else
		{
			//Move to the next object
			++en;
		}
	}
	auto as = bubbles.begin();
	while (as != bubbles.end())
	{
		bubble_box = (*as)->GetHitbox();
		if (bubble_box.TestAABB(enemy_box))
		{
			delete* as;
			//Erase the object from the vector and get the iterator to the next valid element
			as = bubbles.erase(as);
		}
		else
		{
			//Move to the next object
			++as;
		}
	}
	auto yp = enemies.begin();
	while (yp != enemies.end())
	{
		enemy_box = (*yp)->GetHitbox();
		if (bubble_box.TestAABB(enemy_box))
		{
			
			delete* yp;
			//Erase the object from the vector and get the iterator to the next valid element
			yp = enemies.erase(yp);
		}
		else
		{
			//Move to the next object
			++yp;
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
	for (Enemy* enem : enemies)
	{
		delete enem;
	}
	enemies.clear();
}
void Scene::UpdateBubbles()
{
	for (Bubble* buble : bubbles)
	{
		buble->Update();
	}
}
void Scene::EnemyUpdate() const
{
	for (Enemy* enem : enemies)
	{
		enem->Update();
	}
}
void Scene::RenderObjects() const
{
	for (Object* obj : objects)
	{
		obj->Draw();
	}
	for (Bubble* bubl : bubbles)
	{
		bubl->Draw();
	}
	auto it = bubbles.begin();
	while (it != bubbles.end())
	{

		(*it)->Draw();
		++it;
	}
}
void Scene::RenderEnemies() const
{
	for (Enemy* enem : enemies)
	{
		enem->Draw();
	}
}
void Scene::RenderObjectsDebug(const Color& col) const
{
	for (Object* obj : objects)
	{
		obj->DrawDebug(col);
	}
}
void Scene::RenderEnemiesDebug(const Color& col) const
{
	for (Enemy* enem : enemies)
	{
		enem->DrawDebug(col);
	}
}
void Scene::RenderGUI() const
{
	//Player 1
	font1->Draw(35, 10, TextFormat("1UP"), GREEN);
	font1->Draw(38, 20, TextFormat("%d", player->GetScore()));
	font1->Draw(2, 220, TextFormat("%d", player->GetLifes()), GREEN);

	//Player 2
	font1->Draw(204, 10, TextFormat("2UP"), BLUE);
	font1->Draw(210, 20, TextFormat("%d", player->GetScore()));
	font1->Draw(245, 220, TextFormat("%d", player->GetLifes()), BLUE);
}
Player* Scene::GetPlayer()
{
	return player;
}