#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	bool CheckCollision(SDL_Rect enemyRect);

public:

	//L02: TODO 2: Declare player parameters
	float speed = 0.2f;
	SDL_Texture* texture = NULL;

public:

	float totalTime = 0.0f;
	fPoint initialPosition = { 0, 0 };
	fPoint initialVelocity = { 0, 0 };
	float gravity = 0.0f;
	int numBounces = 0;
	bool hasJumped = false;

	double rotation;
	float x, y;
	float angle = 45.0f;
	float speedX = 0.6f;
	float speedY = 0.9f;

	bool visible = false;
};

#endif // __PLAYER_H__