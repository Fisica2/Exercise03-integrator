#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"

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

    if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
    {
        initialPosition = position;
        float angle = 45.0f;
        float speedX = 0.4f;
        float speedY = 0.9f;
        float radians = angle * M_PI / 180.0f;
        initialVelocity = { speedX * cosf(radians), -speedY * sinf(radians) };
        gravity = -0.00098f;
        totalTime = 0.0f;
        numBounces = 0;
    }

    float t = totalTime + dt;
    float x = initialPosition.x + initialVelocity.x * t;
    float y = initialPosition.y + initialVelocity.y * t - 0.5f * gravity * t * t;

    if (y < initialPosition.y) // La bola está en el aire
    {
        position = { x, y };
    }
    else // La bola ha tocado el suelo
    {
        if (numBounces < 12) // Definir MAX_BOUNCES según tus necesidades
        {
            initialPosition = position;
            initialVelocity.y = -initialVelocity.y * 0.85f; // Factor de rebote
            initialVelocity.x = -initialVelocity.x * 0.95f;
            totalTime = 0.0f;
            numBounces++;
        }
    }

    app->render->DrawTexture(texture, position.x, position.y);

    totalTime += dt;

    printf("\r x: %.2f, y: %.2f", position.x, position.y);
    return true;
}


bool Player::CleanUp()
{

	return true;
}