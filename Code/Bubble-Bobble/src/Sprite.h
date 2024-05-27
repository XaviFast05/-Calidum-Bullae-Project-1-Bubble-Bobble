#pragma once

#include "RenderComponent.h"
#include <vector>

enum class AnimMode { AUTOMATIC, MANUAL };

struct Animation
{
    int delay;
    int offsetX = 0;
    int offsetY = 0;
    std::vector<Rectangle> frames;
};

class Sprite : public RenderComponent
{
public:
    Sprite(const Texture2D *texture);
    ~Sprite();

    void SetNumberAnimations(int num);
    void SetAnimationDelay(int id, int delay);
    void AddKeyFrame(int id, const Rectangle& rect);
    void AddKeyFrameOffset(int id, const Rectangle& rect, int offsetX, int offsetY);
    void SetAnimation(int id);
    int GetAnimation();

    void SetManualMode();
    void SetAutomaticMode();
    bool AnimationEnd() const;
    
    void Update();
    void NextFrame();
    void PrevFrame();

    void Draw(int x, int y) const override;
    void DrawTint(int x, int y, const Color& col) const override;

    void Release() override;

private:
    int current_anim;
    int current_frame;
    int current_delay;

    const Texture2D *img;
    std::vector<Animation> animations;

    AnimMode mode;

    bool animation_end = false;
};

