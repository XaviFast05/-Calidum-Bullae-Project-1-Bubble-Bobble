#pragma once
#include <raylib.h>
#include "Player.h"
#include "TileMap.h"
#include "Object.h"
#include "Enemy.h"
#include "Bubble.h"
#include "Text.h"
#include "SuperDrunk.h"
#include "Particle.h"

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
    void BubbleShoot();
    Player* GetPlayer();
private:
    AppStatus LoadLevel(int stage);
    
    void CheckCollisions(); 
    void ClearLevel();
    void EnemyUpdate() const;
    void RenderObjects() const;
    void RenderEnemies() const;
    void RenderObjectsDebug(const Color& col) const;
    void RenderEnemiesDebug(const Color& col) const;
    void RenderGUI() const;
    void UpdateBubbles();
    void RandomItem();

    Player *player;
    TileMap *level;
    std::vector<Drunk*> supers ;
    std::vector<Object*> objects;
    std::vector<Enemy*> enemies;
    std::vector<Bubble*> bubbles;
    std::vector<Particle*> particles;
    float BubbleTime;
    float ObjectTime;
    Camera2D camera;
    int enemiesDefeat;
    int currentStage;
    float TimerStage;

    Text *font1;

    DebugMode debug;
};

