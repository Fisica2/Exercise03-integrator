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

public:

	//L02: TODO 2: Declare player parameters
	float speed = 0.2f;
	SDL_Texture* texture = NULL;

private:

	float totalTime = 0.0f;
	fPoint initialPosition = { 0, 0 };
	fPoint initialVelocity = { 0, 0 };
	float gravity = 0.0f;
	int numBounces = 0;
	bool hasJumped = false;
};

#endif // __PLAYER_H__