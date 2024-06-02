#include "Object.h"
#include "StaticImage.h"

Sound sounObjects[10];

Object::Object(const Point& p, ObjectType t) : Entity(p, OBJECT_PHYSICAL_SIZE, OBJECT_PHYSICAL_SIZE, OBJECT_FRAME_SIZE, OBJECT_FRAME_SIZE)
{
	type = t; 
	
	Rectangle rc;
	const int n = OBJECT_FRAME_SIZE;
	switch (type)
	{
		case ObjectType::MUSHROOM: rc = {0, 0, n, n}; break;
		case ObjectType::BANANA: rc = {n, 0, n, n}; break;
		case ObjectType::ICE_CREAM: rc = { n*2, 0, n, n }; break;
		case ObjectType::FLAN: rc = { n*3, 0, n, n }; break;
		case ObjectType::CAKE: rc = { n*4, 0, n, n }; break;
		case ObjectType::CHERRY: rc = { n*5, 0, n, n }; break;

		default: LOG("Internal error: object creation of invalid type");
	}
	sounObjects[0] = LoadSound("sound/SoundEffects/Items/GrabFruitFX.wav");
	ResourceManager& data = ResourceManager::Instance();
	render = new StaticImage(data.GetTexture(Resource::IMG_ITEMS), rc);
}
Object::~Object()
{
}
void Object::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
int Object::Points() const
{
	if (type == ObjectType::MUSHROOM)
	{
		PlaySound(sounObjects[0]);
		return POINTS_MUSHROOM;
	}
	else if (type == ObjectType::BANANA) 
	{ 
		PlaySound(sounObjects[0]);
		return POINTS_BANANA; 
	}
	else if (type == ObjectType::ICE_CREAM) 
	{ 
		PlaySound(sounObjects[0]);
		return POINTS_ICE_CREAM; 
	}
	else if (type == ObjectType::FLAN) 
	{ 
		PlaySound(sounObjects[0]);
		return POINTS_FLAN; 
	}
	else if (type == ObjectType::CAKE)
	{
		PlaySound(sounObjects[0]);
		return POINTS_CAKE;
	}
	else if (type == ObjectType::CHERRY) 
	{ 
		PlaySound(sounObjects[0]);
		return POINTS_CHERRY; 
	}
	else
	{
		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}