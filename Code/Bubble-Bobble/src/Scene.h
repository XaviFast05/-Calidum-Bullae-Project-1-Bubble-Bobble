#pragma once
#include <raylib.h>
#include "Player.h"
#include "TileMap.h"
#include "Object.h"
#include "Enemy.h"

enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SIZE };

class Scene
{
public:
    Scene();
    ~Scene();

    AppStatus Init();
    void Update();
    void Render();
    void Release();

private:
    AppStatus LoadLevel(int stage);
    
    void CheckCollisions(); 
    void ClearLevel();
    void RenderObjects() const;
    void RenderEnemies() const;
    void RenderObjectsDebug(const Color& col) const;
    void RenderEnemiesDebug(const Color& col) const;

    void RenderGUI() const;

    Player *player;
    TileMap *level;
    std::vector<Object*> objects;
    std::vector<Enemy*> enemies;

    Camera2D camera;
    DebugMode debug;
};

