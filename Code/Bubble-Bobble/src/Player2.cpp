
#include "Player2.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Sound soundEffectsplayer2[10];

Player2::Player2(const Point& p, State s, Look view) :
	Entity(p, PLAYER2_PHYSICAL_WIDTH, PLAYER2_PHYSICAL_HEIGHT, PLAYER2_FRAME_SIZE, PLAYER2_FRAME_SIZE, PLAYER2_GROUNDCHECK_WIDTH)
{
	state = s;
	look = view;
	jump_delay = PLAYER2_JUMP_DELAY;
	map = nullptr;
	score = 0;
	lifes = 3;
}

Player2::~Player2()
{
}
AppStatus Player2::Initialise()
{
	int i;
	const int n = PLAYER2_FRAME_SIZE;
	soundEffectsplayer2[0] = LoadSound("sound/SoundEffects/Characters/JumpFX.wav");

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_PLAYER, "images/bubMoveSprite.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_PLAYER));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)Player2Anim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)Player2Anim::IDLE_RIGHT, ANIM_DELAY);
	for (i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)Player2Anim::IDLE_RIGHT, { 0, 0, -n, n });
		sprite->AddKeyFrame((int)Player2Anim::IDLE_RIGHT, { 0, 0, -n, n });
		sprite->AddKeyFrame((int)Player2Anim::IDLE_RIGHT, { 2 * n, 0, -n, n });
		sprite->AddKeyFrame((int)Player2Anim::IDLE_RIGHT, { 2 * n, 0, -n, n });
	}

	sprite->SetAnimationDelay((int)Player2Anim::IDLE_LEFT, ANIM_DELAY);
	for (i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)Player2Anim::IDLE_LEFT, { 0, 0, n, n });
		sprite->AddKeyFrame((int)Player2Anim::IDLE_LEFT, { 0, 0, n, n });
		sprite->AddKeyFrame((int)Player2Anim::IDLE_LEFT, { 2 * n, 0, n, n });
		sprite->AddKeyFrame((int)Player2Anim::IDLE_LEFT, { 2 * n, 0, n, n });
	}

	sprite->SetAnimationDelay((int)Player2Anim::WALKING_RIGHT, ANIM_DELAY);
	for (i = 1; i < 5; ++i)
		sprite->AddKeyFrame((int)Player2Anim::WALKING_RIGHT, { (float)i * n, 0 * n, -n, n });
	sprite->SetAnimationDelay((int)Player2Anim::WALKING_LEFT, ANIM_DELAY);
	for (i = 1; i < 5; ++i)
		sprite->AddKeyFrame((int)Player2Anim::WALKING_LEFT, { (float)i * n, 0 * n, n, n });

	sprite->SetAnimationDelay((int)Player2Anim::FALLING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)Player2Anim::FALLING_RIGHT, { (float)i * n, 6 * n, -n, n });

	sprite->SetAnimationDelay((int)Player2Anim::FALLING_LEFT, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)Player2Anim::FALLING_LEFT, { (float)i * n, 6 * n, n, n });


	sprite->SetAnimationDelay((int)Player2Anim::JUMPING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)Player2Anim::JUMPING_RIGHT, { (float)i * n, 5 * n, -n, n });

	sprite->SetAnimationDelay((int)Player2Anim::JUMPING_LEFT, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)Player2Anim::JUMPING_LEFT, { (float)i * n, 5 * n, n, n });

	sprite->SetAnimationDelay((int)Player2Anim::LEVITATING_RIGHT, ANIM_JUMP_DELAY);
	sprite->AddKeyFrame((int)Player2Anim::LEVITATING_RIGHT, { n, 5 * n, -n, n });
	sprite->AddKeyFrame((int)Player2Anim::LEVITATING_RIGHT, { 0, 6 * n, -n, n });
	sprite->SetAnimationDelay((int)Player2Anim::LEVITATING_LEFT, ANIM_JUMP_DELAY);
	sprite->AddKeyFrame((int)Player2Anim::LEVITATING_LEFT, { n, 5 * n, n, n });
	sprite->AddKeyFrame((int)Player2Anim::LEVITATING_LEFT, { 0, 6 * n, n, n });

	sprite->SetAnimationDelay((int)Player2Anim::ATTACK_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)Player2Anim::ATTACK_RIGHT, { 0, 1 * n, -n, n });
	sprite->AddKeyFrame((int)Player2Anim::ATTACK_RIGHT, { 8 * n, 1 * n, -n, n });
	sprite->AddKeyFrame((int)Player2Anim::ATTACK_RIGHT, { 8 * n, 1 * n, -n, n });
	sprite->AddKeyFrame((int)Player2Anim::ATTACK_RIGHT, { 8 * n, 1 * n, -n, n });
	sprite->SetAnimationDelay((int)Player2Anim::ATTACK_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)Player2Anim::ATTACK_LEFT, { 0, 1 * n, n, n });
	sprite->AddKeyFrame((int)Player2Anim::ATTACK_LEFT, { 1 * n, 1 * n, n, n });
	sprite->AddKeyFrame((int)Player2Anim::ATTACK_LEFT, { 2 * n, 1 * n, n, n });
	sprite->AddKeyFrame((int)Player2Anim::ATTACK_LEFT, { 3 * n, 1 * n, n, n });



	sprite->SetAnimation((int)Player2Anim::IDLE_RIGHT);

	return AppStatus::OK;
}
void Player2::InitScore()
{
	score = 0;
}
void Player2::IncrScore(int n)
{
	score += n;
}
int Player2::GetScore()
{
	return score;
}
void Player2::GetHit()
{

	lifes--;
}
void Player2::LooseCondition()
{
	if (lifes < 0)
	{

	}
}
void Player2::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
bool Player2::IsLookingRight() const
{
	return look == Look::RIGHT;
}
bool Player2::IsLookingLeft() const
{
	return look == Look::LEFT;
}
bool Player2::IsAscending() const
{
	return dir.y < -PLAYER2_LEVITATING_SPEED;
}
bool Player2::IsLevitating() const
{
	return abs(dir.y) <= PLAYER2_LEVITATING_SPEED;
}
bool Player2::IsDescending() const
{
	return dir.y > PLAYER2_LEVITATING_SPEED;
}
bool Player2::IsInFirstHalfTile() const
{
	return pos.y % TILE_SIZE < TILE_SIZE / 2;
}
bool Player2::IsInSecondHalfTile() const
{
	return pos.y % TILE_SIZE >= TILE_SIZE / 2;
}
void Player2::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
Player2Anim Player2::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (Player2Anim)sprite->GetAnimation();
}
void Player2::Stop()
{
	dir = { 0,0 };
	state = State::IDLE;
	if (IsLookingRight())	SetAnimation((int)Player2Anim::IDLE_RIGHT);
	else					SetAnimation((int)Player2Anim::IDLE_LEFT);
}
void Player2::StartWalkingLeft()
{
	state = State::WALKING;
	look = Look::LEFT;
	SetAnimation((int)Player2Anim::WALKING_LEFT);
}
void Player2::StartWalkingRight()
{
	state = State::WALKING;
	look = Look::RIGHT;
	SetAnimation((int)Player2Anim::WALKING_RIGHT);
}
void Player2::StartFalling()
{
	dir.y = PLAYER2_SPEED;
	state = State::FALLING;
	if (IsLookingRight())	SetAnimation((int)Player2Anim::FALLING_RIGHT);
	else					SetAnimation((int)Player2Anim::FALLING_LEFT);
}
void Player2::StartJumping()
{
	dir.y = -PLAYER2_JUMP_FORCE;
	state = State::JUMPING;
	if (IsLookingRight())	SetAnimation((int)Player2Anim::JUMPING_RIGHT);
	else					SetAnimation((int)Player2Anim::JUMPING_LEFT);
	jump_delay = PLAYER2_JUMP_DELAY;
}
void Player2::StartAttacking()
{

	state = State::ATTACKING;
	if (IsLookingRight())	SetAnimation((int)Player2Anim::ATTACK_RIGHT);
	else					SetAnimation((int)Player2Anim::ATTACK_LEFT);
}
void Player2::ChangeAnimRight()
{
	look = Look::RIGHT;
	switch (state)
	{
	case State::IDLE:	 SetAnimation((int)Player2Anim::IDLE_RIGHT);    break;
	case State::WALKING: SetAnimation((int)Player2Anim::WALKING_RIGHT); break;
	case State::JUMPING: SetAnimation((int)Player2Anim::JUMPING_RIGHT); break;
	case State::FALLING: SetAnimation((int)Player2Anim::FALLING_RIGHT); break;
	case State::ATTACKING: SetAnimation((int)Player2Anim::ATTACK_RIGHT); break;
	}
}
void Player2::ChangeAnimLeft()
{
	look = Look::LEFT;
	switch (state)
	{
	case State::IDLE:	 SetAnimation((int)Player2Anim::IDLE_LEFT);    break;
	case State::WALKING: SetAnimation((int)Player2Anim::WALKING_LEFT); break;
	case State::JUMPING: SetAnimation((int)Player2Anim::JUMPING_LEFT); break;
	case State::FALLING: SetAnimation((int)Player2Anim::FALLING_LEFT); break;
	case State::ATTACKING: SetAnimation((int)Player2Anim::ATTACK_LEFT); break;
	}
}
void Player2::Update()
{
	//Player doesn't use the "Entity::Update() { pos += dir; }" default behaviour.
	//Instead, uses an independent behaviour for each axis.
	MoveX();
	MoveY();
	LooseCondition();


	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
	connect();

}
void Player2::MoveX()
{
	AABB box;
	int prev_x = pos.x;

	//We can only go up and down while climbing


	if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT))
	{
		pos.x += -PLAYER2_SPEED;
		if (state == State::IDLE || state == State::ATTACKING) StartWalkingLeft();
		else
		{
			if (IsLookingRight()) ChangeAnimLeft();
		}

		box = GetHitbox();
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
		}
	}
	else if (IsKeyDown(KEY_RIGHT))
	{
		pos.x += PLAYER2_SPEED;
		if (state == State::IDLE || state == State::ATTACKING) StartWalkingRight();
		else
		{
			if (IsLookingLeft()) ChangeAnimRight();
		}

		box = GetHitbox();
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
		}
	}
	else if (IsKeyDown(KEY_F) && state != State::JUMPING)
	{
		StartAttacking();
	}
	else
	{
		if (state == State::WALKING) Stop();
	}
}
void Player2::MoveY()
{
	AABB box;

	if (state == State::JUMPING)
	{
		LogicJumping();
	}
	else //idle, walking, falling
	{
		pos.y += PLAYER2_SPEED;
		box = GetHitbox();
		if (map->TestCollisionGround(box, &pos.y))
		{
			if (state == State::FALLING) Stop();
			else if (IsKeyPressed(KEY_SPACE))
			{
				PlaySound(soundEffectsplayer2[0]);
				StartJumping();

			}
		}
		else
		{
			if (state != State::FALLING) StartFalling();
		}
	}
}
void Player2::LogicJumping()
{
	AABB box, prev_box;
	int prev_y;

	jump_delay--;
	if (jump_delay == 0)
	{
		prev_y = pos.y;
		prev_box = GetHitbox();

		pos.y += dir.y;
		dir.y += GRAVITY2_FORCE;
		jump_delay = PLAYER2_JUMP_DELAY;

		//Is the jump finished?
		if (dir.y > PLAYER2_JUMP_FORCE)
		{
			dir.y = PLAYER2_SPEED;
			StartFalling();
		}
		else
		{
			//Jumping is represented with 3 different states
			if (IsAscending())
			{
				if (IsLookingRight())	SetAnimation((int)Player2Anim::JUMPING_RIGHT);
				else					SetAnimation((int)Player2Anim::JUMPING_LEFT);
			}
			else if (IsLevitating())
			{
				if (IsLookingRight())	SetAnimation((int)Player2Anim::LEVITATING_RIGHT);
				else					SetAnimation((int)Player2Anim::LEVITATING_LEFT);
			}
			else if (IsDescending())
			{
				if (IsLookingRight())	SetAnimation((int)Player2Anim::FALLING_RIGHT);
				else					SetAnimation((int)Player2Anim::FALLING_LEFT);
			}
		}
		//We check ground collision when jumping down
		if (dir.y >= 0)
		{
			box = GetHitbox();

			//A ground collision occurs if we were not in a collision state previously.
			//This prevents scenarios where, after levitating due to a previous jump, we found
			//ourselves inside a tile, and the entity would otherwise be placed above the tile,
			//crossing it.
			if (!map->TestCollisionGround(prev_box, &prev_y) &&
				map->TestCollisionGround(box, &pos.y))
			{
				Stop();
			}
		}
	}
}

void Player2::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);

	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 18 * 16, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}
void Player2::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER);

	render->Release();
}
