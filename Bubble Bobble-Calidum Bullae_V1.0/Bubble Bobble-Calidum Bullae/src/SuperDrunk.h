#pragma once
#include "Entity.h"
#include "TileMap.h"
#include "Player.h"

//Representation model size: 32x32
#define DRUNK_FRAME_SIZE	64

//Logical model size: 12x28
#define DRUNK_PHYSICAL_WIDTH	64
#define DRUNK_PHYSICAL_HEIGHT	64
#define DRUNK_GROUNDCHECK_WIDTH 0

//Horizontal speed and vertical speed while falling down
#define DRUNK_SPEED			1

//Frame animation delay while on a ladder
#define DRUNK_JUMP_DELAY		(2*ANIM_DELAY)

//When jumping, initial jump speed and maximum falling speed
#define DRUNK_JUMP_FORCE		9

//Frame delay for updating the jump velocity
#define DRUNK_JUMP_DELAY		2

//Player is levitating when abs(speed) <= this value
#define DRUNK_LEVITATING_SPEED	2

//Gravity affects jumping velocity when jump_delay is 0
#define DRUNK_FORCE			1



//Logic states
enum class D_State { IDLE, WALKING, ATTACKING, JUMPING, FALLING, CLIMBING, DEAD };
enum class D_Look { RIGHT, LEFT };

//Rendering states
enum class Diag {DIAG_UP, DIAG_DOWN};
enum class DrunkAnim {
	IDLE_LEFT, IDLE_RIGHT,
	WALKING_LEFT, WALKING_RIGHT,
	JUMPING_LEFT, JUMPING_RIGHT,
	LEVITATING_LEFT, LEVITATING_RIGHT,
	FALLING_LEFT, FALLING_RIGHT,
	CLIMBING, CLIMBING_PRE_TOP, CLIMBING_TOP,
	SHOCK_LEFT, SHOCK_RIGHT,
	TELEPORT_LEFT, TELEPORT_RIGHT,
	NUM_ANIMATIONS,
	ATTACK_RIGHT, ATTACK_LEFT
};

class Drunk : public Entity
{
public:
	Drunk(const Point& p, D_State s, D_Look view);
	~Drunk();

	AppStatus Initialise();
	void LifeManager();
	void SetTileMap(TileMap* tilemap);
	void SetPlayer(Player* play);

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();
	void Stop();
	int GetLifes();
private:

	bool IsLookingRight() const;
	bool IsLookingLeft() const;

	//Drunk movement
	void MoveX();
	void MoveY();

	Diag DIAG_MOVE;

	//Animation management
	void SetAnimation(int id);
	DrunkAnim GetAnimation();

	void StartWalkingLeft();
	void StartWalkingRight();
	void StartFalling();
	void StartJumping();
	void StartAttacking();
	void ChangeAnimRight();
	void ChangeAnimLeft();

	//Jump steps
	bool IsAscending() const;
	bool IsLevitating() const;
	bool IsDescending() const;

	//Ladder get in/out steps
	bool IsInFirstHalfTile() const;
	bool IsInSecondHalfTile() const;



	D_State state;
	D_Look look;
	int jump_delay;

	TileMap* map;

	int lifes;
	int score;
	Player* player;
};