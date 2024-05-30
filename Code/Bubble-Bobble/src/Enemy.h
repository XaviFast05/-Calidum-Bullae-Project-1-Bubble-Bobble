#pragma once
#include "Entity.h"
#include "TileMap.h"
#include "Player.h"

//Representation model size: 32x32
#define ENEMY_FRAME_SIZE		16

//Logical model size: 12x28
#define ENEMY_PHYSICAL_WIDTH	12
#define ENEMY_PHYSICAL_HEIGHT	16
#define ENEMY_GROUNDCHECK_WIDTH 9

//Horizontal speed and vertical speed while falling down
#define ENEMY_SPEED			1

//Frame animation delay while on a ladder
#define ANIM_JUMP_DELAY		(2*ANIM_DELAY)

//When jumping, initial jump speed and maximum falling speed
#define ENEMY_JUMP_FORCE		9

//Frame delay for updating the jump velocity
#define ENEMY_JUMP_DELAY		2

//Player is levitating when abs(speed) <= this value
#define ENEMY_LEVITATING_SPEED	2

//Gravity affects jumping velocity when jump_delay is 0
#define ENEMY_FORCE			1



//Logic states
enum class E_Type {BUSTER, SKELMON};
enum class E_State { IDLE, WALKING, ATTACKING, JUMPING, FALLING, CLIMBING, BUBBLED };
enum class E_Look { RIGHT, LEFT };

//Rendering states
enum class Diag_E { DIAG_UP_E, DIAG_DOWN_E };
enum class EnemyAnim {
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

class Enemy : public Entity
{
public:
	Enemy(const Point& p, E_State s, E_Look view,  E_Type t);
	~Enemy();

	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);
	void SetPlayer(Player* play);

	E_State GetState();

	void Bubbler();
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
	void TimeInBubble();
	int CheckTime();
	void BubbleMovement();
	void LogicJumping();

	Diag_E DIAG_MOVE_E;

	//Animation management
	void SetAnimation(int id);
	EnemyAnim GetAnimation();

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

	E_Type type;
	E_State state;
	E_Look look;
	int jump_delay;

	TileMap* map;

	int lifes;
	int score;
	bool GettingTime;
	int Time;
	Player* player;
	float jumptime;
	bool inShoot;
	int stages;
	int logPosXL;
	int logPosXR;
};