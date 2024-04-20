#pragma once
#include "Entity.h"

#define OBJECT_PHYSICAL_SIZE	14
#define OBJECT_FRAME_SIZE		16

#define POINTS_MUSHROOM	100
#define POINTS_BANANA	500
#define POINTS_ICE_CREAM	900
#define POINTS_FLAN	2000
#define POINTS_CAKE	2000
#define POINTS_CHERRY	700

enum class ObjectType { MUSHROOM, BANANA, ICE_CREAM, FLAN, CAKE, CHERRY };

class Object : public Entity
{
public:
	Object(const Point& p, ObjectType t);
	~Object();

	void DrawDebug(const Color& col) const;
	int Points() const;

private:
	ObjectType type;
};

