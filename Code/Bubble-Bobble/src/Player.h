#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 32x32
#define PLAYER_FRAME_SIZE		16

//Logical model size: 12x28
#define PLAYER_PHYSICAL_WIDTH	12
#define PLAYER_PHYSICAL_HEIGHT	16
#define PLAYER_GROUNDCHECK_WIDTH 9

//Horizontal speed and vertical speed while falling down
#define PLAYER_SPEED			1

//Frame animation delay while on a ladder
#define ANIM_JUMP_DELAY		(2*ANIM_DELAY)

//When jumping, initial jump speed and maximum falling speed
#define PLAYER_JUMP_FORCE		9

//Frame delay for updating the jump velocity
#define PLAYER_JUMP_DELAY		2

//Player is levitating when abs(speed) <= this value
#define PLAYER_LEVITATING_SPEED	2

//Gravity affects jumping velocity when jump_delay is 0
#define GRAVITY_FORCE			1



//Logic states
enum class State { IDLE, WALKING, ATTACKING, JUMPING, FALLING, CLIMBING, DEAD, DIE };
enum class Look { RIGHT, LEFT };

//Rendering states
enum class PlayerAnim {
	IDLE_LEFT, IDLE_RIGHT,
	WALKING_LEFT, WALKING_RIGHT,
	JUMPING_LEFT, JUMPING_RIGHT,
	LEVITATING_LEFT, LEVITATING_RIGHT,
	FALLING_LEFT, FALLING_RIGHT,
	CLIMBING, CLIMBING_PRE_TOP, CLIMBING_TOP,
	SHOCK_LEFT, SHOCK_RIGHT,
	TELEPORT_LEFT, TELEPORT_RIGHT,
	NUM_ANIMATIONS,
	ATTACK_RIGHT, ATTACK_LEFT,
	DEATH_ANIM
};

class Player: public Entity
{
public:
	Player(const Point& p, State s, Look view);
	~Player();
	
	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void InitScore();
	void IncrScore(int n);
	int GetScore();
	void GetHit();
	int GetLifes();
	State GetState();
	bool LooseCondition();

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();
	void Stop();

	bool IsLookingRight() const;
	bool IsLookingLeft() const;

private:

	//Player mechanics
	void MoveX();
	void MoveY();
	void LogicJumping();
	void Attack();

	//Animation management
	void SetAnimation(int id);
	PlayerAnim GetAnimation();
	
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
	int frame_ground;

	TileMap *map;

	int lifes;
	int score;
	float BubbleTime;
};

