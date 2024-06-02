#pragma once
#include "Globals.h"
#include "Scene.h"

enum class GameState { MAIN_MENU, PLAYING, SETTINGS, CREDITS, TRANSITION, OPENING, GAME_OVER, START };

class Game
{
public:
    Game();
    ~Game();

    AppStatus Initialise(float scale);
    AppStatus Update();
    void Render();
    void Cleanup();
    void Transition();
    int CheckTime();
    void MusicPlayer();

private:
    AppStatus BeginPlay();
    void FinishPlay();

    AppStatus LoadResources();
    void UnloadResources();

    int Time;
    bool GettingTime;
    bool TransCondition;
    int TransCounter;
    float a;

    GameState state;
    Scene* scene;
    const Texture2D* img_menu;
    const Texture2D* img_upc;
    const Texture2D* img_creators;
    const Texture2D* img_copy;
    const Texture2D* img_game_over;
    const Texture2D* img_opening;

    int TimeCount = 0;
    int Timer = 0;
    float MaxTime = 1.5f;
    float TimeElapsed = 0.0f;
    int i = 0;
    int j = 0;
    int k = 0;

    //To work with original game units and then scale the result
    RenderTexture2D target;
    Rectangle src, dst;
};

