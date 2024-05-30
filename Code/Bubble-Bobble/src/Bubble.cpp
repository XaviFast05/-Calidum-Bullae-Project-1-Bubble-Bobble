#include "Bubble.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Bubble::Bubble(const Point& p, Directions d) : Entity(p, BUBBLE_PHYSICAL_SIZE, BUBBLE_PHYSICAL_SIZE, BUBBLE_FRAME_SIZE, BUBBLE_FRAME_SIZE)
{
	dire = d;
	speed = .3;
	stages = 1;
	logPosXL = pos.x - SHOOT_RANGE;
	logPosXR = pos.x + SHOOT_RANGE;
	eTime = 0;
	spawnTime = 0;
	player = nullptr;
	lifeTime = GetRandomValue(3, 5);
	Rectangle rc;
	inShoot = true;
	eTimePogo = 0;

}
Bubble::~Bubble()
{
}
void Bubble::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
AppStatus Bubble::Initialise()
{
	Rectangle rc;
	const int n = 16;
	const int p = PADDINGG_X;
	const int y = 22;

	ResourceManager& data = ResourceManager::Instance();
	data.LoadTexture(Resource::IMG_BUBBLE, "images/bubbles.png");

	render = new Sprite(data.GetTexture(Resource::IMG_BUBBLE));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}
	Sprite* sprite = dynamic_cast<Sprite*>(render);

	sprite->SetNumberAnimations((int)BubbleAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)BubbleAnim::IDLE, ANIM_DELAY);
	sprite->AddKeyFrame((int)BubbleAnim::IDLE, { n, 0, n, n });
	sprite->AddKeyFrame((int)BubbleAnim::IDLE, { n, 0, n, n });

	sprite->SetAnimationDelay((int)BubbleAnim::INSHOOT, ANIM_DELAY + 6);
	for (int i = 2; i < 8; ++i)
		sprite->AddKeyFrame((int)BubbleAnim::INSHOOT, { (float)i * n, 7*n, n, n });

	sprite->SetAnimation((int)BubbleAnim::INSHOOT);
	return AppStatus::OK;

}
void Bubble::Update()
{
	pos += dir;
	Movement(dire);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
bool Bubble::isAlive()
{
	eTime += GetFrameTime();
	if (eTime >= lifeTime)
	{
		return false;
	}
	else {
		return true;
	}
}
void Bubble::ClampPos()
{

	if (pos.y < 32)
	{
		if (pos.x <= WINDOW_WIDTH / 2)
		{
			dir = { 1, 1 };
		}
		else {
			dir = { -1, 1 };
		}
	}
	if (pos.y == 32)
	{
		if (pos.x <= GetRandomValue(110, WINDOW_WIDTH / 2))
		{
			dir = { 1, 0 };
		}
		else if (pos.x > GetRandomValue(WINDOW_WIDTH / 2, 140))
		{
			dir = { -1, 0 };
		}
	}

}
void Bubble::SetPlayer(Player* p)
{
	player = p;
}

void Bubble::Movement(Directions d)
{
	ClampPos();
	
	if (pos.y > 32)
	{
		if (d == Directions::LEFT)
		{
			switch (stages) {
				SetAnimation((int)BubbleAnim::INSHOOT);
			case 1:
				if (pos.x < 20)
				{
					pos.x++;
					stages++;
				}
				inShoot = true;

				dir = { -2, 0 };
				if (pos.x <= logPosXL) {
					stages++;
				}
				break;
			case 2:
				SetAnimation((int)BubbleAnim::IDLE);

				inShoot = false;
				dir = { 0, -1 };
				break;


			}
		}
		else if (d == Directions::RIGHT)
		{

			switch (stages) {
				SetAnimation((int)BubbleAnim::INSHOOT);
			case 1:
				if (pos.x > 226)
				{
					pos.x--;
					stages++;
				}
				inShoot = true;

				dir = { 2, 0 };
				if (pos.x >= logPosXR) {
					stages++;
				}
				break;
			case 2:
				SetAnimation((int)BubbleAnim::IDLE);

				inShoot = false;
				dir = { 0, -1 };

				break;

			}

		}
	}

}
void Bubble::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
void Bubble::Release()
{

	render->Release();
}
