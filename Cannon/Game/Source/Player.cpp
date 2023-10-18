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

    if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN && angle < 70)
    {
        angle += 5;
        scene->angle -= 5;
    }
    if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN && angle > 20)
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

    vy = initialVelocity.y - gravity * totalTime;

    if (CheckCollision(platformRect) && !hasBounced) {
        colFace = GetCollisionSide(platformRect);
        col = true;
        printf("Collision with platform\n");
    }
    else if (CheckCollision(platformRect2) && !hasBounced) {
        colFace = GetCollisionSide(platformRect2);
        col = true;
        printf("Collision with platform 2\n");
    }
    else
    {
        col = false;
        hasBounced = false;
    }

    if (!col && hasJumped && numBounces < 12)
    {
        position = { x, y };
        rotation += 1.0f * dt;
    }
    else if (col)
    {
        rotation = 0.0f;
        float radians = angle * M_PI / 180.0f;
        float posX = position.x + cos(radians);
        float posY = position.y - sin(radians);
        position = fPoint(posX, posY);
        initialPosition = position;

        if (colFace == 1) // left
        {
            if (vy > 0) initialVelocity.y = -initialVelocity.y * 0.75f;
            else initialVelocity.y = initialVelocity.y * 0.75f;
            initialVelocity.x = -initialVelocity.x * 0.90f;
        }
        else if (colFace == 2) //right
        {
            if (vy > 0) initialVelocity.y = -initialVelocity.y * 0.75f;
            else initialVelocity.y = initialVelocity.y * 0.75f;
            initialVelocity.x = -initialVelocity.x * 0.90f;
        }
        else if (colFace == 3) //top
		{
			initialVelocity.y = initialVelocity.y * 0.90f;
			initialVelocity.x = initialVelocity.x * 0.75f;
		}
		else if (colFace == 4) //bottom
		{
			initialVelocity.y = -initialVelocity.y * 0.90f;
			initialVelocity.x = initialVelocity.x * 0.75f;
		}

        gravity = -0.00098f;
        totalTime = 0.0f;
        numBounces++;
        hasBounced = true;
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
            SDL_RenderDrawRect(app->render->renderer, &platformRect2);
        }

        app->render->DrawTexture(texture, position.x, position.y, NULL, 1.0f, rotation);
    }

    totalTime += dt;

   /* printf("\r speedX: %.2f, speedY: %.2f, x: %.2f, y: %.2f, angle: %.0f", speedX, speedY, position.x, position.y, angle);*/
    return true;
}

bool Player::CheckCollision(SDL_Rect rect)
{
    float closestX = std::max(rect.x, std::min(circleX, rect.x + rect.w));
    float closestY = std::max(rect.y, std::min(circleY, rect.y + rect.h));

    float distanceX = circleX - closestX;
    float distanceY = circleY - closestY;

    float circleRadiusSquared = pow(circleRadius, 2);

    if ((distanceX * distanceX + distanceY * distanceY) <= circleRadiusSquared)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int Player::GetCollisionSide(SDL_Rect rect)
{
    float closestX = std::max(rect.x, std::min(circleX, rect.x + rect.w));
    float closestY = std::max(rect.y, std::min(circleY, rect.y + rect.h));

    float distanceX = circleX - closestX;
    float distanceY = circleY - closestY;

    float circleRadiusSquared = pow(circleRadius, 2);


    if ((distanceX * distanceX + distanceY * distanceY) <= circleRadiusSquared)
    {

        float overlapX = circleRadius - abs(distanceX);
        float overlapY = circleRadius - abs(distanceY);

        if (overlapX < overlapY)
        {
            if (distanceX < 0)
            {
                printf("Collision on the left\n");
                colFace = 1;
            }
            else
            {
                printf("Collision on the right\n");
                colFace = 2;
            }
        }
        else
        {
            if (distanceY < 0)
            {
                printf("Collision on the top\n");
                colFace = 3;
            }
            else
            {
                printf("Collision on the bottom\n");
                colFace = 4;
            }
        }
    }
    return colFace;
}

bool Player::CleanUp()
{

    return true;
}