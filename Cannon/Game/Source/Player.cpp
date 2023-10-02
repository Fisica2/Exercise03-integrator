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

    if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
    {
        initialPosition = position;
        float radians = angle * M_PI / 180.0f;
        initialVelocity = { speedX * cosf(radians), -speedY * sinf(radians) };
        gravity = -0.00098f;
        totalTime = 0.0f;
        numBounces = 0;
    }

    float t = totalTime + dt;
    float x = initialPosition.x + initialVelocity.x * t;
    float y = initialPosition.y + initialVelocity.y * t - 0.5f * gravity * t * t;

    if (y < initialPosition.y)
    {
        position = { x, y };
        rotation += 3.0f;
    }
    else
    {
        rotation += 0.0f;
        if (numBounces < 12)
        {
            initialPosition = position;
            initialVelocity.y = -initialVelocity.y * 0.85f;
            initialVelocity.x = -initialVelocity.x * 0.95f;
            totalTime = 0.0f;
            numBounces++;
        }
    }

    app->render->DrawTexture(texture, position.x, position.y, NULL, 1.0f, rotation);

    totalTime += dt;

    printf("\r speedX: %.2f, speedY: %.2f, x: %.2f, y: %.2f, angle: %0.f", speedX, speedY, position.x, position.y, angle);
    return true;
}


bool Player::CleanUp()
{

	return true;
}