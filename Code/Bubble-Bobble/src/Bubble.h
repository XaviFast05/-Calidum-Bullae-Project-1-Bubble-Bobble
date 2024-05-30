#pragma once
#include "Entity.h"
#include "TileMap.h"
#include "Player.h"
#include "Bubble.h"
#include "ParticleManager.h"

#define BUBBLE_PHYSICAL_SIZE	14
#define BUBBLE_FRAME_SIZE		16
#define POGOJUMP				-30

#define PADDINGG_X 1
#define PADDINGG_Y 6

#define SHOOT_RANGE 65

enum class Directions { LEFT, RIGHT };
enum class BubbleAnim { INSHOOT, IDLE, SMASHED, CATCHEDENEMY, NUM_ANIMATIONS };
class Bubble : public Entity
{
public:
	Bubble(const Point& p, Directions d);
	~Bubble();

	void Update();

	void Movement(Directions d);
	void ClampPos();
	AppStatus Initialise();
	bool isBubbleAlive();
	void Release();
	Directions dire;
	int stages;
	void DrawDebug(const Color& col) const;
	
	void SetPlayer(Player* p);

	//Set the TileMap reference for managing shot collisions
	void SetParticleManager(ParticleManager* particles);

private:
	//void DrawDebug(const Color& col) const;
	void SetAnimation(int id);

	bool inShoot;
	int logPosXL;
	int logPosXR;
	float spawnTime;
	float eTime;
	float eTimePogo;
	float lifeTime;
	float speed;
	Player* player;

	//Reference to the TileMap object
	//This class does not own the object, it only holds a reference to it

};