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
	position = fPoint(0, 0);
    visible = false;
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

    if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && angle < 70)
    {
        angle += 5;
        scene->angle -= 5;
    }
    if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && angle > 20)
    {
		angle -= 5;
        scene->angle += 5;
	}
    if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && speedX > 0.4f)
    {
        speedX -= 0.1f;
        speedY -= 0.1f;
    }
    if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
    {
        speedX += 0.1f;
        speedY += 0.1f;
    }

    if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
    {
        scene->hit = false;
    }

    if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
    {
        float radians = angle * M_PI / 180.0f;
        float posX = 30 + cos(radians) * 70;
        float posY = 315 - sin(radians) * 80;
        position = fPoint(posX, posY);
        initialPosition = position;
        initialVelocity = { speedX * cosf(radians), -speedY * sinf(radians) };
        gravity = -0.00098f;
        totalTime = 0.0f;
        numBounces = 0;
        hasJumped = true;
        visible = true;
    }

    float t = totalTime + dt;
    x = initialPosition.x + initialVelocity.x * t;
    y = initialPosition.y + initialVelocity.y * t - 0.5f * gravity * t * t;

    if (y <= platformRect.y - 20 && hasJumped)
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
        else
        {
            hasJumped = false;
            visible = false;
        }
    }


    if (CheckCollision(enemyRect))
    {
        scene->hit = true;
    }

    circleX = (int)position.x + 15;
    circleY = (int)position.y + 15;

    if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) debug = !debug;

    if (visible)
    {
        if (debug)
        {
            for (float t = 0.0f; t <= totalTime; t += 1.0f)
            {
                float x1 = initialPosition.x + 15 + initialVelocity.x * t;
                float y1 = initialPosition.y + 15 + initialVelocity.y * t - 0.5f * gravity * t * t;

                app->render->DrawCircle(static_cast<int>(x1), static_cast<int>(y1), 1, 255, 0, 0);
            }

            app->render->DrawCircle(static_cast<int>(circleX), static_cast<int>(circleY), static_cast<int>(circleRadius), 255, 0, 0);

            SDL_RenderDrawRect(app->render->renderer, &enemyRect);
            SDL_RenderDrawRect(app->render->renderer, &platformRect);
		}
        
        app->render->DrawTexture(texture, position.x, position.y, NULL, 1.0f, rotation);
    }

    totalTime += dt;

    printf("\r speedX: %.2f, speedY: %.2f, x: %.2f, y: %.2f, angle: %.0f", speedX, speedY, position.x, position.y, angle);
    return true;
}

bool Player::CheckCollision(SDL_Rect enemyRect)
{

    int closestX = std::max(enemyRect.x, std::min(circleX, enemyRect.x + enemyRect.w));
    int closestY = std::max(enemyRect.y, std::min(circleY, enemyRect.y + enemyRect.h));

    int distanceSquared = (circleX - closestX) * (circleX - closestX) + (circleY - closestY) * (circleY - closestY);

    int circleRadiusSquared = 15 * 15;
    return distanceSquared <= circleRadiusSquared;
}


bool Player::CleanUp()
{

	return true;
}