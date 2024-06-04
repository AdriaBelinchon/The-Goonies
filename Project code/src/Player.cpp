
#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Player::Player(const Point& p, State s, Look view) :
	Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE)
{
	state = s;
	look = view;
	jump_delay = PLAYER_JUMP_DELAY;
	map = nullptr;
	score = 0;
	vitality = 100;
	experience = 0;
	fxPlayer[0] = {};
}
Player::~Player()
{
}
AppStatus Player::Initialise()
{
	
	const int n = PLAYER_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_PLAYER, "images/sloth.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	fxPlayer[0] = LoadSound("sound/fx/Jump.wav");

	render = new Sprite(data.GetTexture(Resource::IMG_PLAYER));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)PlayerAnim::NUM_ANIMATIONS);
	
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_RIGHT, { n-3, n+1, n-3, n });
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_LEFT, { n-3, n+1, -(n-3), n });

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_RIGHT, ANIM_DELAY);
	for (int i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_RIGHT, { (float)i*(n-3), n+1, n-3, n });
	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_LEFT, ANIM_DELAY);
	for (int i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_LEFT, { (float)i*(n-3), n+1, -(n-3), n });

	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_RIGHT, { n-3, n+1, n-3, n });
	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_LEFT, { n-3, n + 1, -(n - 3), n });

	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_RIGHT, { 0, n*2+1, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_LEFT, { 0, n*2+1, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING, ANIM_LADDER_DELAY);
	for (int i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::CLIMBING, { (float)i * 10, 0, 10, n+1 });
	
	sprite->SetAnimationDelay((int)PlayerAnim::ATTACK_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::ATTACK_RIGHT, { 0, n * 3 + 1, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::ATTACK_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::ATTACK_LEFT, { 0, n * 3 + 1, -n, n });


		
	sprite->SetAnimation((int)PlayerAnim::IDLE_RIGHT);

	return AppStatus::OK;
}
void Player::InitScore()
{
	score = 0;
}
void Player::InitVitality()
{
	vitality = 100;
}
void Player::InitExperience()
{
	experience = 0;
}
void Player::IncrScore(int n)
{
	score += n;
}
int Player::GetScore()
{
	return score;
}
void Player::ModVitality(int vit)
{
	vitality += vit;
	if (vitality >= 100) vitality = 100;
	else if (vitality <= 0) vitality = 0;
}
int Player::GetVitality()
{
	return vitality;
}
void Player::ModExperience(int exp)
{
	experience += exp;
	if (experience >= 100) experience = 100;
	else if (experience <= 0) experience = 0;
}
int Player::GetExperience() 
{
	return experience;
}
int Player::GetPosX()
{
	return GetRenderingPositionP().x; 
}
int Player::GetPosY()
{
	return GetRenderingPositionP().y;
}
void Player::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
bool Player::IsLookingRight() const
{
	return look == Look::RIGHT;
}
bool Player::IsLookingLeft() const
{
	return look == Look::LEFT;
}
bool Player::IsAscending() const
{
	return dir.y < -PLAYER_LEVITATING_SPEED;
}
bool Player::IsLevitating() const
{
	return abs(dir.y) <= PLAYER_LEVITATING_SPEED;
}
bool Player::IsDescending() const
{
	return dir.y > PLAYER_LEVITATING_SPEED;
}
bool Player::IsInFirstHalfTile() const
{
	return pos.y % TILE_SIZE < TILE_SIZE / 2;
}
bool Player::IsInSecondHalfTile() const
{
	return pos.y % TILE_SIZE >= TILE_SIZE/2;
}


void Player::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
PlayerAnim Player::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (PlayerAnim)sprite->GetAnimation();
}
void Player::Stop()
{
	dir = { 0,0 };
	state = State::IDLE;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::IDLE_RIGHT);
	else					SetAnimation((int)PlayerAnim::IDLE_LEFT);
}
void Player::StartWalkingLeft()
{
	state = State::WALKING;
	look = Look::LEFT;
	SetAnimation((int)PlayerAnim::WALKING_LEFT);
}
void Player::StartWalkingRight()
{
	state = State::WALKING;
	look = Look::RIGHT;
	SetAnimation((int)PlayerAnim::WALKING_RIGHT);
}
void Player::StartFalling()
{
	dir.y = PLAYER_SPEED;
	state = State::FALLING;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::FALLING_RIGHT);
	else					SetAnimation((int)PlayerAnim::FALLING_LEFT);
}
void Player::StartJumping()
{
	PlaySound(fxPlayer[0]);
	dir.y = -PLAYER_JUMP_FORCE;
	state = State::JUMPING;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::JUMPING_RIGHT);
	else					SetAnimation((int)PlayerAnim::JUMPING_LEFT);
	jump_delay = PLAYER_JUMP_DELAY;
}
void Player::StartClimbingUp()
{
	state = State::CLIMBING;
	SetAnimation((int)PlayerAnim::CLIMBING);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
}
void Player::StartClimbingDown()
{
	state = State::CLIMBING;
	SetAnimation((int)PlayerAnim::CLIMBING);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
}
void Player::StartAttack()
{
	
	state = State::ATTACK;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::ATTACK_RIGHT);
	else					SetAnimation((int)PlayerAnim::ATTACK_LEFT);
	jump_delay = PLAYER_JUMP_DELAY;
}
void Player::ChangeAnimRight()
{
	look = Look::RIGHT;
	switch (state)
	{
		case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_RIGHT);    break; 
		case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_RIGHT); break;
		case State::JUMPING: SetAnimation((int)PlayerAnim::JUMPING_RIGHT); break;
		case State::FALLING: SetAnimation((int)PlayerAnim::FALLING_RIGHT); break;
		case State::ATTACK: SetAnimation((int)PlayerAnim::ATTACK_RIGHT); break;

	}
}
void Player::ChangeAnimLeft()
{
	look = Look::LEFT;
	switch (state)
	{
		case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_LEFT);    break;
		case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_LEFT); break;
		case State::JUMPING: SetAnimation((int)PlayerAnim::JUMPING_LEFT); break;
		case State::FALLING: SetAnimation((int)PlayerAnim::FALLING_LEFT); break;
		case State::ATTACK: SetAnimation((int)PlayerAnim::ATTACK_LEFT); break;
	}
}
void Player::Update()
{
	//Player doesn't use the "Entity::Update() { pos += dir; }" default behaviour.
	//Instead, uses an independent behaviour for each axis.
	MoveX();
	MoveY();

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void Player::MoveX()
{
	AABB box;
	int prev_x = pos.x;

	//We can only go up and down while climbing
	if (state == State::CLIMBING)	return;
	if (state != State::FALLING) 
	{
		if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT))
		{
			pos.x += -PLAYER_SPEED;
			if (state == State::IDLE) StartWalkingLeft();
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
			pos.x += PLAYER_SPEED;
			if (state == State::IDLE) StartWalkingRight();
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
		else if (IsKeyDown(KEY_E)) {

			if (state == State::IDLE || state == State::WALKING) StartAttack();

			state = State::WALKING;

		}
		else
		{
			if (state == State::WALKING) Stop();
		}

	}
	
}
void Player::MoveY()
{
	AABB box;

	if (state == State::JUMPING)
	{
		LogicJumping();
	}
	else if (state == State::CLIMBING)
	{
		LogicClimbing();
	}
	else //idle, walking, falling
	{
		pos.y += PLAYER_SPEED;
		box = GetHitbox();
		if (map->TestCollisionGround(box, &pos.y))
		{
			if (state == State::FALLING) Stop();

			if (IsKeyDown(KEY_UP))
			{
				box = GetHitbox();
				if (map->TestOnLadder(box, &pos.x)) {

					StartClimbingUp();
					pos.y += PLAYER_LADDER_SPEED;

				}
				else if (map->TestOnLadderBottom(box, &pos.x)) {

					StartClimbingUp();
					pos.y += PLAYER_LADDER_SPEED;
				}

			}
			else if(IsKeyDown(KEY_SPACE)) {

				StartJumping();
				

			}
					
			
			else if (IsKeyDown(KEY_DOWN))
			{
				//To climb up the ladder, we need to check the control point (x, y)
				//To climb down the ladder, we need to check pixel below (x, y+1) instead
				box = GetHitbox();
				box.pos.y++;
				if (map->TestOnLadderTop(box, &pos.x))
				{
					StartClimbingDown();
					pos.y += PLAYER_LADDER_SPEED;
				}


				else if (map->TestOnLadderBottom(box, &pos.x))
				{
					StartClimbingUp();
					pos.y += PLAYER_LADDER_SPEED;
				}
					
			}
			
		}
		else
		{
			if (state != State::FALLING) StartFalling();
		}
		if (pos.y > 160) pos.y = 160;

	}
}
void Player::LogicJumping()
{
	AABB box, prev_box;
	int prev_y;

	jump_delay--;
	if (jump_delay == 0)
	{
		prev_y = pos.y;
		prev_box = GetHitbox();

		pos.y += dir.y;
		dir.y += GRAVITY_FORCE;
		jump_delay = PLAYER_JUMP_DELAY;

		//Is the jump finished?
		if (dir.y > PLAYER_JUMP_FORCE)
		{
			dir.y = PLAYER_SPEED;
			StartFalling();
		}
		else
		{
			if (IsLookingRight())	SetAnimation((int)PlayerAnim::JUMPING_RIGHT);
			else					SetAnimation((int)PlayerAnim::JUMPING_LEFT);
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
void Player::LogicClimbing()
{
	AABB box;
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	int tmp;

	if (IsKeyDown(KEY_UP))
	{
		pos.y -= PLAYER_LADDER_SPEED;
		sprite->NextFrame();
	}
	else if (IsKeyDown(KEY_DOWN))
	{
		pos.y += PLAYER_LADDER_SPEED;
		sprite->PrevFrame();
	}

	//It is important to first check LadderTop due to its condition as a collision ground.
	//By doing so, we ensure that we don't stop climbing down immediately after starting the descent.
	box = GetHitbox();
	if (map->TestOnLadderTop(box, &tmp))
	{
		if (IsInFirstHalfTile()) SetAnimation((int)PlayerAnim::CLIMBING);
		/*if (IsInSecondHalfTile())		SetAnimation((int)PlayerAnim::CLIMBING_PRE_TOP);*/
		/*else if (IsInFirstHalfTile())	SetAnimation((int)PlayerAnim::CLIMBING_TOP);*/
		else					LOG("Internal error, tile should be a LADDER_TOP, coord: (%d,%d)", box.pos.x, box.pos.y);
	}
	else if (map->TestCollisionGround(box, &pos.y))
	{
		//Case leaving the ladder descending.
		Stop();
		sprite->SetAutomaticMode();
	}
	else if (!map->TestOnLadder(box, &tmp))
	{
		//Case leaving the ladder ascending.
		//If we are not in a LadderTop, colliding ground or in the Ladder it means we are leaving
		//ther ladder ascending.
		Stop();
		sprite->SetAutomaticMode();
	}
	else
	{
		if (GetAnimation() != PlayerAnim::CLIMBING)	SetAnimation((int)PlayerAnim::CLIMBING);
	}
}
void Player::LogicAttack()
{
	
	jump_delay--;

	if (jump_delay == 0)
	{
		jump_delay = PLAYER_JUMP_DELAY;	
	}
}

void Player::DrawDebug(const Color& col) const
{	
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
	
	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 18*16, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}
void Player::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER);

	render->Release();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//Enemy::Enemy(const Point& p, State s, Look view) :
//	Entity(p, ENEMY_PHYSICAL_WIDTH, ENEMY_PHYSICAL_HEIGHT, ENEMY_FRAME_SIZE, ENEMY_FRAME_SIZE)
//{
//	state = s;
//	look = view;
//	map = nullptr;
//}
//Enemy::~Enemy()
//{
//}
//AppStatus Enemy::Initialise()
//{
//	int i;
//	const int n = ENEMY_FRAME_SIZE;
//	ResourceManager& data = ResourceManager::Instance();
//	if (data.LoadTexture(Resource::IMG_ENEMY, "//PONER PNG DEL ENEMIGO") != AppStatus::OK)
//	{
//		return AppStatus::ERROR;
//	}
//	render = new Sprite(data.GetTexture(Resource::IMG_ENEMY));
//
//	if (render == nullptr)
//	{
//		LOG("Failed to allocate memory for Enemy sprite");
//		return AppStatus::ERROR;
//	}
//	Sprite* sprite = dynamic_cast<Sprite*>(render);
//	sprite->SetNumberAnimations((int)EnemyAnim::NUM_ANIMATIONS);
//	sprite->SetAnimationDelay((int)EnemyAnim::IDLE_RIGHT, ANIM_DELAY);
//	sprite->AddKeyFrame((int)EnemyAnim::IDLE_RIGHT, { 0, 0, n, n }); //MIRAR SI LAS STATS SON CORRECTAS
//	sprite->SetAnimationDelay((int)EnemyAnim::IDLE_LEFT, ANIM_DELAY);
//	sprite->AddKeyFrame((int)EnemyAnim::IDLE_LEFT, { 0, 0, -n, n });
//	sprite->SetAnimationDelay((int)EnemyAnim::WALKING_RIGHT, ANIM_DELAY);
//	for (i = 0; i < 8; ++i) sprite->AddKeyFrame((int)EnemyAnim::WALKING_RIGHT, { (float)i * n, 4 * n, n, n });
//	sprite->SetAnimationDelay((int)EnemyAnim::WALKING_LEFT, ANIM_DELAY);
//	for (i = 0; i < 8; ++i) sprite->AddKeyFrame((int)EnemyAnim::WALKING_LEFT, { (float)i * n, 4 * n, -n, n });
//	sprite->SetAnimation((int)EnemyAnim::IDLE_RIGHT);
//	return AppStatus::OK;
//}
//void Enemy::SetScore(int n)
//{
//	if (State::state == DEAD)
//	{
//		score +=  // MIRAR CANTIDAD DE SCORE DADA POR RL ENEMIGO Y SUMARSELA AL Enemy
//	}
//}
//
//void Enemy::SetTileMap(TileMap* tilemap)
//{
//	map = tilemap;
//}
//bool Enemy::IsInFirstHalfTile() const
//
//{
//
//	return pos.y % TILE_SIZE < TILE_SIZE / 2;
//
//}
//bool Enemy::IsInSecondHalfTile() const
//{
//	return pos.y % TILE_SIZE >= TILE_SIZE / 2;
//}
//void Enemy::SetAnimation(int id)
//{
//	Sprite* sprite = dynamic_cast<Sprite*>(render);
//	sprite->SetAnimation(id);
//}
//EnemyAnim Enemy::GetAnimation()
//{
//	Sprite* sprite = dynamic_cast<Sprite*>(render);
//	return (EnemyAnim)sprite->GetAnimation();
//}
//void Enemy::WalkingLeft()
//{
//	state = State::WALKING;
//	SetAnimation((int)EnemyAnim::WALKING_LEFT);
//}
//
//void Enemy::WalkingRight()
//{
//	state = State::WALKING;
//	SetAnimation((int)EnemyAnim::WALKING_RIGHT);
//}
//void Enemy::ChangeAnimRight()
//{
//	switch (state)
//	{
//	case State::IDLE:	 SetAnimation((int)EnemyAnim::IDLE_RIGHT);    break;
//	case State::WALKING: SetAnimation((int)EnemyAnim::WALKING_RIGHT); break;
//	}
//}
//void Enemy::ChangeAnimLeft()
//{
//	switch (state)
//	{
//	case State::IDLE: SetAnimation((int)EnemyAnim::IDLE_LEFT);    break;
//	case State::WALKING: SetAnimation((int)EnemyAnim::WALKING_LEFT); break;
//	}
//}
//void Enemy::DealDamage()
//{
//	Player.life--1;
//	void Enemy::Update()
//	{
//
//
//
//		MoveX();
//
//		MoveY();
//
//
//
//		Sprite* sprite = dynamic_cast<Sprite*>(render);
//
//		sprite->Update();
//
//	}
//}
//
//void Enemy::MoveX()
//{
//	AABB box;
//	int prev_x = pos.x;
//	box = GetHitbox();
//
//	if (map->TestCollisionWallLeft(box))
//
//	{
//
//		pos.x = prev_x;
//
//		if (state == State::WALKING)ChangeAnimRight();
//
//	}
//	else if (map->TestCollisionWallRight(box))
//	{
//		pos.x = prev_x;
//		if (state == State::WALKING)ChangeAnimLeft();
//
//	}
//
//	pos.x += ENEMY_SPEED;
//
//	if (state == State::IDLE) StartWalkingRight();
//
//	else
//
//	{
//
//		if (IsLookingLeft()) ChangeAnimRight();
//
//	}
//
//
//
//	box = GetHitbox();
//	if (enemy->TestCollisionPlayer(box))DealDamage();
//	
//
//}
//void Enemy::DrawDebug(const Color& col) const
//
//{
//	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
//	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 8 * 8, 0, 8, LIGHTGRAY);
//	DrawPixel(pos.x, pos.y, WHITE);
//}
//void Enemy::Release()
//{
//	ResourceManager& data = ResourceManager::Instance();
//	data.ReleaseTexture(Resource::IMG_Enemy);
//	render->Release();
//}