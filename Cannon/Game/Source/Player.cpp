#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Math.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	//L03: TODO 2: Initialize Player parameters
	position = fPoint(0, 300);
    initialPosition = position;
	return true;
}

bool Player::Start() {
	texture = app->tex->Load("Assets/Textures/ball.png");
	return true;
}

bool Player::Update(float dt)
{
    Scene* scene = app->scene;

    if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
    {
        angle += 5;
    }
    if (app->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
    {
		angle -= 5;
	}
    if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
    {
        speedX -= 0.1f;
        speedY -= 0.1f;
    }
    if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
    {
        speedX += 0.1f;
        speedY += 0.1f;
    }

    if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
    {
        position = fPoint(0, 300);
        scene->hit = false;
    }

    if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
    {
        initialPosition = position;
        float radians = angle * M_PI / 180.0f;
        initialVelocity = { speedX * cosf(radians), -speedY * sinf(radians) };
        gravity = -0.00098f;
        totalTime = 0.0f;
        numBounces = 0;
        hasJumped = true;
    }

    float t = totalTime + dt;
    x = initialPosition.x + initialVelocity.x * t;
    y = initialPosition.y + initialVelocity.y * t - 0.5f * gravity * t * t;

    if (y < 375 && hasJumped)
    {
        position = { x, y };
        rotation += 3.0f;
    }
    else
    {
        rotation += 0.0f;
        if (numBounces < 8)
        {
            initialPosition = position;
            initialVelocity.y = -initialVelocity.y * 0.85f;
            initialVelocity.x = -initialVelocity.x * 0.95f;
            totalTime = 0.0f;
            numBounces++;
        }
    }

    SDL_Rect enemyRect = { 600, 315, 50, 50 }; // Example enemy position and size
    if (CheckCollision(enemyRect))
    {
        scene->hit = true;
    }

    app->render->DrawTexture(texture, position.x, position.y, NULL, 1.0f, rotation);

    totalTime += dt;

    printf("\r speedX: %.2f, speedY: %.2f, x: %.2f, y: %.2f, angle: %0.f", speedX, speedY, position.x, position.y, angle);
    return true;
}

bool Player::CheckCollision(SDL_Rect enemyRect)
{
    SDL_Rect playerRect = { (int)position.x, (int)position.y, 30, 30 };
    return SDL_HasIntersection(&playerRect, &enemyRect);
}


bool Player::CleanUp()
{

	return true;
}