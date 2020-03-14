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

bool circleInternal(SDL_Rect character1Pos, SDL_Rect character2Pos)
{

	Vector2D vec = Vector2D((character1Pos.x - character2Pos.x),
		(character1Pos.y - character2Pos.y));

	double dist = sqrt((vec.x * vec.x) + (vec.y * vec.y));

	double combinedDistance = (30);

	return dist < combinedDistance;
}

bool collisions::Circle(character* character1, character* character2)
{
	SDL_Rect character1Pos = character1->getPosition();
	SDL_Rect character2Pos = character2->getPosition();

	return circleInternal(character1Pos, character2Pos);
}

bool collisions::Circle(character* character1, enemy* character2)
{
	SDL_Rect character1Pos = character1->getPosition();
	SDL_Rect character2Pos = character2->getPosition();

	return circleInternal(character1Pos, character2Pos);
}

bool boxInternal(SDL_Rect character1Pos, SDL_Rect character2Pos)
{
	return (character1Pos.x + (character1Pos.w / 2) > character2Pos.x &&
		character1Pos.x + (character1Pos.w / 2) < character2Pos.x + character2Pos.w &&
		character1Pos.y + (character1Pos.h / 2) > character2Pos.y &&
		character1Pos.y + (character1Pos.h / 2) < character2Pos.y + character2Pos.h);
}

bool collisions::Box(character* character1, character* character2)
{
	SDL_Rect character1Pos = character1->getRenderBox();
	SDL_Rect character2Pos = character2->getRenderBox();

	return boxInternal(character1Pos, character2Pos);
}

bool collisions::Box(character* character1, enemy* character2)
{
	SDL_Rect character1Pos = character1->getRenderBox();
	SDL_Rect character2Pos = character2->getRenderBox();

	return boxInternal(character1Pos, character2Pos);
}

collisions::~collisions()
{
	mInstance = NULL;
}