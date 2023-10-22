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

	bool CheckCollision(SDL_Rect Rect);
	int GetCollisionSide(SDL_Rect Rect);

public:

	//L02: TODO 2: Declare player parameters
	/*float speed = 0.2f;*/
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
	int circleRadius = 15;
	int numSegments = 36;
	bool visible = false;
	float vy = 0;
	int circleX = 0;
	int circleY = 0;

	SDL_Rect enemyRect = { 610, 315 + 100, 50, 85 };
	SDL_Rect platformRect = { 260, 400 + 100, 1200, 25 };
	SDL_Rect platformRect2 = { 460, 70 + 100, 300, 25 };

	bool isCollidingWithPlatform = false;
	bool debug = false;

	int colFace = 0; // 0 = no collision, 1 = top, 2 = bottom, 3 = left, 4 = right
	bool col = false;
	bool hasBounced = false;
	bool hasEntered = false;
};

#endif // __PLAYER_H__