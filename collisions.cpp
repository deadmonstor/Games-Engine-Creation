#include "collisions.h"
#include "character.h"

collisions* collisions::mInstance = NULL;

collisions::collisions()
{

}

collisions* collisions::Instance()
{
	if (!mInstance)
		mInstance = new collisions;

	return mInstance;
}

bool collisions::Circle(character* chracter1, character* character2)
{
	Vector2D vec = Vector2D((character1->GetPosition().x);

	double dist = sqrt((rect1.x * rect2.x) + (rect1.y * rect2.y));

	double combinedDistance = (30); // Change this later

	return dist < combinedDistance;
}

collisions::~collisions()
{
	mInstance = NULL;
}