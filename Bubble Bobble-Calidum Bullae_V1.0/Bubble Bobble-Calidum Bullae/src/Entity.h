
#pragma once
#include <raylib.h>
#include "Point.h"
#include "ResourceManager.h"
#include "RenderComponent.h"
#include "AABB.h"

class Entity
{
public:
	Entity();
	Entity(const Point& p, int width, int height);
	Entity(const Point& p, int width, int height, int frame_width, int frame_height);
	virtual ~Entity();

	void SetPos(const Point& p);
	void Set(const Point& p, const Point& d, int w, int h, int framew, int frameh);
	void Update();
	void connect();
	AABB GetHitbox() const;

	void SetAlive(bool b);
	bool IsAlive() const;

	//Draw representation model
	void Draw() const;
	void DrawTint(const Color& col) const;
	
	//Draw logical/physical model (hitbox)
	void DrawHitbox(const Color& col) const;
	void DrawHitbox(int x, int y, int w, int h, const Color& col) const;

	Point GetPos();
protected:
	Point GetRenderingPosition() const;

	//Logical/physical model
	Point pos, dir;
	int width, height;				

	//Representation model
	int frame_width, frame_height;

	bool alive;

	RenderComponent *render;
};