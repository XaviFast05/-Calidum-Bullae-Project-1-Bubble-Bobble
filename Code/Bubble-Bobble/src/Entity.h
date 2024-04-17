
#pragma once
#include <raylib.h>
#include "Point.h"
#include "ResourceManager.h"
#include "RenderComponent.h"
#include "AABB.h"

#define MAX_SHOTS 32
#define SHOOT_SPEED 5

class Entity
{
public:
	Entity(const Point& p, int width, int height);
	Entity(const Point& p, int width, int height, int frame_width, int frame_height, int frame_ground);
	Entity(const Point& p, int width, int height, int frame_width, int frame_height);
	virtual ~Entity();

	void SetPos(const Point& p);
	void Update();
	void connect();
	AABB GetHitbox() const;

	void Init(Point& p, int s);
	Entity* Shots[MAX_SHOTS];
	int idx_shot;
	bool IsAlive() const;
	void Move(Vector2 dir);
	
	void SetAlive(bool b);

	//Draw representation model
	void Draw() const;
	void DrawTint(const Color& col) const;
	
	//Draw logical/physical model (hitbox)
	void DrawHitbox(const Color& col) const;
	void DrawHitbox(int x, int y, int w, int h, const Color& col) const;

protected:
	Point GetRenderingPosition() const;

	//Logical/physical model
	Point pos, dir;
	int width, height;				

	//Representation model
	int frame_width, frame_height, frame_ground;

	
	int speed;
	bool is_alive;

	RenderComponent *render;
};