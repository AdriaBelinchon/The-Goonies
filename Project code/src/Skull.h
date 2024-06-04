#pragma once
#include "Enemy.h"
#include "TileMap.h"
#include "Entity.h"

#define SKULL_SPEED			1
#define SKULL_ANIM_DELAY	3

enum class SkullState { ROAMING, IDLE };
enum class SkullAnim {
	IDLE_LEFT, IDLE_RIGHT, WALKING_LEFT, WALKING_RIGHT,
	NUM_ANIMATIONS
};

struct Step
{
	Point speed;	//direction
	int frames;		//duration in number of frames
	int anim;		//graphical representation
};

class Skull : public Enemy
{
public:
	Skull(const Point& p, int width, int height, int frame_width, int frame_height);
	~Skull();

	//Initialize the enemy with the specified look and area
	AppStatus Initialise(Look look, const AABB& area) override;

	//Update the enemy according to its logic, return true if the enemy must shoot
	bool Update(const AABB& box) override;
	
private:
	//Create the pattern behaviour
	void InitPattern();
	bool CollidesWithWall();
	TileMap *map;
	void Stop();
	void SetAnimation(int id);

	//Update looking direction according to the current step of the pattern
	void UpdateLook(int anim_id);

	SkullState state;

	int current_step;	//current step of the pattern
	int current_frames;	//number of frames in the current step
	std::vector<Step> pattern;
};

