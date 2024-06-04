#include "Object.h"
#include "StaticImage.h"

Object::Object(const Point& p, ObjectType t) : Entity(p, OBJECT_PHYSICAL_SIZE, OBJECT_PHYSICAL_SIZE, OBJECT_FRAME_SIZE, OBJECT_FRAME_SIZE)
{
	img_items = nullptr;
	type = t; 
	
	Rectangle rc;
	const int n = TILE_SIZE;
	switch (type)
	{
		case ObjectType::POTION: rc = { 26 * n, 4 * n, n, n }; break;
		case ObjectType::KEY: rc = { 27 * n, 4 * n, n, n }; break;
		case ObjectType::BAG: rc = { 28 * n, 4 * n, n, n }; break;
			
		default: LOG("Internal error: object creation of invalid type");
	}

	ResourceManager& data = ResourceManager::Instance();
	render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);

	fxItems[0] = LoadSound("sound/fx/RecollectObj.wav");
	fxItems[1] = LoadSound("sound/fx/GetKey.wav");
	

}
Object::~Object()
{
}
void Object::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
int Object::Points() const
{
	if (type == ObjectType::POTION)		return POINTS_POTION;
	else if (type == ObjectType::KEY)	return POINTS_KEY;
	else
	{
		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}
int Object::Experience() const
{
	if (type == ObjectType::POTION)		return EXP_POTION;
	else if (type == ObjectType::KEY)	return EXP_KEY;
	else if (type == ObjectType::BAG)	return EXP_BAG;
	else
	{
		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}
int Object::Vitality() const
{
	if (type == ObjectType::POTION)		return VIT_POTION;

	else
	{
		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}
 void Object::SoundItem() const
{
	if (type == ObjectType::KEY)	return PlaySound(fxItems[1]);
	else
	{
		return PlaySound(fxItems[0]);
	}
}