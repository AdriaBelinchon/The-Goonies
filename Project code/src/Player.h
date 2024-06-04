#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 32x32
#define PLAYER_FRAME_SIZE		16

//Logical model size: 12x28
#define PLAYER_PHYSICAL_WIDTH	6
#define PLAYER_PHYSICAL_HEIGHT	15

//Horizontal speed and vertical speed while falling down
#define PLAYER_SPEED			1

//Vertical speed while on a ladder
#define PLAYER_LADDER_SPEED		1

//Frame animation delay while on a ladder
#define ANIM_LADDER_DELAY		(2*ANIM_DELAY)

//When jumping, initial jump speed and maximum falling speed
#define PLAYER_JUMP_FORCE		6

//Frame delay for updating the jump velocity
#define PLAYER_JUMP_DELAY		4

//Player is levitating when abs(speed) <= this value
#define PLAYER_LEVITATING_SPEED	4

//Gravity affects jumping velocity when jump_delay is 0
#define GRAVITY_FORCE			1

//Logic states
enum class State { IDLE, WALKING, JUMPING, FALLING, ATTACK, CLIMBING, DEAD };

//Rendering states
enum class PlayerAnim {
	IDLE_LEFT, IDLE_RIGHT,
	WALKING_LEFT, WALKING_RIGHT,
	JUMPING_LEFT, JUMPING_RIGHT,
	LEVITATING_LEFT, LEVITATING_RIGHT,
	FALLING_LEFT, FALLING_RIGHT,
	ATTACK_RIGHT, ATTACK_LEFT,
	CLIMBING, 
	NUM_ANIMATIONS
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
	void InitVitality();
	int GetVitality();
	void ModVitality(int vit);
	void InitExperience();
	int GetExperience();
	void ModExperience(int exp);
    int GetPosX();
	int GetPosY();
	
	

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

private:
	bool IsLookingRight() const;
	bool IsLookingLeft() const;

	//Player mechanics
	void MoveX();
	void MoveY();
	void LogicJumping();
	void LogicClimbing();
	void LogicAttack();
	

	//Animation management
	void SetAnimation(int id);
	PlayerAnim GetAnimation();
	void Stop();
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartFalling();
	void StartJumping();
	void StartClimbingUp();
	void StartClimbingDown();
	void StartAttack();
	void ChangeAnimRight();
	void ChangeAnimLeft();

	//Jump steps
	bool IsAscending() const;
	bool IsLevitating() const;
	bool IsDescending() const;

	//Ladder get in/out steps
	bool IsInFirstHalfTile() const;
	bool IsInSecondHalfTile() const;

	Sound fxPlayer[2];

	State state;
	Look look;
	int jump_delay;

	TileMap *map;

	int score;
	int vitality;
	int experience;
};

//
//
////Representation model size: 32x32
//#define ENEMY_FRAME_SIZE		8
////Logical model size: 12x28
//#define ENEMY_PHYSICAL_WIDTH	6
//#define ENEMY_PHYSICAL_HEIGHT	2
//
////Logic states
//enum class State { IDLE, WALKING, DEAD };
//enum class Look { RIGHT, LEFT };
//
////Rendering states
//enum class EnemyAnim {
//
//	IDLE_LEFT, IDLE_RIGHT,
//	WALKING_LEFT, WALKING_RIGHT,
//	NUM_ANIMATIONS
//};
//class Enemy : public Entity
//{
//public:
//	Enemy(const Point& p, State s, Look view);
//	~Enemy();
//	AppStatus Initialise();
//	void SetTileMap(TileMap* tilemap);
//	void SetScore(int n);
//	void Update();
//	void DrawDebug(const Color& col) const;
//	void Release();
//
//	void DealDamage();
//
//
//private:
//	bool IsLookingRight() const;
//	bool IsLookingLeft() const;
//
//	//Enemy mechanics
//
//	void MoveX();
//	void MoveY();
//
//	//Animation management
//	void SetAnimation(int id);
//
//	EnemyAnim GetAnimation();
//	void WalkingLeft();
//	void WalkingRight();
//	void ChangeAnimRight();
//	void ChangeAnimLeft();
//
//	State state;
//	TileMap* map;
//	int score;
//
//};
//
