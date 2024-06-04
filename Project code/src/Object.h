#pragma once
#include "Entity.h"

#define OBJECT_PHYSICAL_SIZE	8
#define OBJECT_FRAME_SIZE		8


#define POINTS_POTION	500
#define POINTS_KEY	200

#define EXP_POTION  20
#define EXP_KEY  10
#define EXP_BAG  30

#define VIT_POTION  30




enum class ObjectType { POTION, KEY, BAG };

class Object : public Entity
{
public:
	Object(const Point& p, ObjectType t);
	~Object();

	AppStatus Initialise();

	void DrawDebug(const Color& col) const;
	int Points() const;
	int Experience() const;
	int Vitality() const;
	void SoundItem() const;

private:

	Sound fxItems[2];
	ObjectType type;
	const Texture2D* img_items;
};

