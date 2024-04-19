#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 32x32
#define PLAYER2_FRAME_SIZE		16

//Logical model size: 12x28
#define PLAYER2_PHYSICAL_WIDTH	12
#define PLAYER2_PHYSICAL_HEIGHT	16
#define PLAYER2_GROUNDCHECK_WIDTH 9

//Horizontal speed and vertical speed while falling down
#define PLAYER2_SPEED			1

//Frame animation delay while on a ladder
#define ANIM_JUMP_DELAY		(2*ANIM_DELAY)

//When jumping, initial jump speed and maximum falling speed
#define PLAYER2_JUMP_FORCE		9

//Frame delay for updating the jump velocity
#define PLAYER2_JUMP_DELAY		2

//Player is levitating when abs(speed) <= this value
#define PLAYER2_LEVITATING_SPEED	2

//Gravity affects jumping velocity when jump_delay is 0
#define GRAVITY2_FORCE			1



//Logic states
enum class State { IDLE, WALKING, ATTACKING, JUMPING, FALLING, CLIMBING, DEAD, DIE };
enum class Look { RIGHT, LEFT };

//Rendering states
enum class Player2Anim {
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

class Player2 : public Entity
{
public:
	Player2(const Point& p, State s, Look view);
	~Player2();

	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void InitScore();
	void IncrScore(int n);
	int GetScore();
	void GetHit();
	void LooseCondition();

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();
	void Stop();

private:

	bool IsLookingRight() const;
	bool IsLookingLeft() const;

	//Player mechanics
	void MoveX();
	void MoveY();
	void LogicJumping();

	//Animation management
	void SetAnimation(int id);
	Player2Anim GetAnimation();

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

	State state;
	Look look;
	int jump_delay;

	TileMap* map;

	int lifes;
	int score;
};

