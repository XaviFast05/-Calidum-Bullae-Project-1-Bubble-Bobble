
#include "SuperDrunk.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>
#include "Player.h"



Drunk::Drunk(const Point& p, D_State s, D_Look view) :
	Entity(p, DRUNK_PHYSICAL_WIDTH, DRUNK_PHYSICAL_HEIGHT, DRUNK_FRAME_SIZE, DRUNK_FRAME_SIZE)
{
	state = s;
	look = view;
	jump_delay = DRUNK_JUMP_DELAY;
	map = nullptr;
	score = 0;
	lifes = 3;
	player = nullptr;
}
Drunk::~Drunk()
{
}
AppStatus Drunk::Initialise()
{
	int i;
	const int n = DRUNK_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(Resource::IMG_SUPERDRUNK, "images/TrueSuperDrunk.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	render = new Sprite(data.GetTexture(Resource::IMG_SUPERDRUNK));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for enemy sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)DrunkAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)DrunkAnim::IDLE_RIGHT, ANIM_DELAY);

	sprite->AddKeyFrame((int)DrunkAnim::IDLE_RIGHT, { 0, 0, -n, n });
	sprite->AddKeyFrame((int)DrunkAnim::IDLE_RIGHT, { 0, 0, -n, n });


	sprite->SetAnimationDelay((int)DrunkAnim::IDLE_LEFT, ANIM_DELAY);

	sprite->AddKeyFrame((int)DrunkAnim::IDLE_LEFT, { 0, 0, n, n });
	sprite->AddKeyFrame((int)DrunkAnim::IDLE_LEFT, { 0, 0, n, n });



	sprite->SetAnimationDelay((int)DrunkAnim::WALKING_RIGHT, ANIM_DELAY);
	for (i = 1; i < 4; ++i)
	{
		sprite->AddKeyFrame((int)DrunkAnim::WALKING_RIGHT, { 0 * n, 0 * n, -n, n });
		sprite->AddKeyFrame((int)DrunkAnim::WALKING_RIGHT, { 1 * n, 0 * n, -n, n });
		sprite->AddKeyFrame((int)DrunkAnim::WALKING_RIGHT, { 2 * n, 0 * n, -n, n });
		sprite->AddKeyFrame((int)DrunkAnim::WALKING_RIGHT, { 3 * n, 0 * n, -n, n });
	}
	sprite->SetAnimationDelay((int)DrunkAnim::WALKING_LEFT, ANIM_DELAY);
	for (i = 1; i < 4; ++i)
	{
		sprite->AddKeyFrame((int)DrunkAnim::WALKING_LEFT, { 0 * n, 0 * n, n, n });
		sprite->AddKeyFrame((int)DrunkAnim::WALKING_LEFT, { 1 * n, 0 * n, n, n });
		sprite->AddKeyFrame((int)DrunkAnim::WALKING_LEFT, { 2 * n, 0 * n, n, n });
		sprite->AddKeyFrame((int)DrunkAnim::WALKING_LEFT, { 3 * n, 0 * n, n, n });
	}

	sprite->SetAnimationDelay((int)DrunkAnim::FALLING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)DrunkAnim::FALLING_RIGHT, { (float)i * n, 0 * n, -n, n });

	sprite->SetAnimationDelay((int)DrunkAnim::FALLING_LEFT, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)DrunkAnim::FALLING_LEFT, { (float)i * n, 0 * n, n, n });


	sprite->SetAnimationDelay((int)DrunkAnim::JUMPING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)DrunkAnim::JUMPING_RIGHT, { (float)i * n, 0 * n, -n, n });

	sprite->SetAnimationDelay((int)DrunkAnim::JUMPING_LEFT, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)DrunkAnim::JUMPING_LEFT, { (float)i * n, 0 * n, n, n });

	sprite->SetAnimationDelay((int)DrunkAnim::LEVITATING_RIGHT, ANIM_JUMP_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)DrunkAnim::LEVITATING_RIGHT, { (float)i * n, 0 * n, -n, n });
	sprite->SetAnimationDelay((int)DrunkAnim::LEVITATING_LEFT, ANIM_JUMP_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)DrunkAnim::LEVITATING_LEFT, { (float)i * n, 0 * n, n, n });


	sprite->SetAnimation((int)DrunkAnim::IDLE_RIGHT);

	return AppStatus::OK;
}
void Drunk::LifeManager()
{
	lifes--;
}
void Drunk::LooseCondition()
{
	if (lifes < 0)
	{

	}
}
void Drunk::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
void Drunk::SetPlayer(Player* play)
{
	player = play;
}
bool Drunk::IsLookingRight() const
{
	return look == D_Look::RIGHT;
}
bool Drunk::IsLookingLeft() const
{
	return look == D_Look::LEFT;
}
bool Drunk::IsAscending() const
{
	return dir.y < -DRUNK_LEVITATING_SPEED;
}
bool Drunk::IsLevitating() const
{
	return abs(dir.y) <= DRUNK_LEVITATING_SPEED;
}
bool Drunk::IsDescending() const
{
	return dir.y > DRUNK_LEVITATING_SPEED;
}
bool Drunk::IsInFirstHalfTile() const
{
	return pos.y % TILE_SIZE < TILE_SIZE / 2;
}
bool Drunk::IsInSecondHalfTile() const
{
	return pos.y % TILE_SIZE >= TILE_SIZE / 2;
}
void Drunk::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
DrunkAnim Drunk::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (DrunkAnim)sprite->GetAnimation();
}
void Drunk::Stop()
{
	dir = { 0,0 };
	state = D_State::IDLE;
	if (IsLookingRight())	SetAnimation((int)DrunkAnim::IDLE_RIGHT);
	else					SetAnimation((int)DrunkAnim::IDLE_LEFT);
}
void Drunk::StartWalkingLeft()
{
	state = D_State::WALKING;
	look = D_Look::LEFT;
	SetAnimation((int)DrunkAnim::WALKING_LEFT);
}
void Drunk::StartWalkingRight()
{
	state = D_State::WALKING;
	look = D_Look::RIGHT;
	SetAnimation((int)DrunkAnim::WALKING_RIGHT);
}
void Drunk::StartFalling()
{
	dir.y = DRUNK_SPEED;
	state = D_State::FALLING;
	if (IsLookingRight())	SetAnimation((int)DrunkAnim::FALLING_RIGHT);
	else					SetAnimation((int)DrunkAnim::FALLING_LEFT);
}
void Drunk::StartJumping()
{
	dir.y = -DRUNK_JUMP_FORCE;
	state = D_State::JUMPING;
	if (IsLookingRight())	SetAnimation((int)DrunkAnim::JUMPING_RIGHT);
	else					SetAnimation((int)DrunkAnim::JUMPING_LEFT);
	jump_delay = DRUNK_JUMP_DELAY;
}
void Drunk::StartAttacking()
{

	state = D_State::ATTACKING;
	if (IsLookingRight())	SetAnimation((int)DrunkAnim::ATTACK_RIGHT);
	else					SetAnimation((int)DrunkAnim::ATTACK_LEFT);
}
void Drunk::ChangeAnimRight()
{
	look = D_Look::RIGHT;
	switch (state)
	{
	case D_State::IDLE:	 SetAnimation((int)DrunkAnim::IDLE_RIGHT);    break;
	case D_State::WALKING: SetAnimation((int)DrunkAnim::WALKING_RIGHT); break;
	case D_State::JUMPING: SetAnimation((int)DrunkAnim::JUMPING_RIGHT); break;
	case D_State::FALLING: SetAnimation((int)DrunkAnim::FALLING_RIGHT); break;
	case D_State::ATTACKING: SetAnimation((int)DrunkAnim::ATTACK_RIGHT); break;
	}
}
void Drunk::ChangeAnimLeft()
{
	look = D_Look::LEFT;
	switch (state)
	{
	case D_State::IDLE:	 SetAnimation((int)DrunkAnim::IDLE_LEFT);    break;
	case D_State::WALKING: SetAnimation((int)DrunkAnim::WALKING_LEFT); break;
	case D_State::JUMPING: SetAnimation((int)DrunkAnim::JUMPING_LEFT); break;
	case D_State::FALLING: SetAnimation((int)DrunkAnim::FALLING_LEFT); break;
	case D_State::ATTACKING: SetAnimation((int)DrunkAnim::ATTACK_LEFT); break;
	}
}
void Drunk::Update()
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
void Drunk::MoveX()
{
	AABB box;
	int prev_x = pos.x;
	box = GetHitbox();
	if (look == D_Look::RIGHT)
	{
		pos.x += DRUNK_SPEED;
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			look = D_Look::LEFT;
			SetAnimation((int)DrunkAnim::WALKING_LEFT);
		}
	}
	else if (look == D_Look::LEFT)
	{
		pos.x += -DRUNK_SPEED;
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			look = D_Look::RIGHT;
			SetAnimation((int)DrunkAnim::WALKING_RIGHT);
		}
	}

}
void Drunk::MoveY()
{
	AABB box;
	int prev_y = pos.y;
	box = GetHitbox();
	if (DIAG_MOVE == Diag::DIAG_UP)
	{
		pos.y += -DRUNK_SPEED;
		if (pos.y <= 80)
		{
			pos.y = prev_y;
			DIAG_MOVE = Diag::DIAG_DOWN;
		}
	}
	else if (DIAG_MOVE == Diag::DIAG_DOWN)
	{
		pos.y += DRUNK_SPEED;
		if (pos.y >= 216)
		{
			pos.y = prev_y;
			DIAG_MOVE = Diag::DIAG_UP;
		}
	}
}


void Drunk::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);

	/*DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 10 * 10, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);*/
}
void Drunk::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_BUSTER);
	data.ReleaseTexture(Resource::IMG_SKELMON);

	render->Release();
}