#define PLAY_IMPLEMENTATION
#include "Game.h"

Paddle paddle;

void SpawnBall(Play::Point2f Position)
{
	const int ObjectId = Play::CreateGameObject(TYPE_BALL, Position, 0.5f * Play::GetSpriteWidth(BALL_SPRITE), BALL_SPRITE);
	GameObject& Ball = Play::GetGameObject(ObjectId);
	Ball.velocity = normalize(Vector2D((rand() * 2.f - RAND_MAX) / RAND_MAX,  (rand() * 2.f - RAND_MAX) / RAND_MAX)) * BALL_SPEED;
	//Ball.acceleration = { 0.f, -GRAVITY_FORCE };
}

void SetupScene()
{
	const Play::Vector2f BRICK_DIMENSIONS = Play::Graphics::GetSpriteSize(Play::GetSpriteId(BRICK_SPRITE));
	for (float x = 0.f; x < 32.f; x++)
	{
		const float NewPosX = x * (BRICK_DIMENSIONS.x + BRICK_SPACE) + 0.5f * BRICK_DIMENSIONS.x + BRICK_SPACE;
		for (float y = 0.f; y < 32.f; y++)
		{
			const float NewPosY = DISPLAY_HEIGHT - (y * (BRICK_DIMENSIONS.y + BRICK_SPACE) + 0.5f * BRICK_DIMENSIONS.y + BRICK_SPACE);
			Play::CreateGameObject(ObjectType::TYPE_BRICK, Play::Point2D(NewPosX, NewPosY), 0.5f * BRICK_DIMENSIONS.x, BRICK_SPRITE);
		}
	}
}

void StepFrame(float DeltaTime)
{
	const float BALL_RADIUS = 0.5f * Play::GetSpriteWidth(BALL_SPRITE);
	const Play::Vector2f BRICK_RADII = 0.5f * Play::Graphics::GetSpriteSize(Play::GetSpriteId(BRICK_SPRITE));

	// Balls
	const std::vector<int> BallIds = Play::CollectGameObjectIDsByType(TYPE_BALL);
	for (int i = 0; i < BallIds.size(); i++)
	{
		// Border collision
		GameObject& Ball = Play::GetGameObject(BallIds[i]);
		if (Ball.pos.x < BALL_RADIUS)
		{
			Ball.velocity.x = abs(Ball.velocity.x);
		}
		else if (Ball.pos.x > DISPLAY_WIDTH - BALL_RADIUS)
		{
			Ball.velocity.x = -abs(Ball.velocity.x);
		}

		if (Ball.pos.y < BALL_RADIUS)
		{
			Ball.velocity.y = abs(Ball.velocity.y);
		}
		else if (Ball.pos.y >= DISPLAY_HEIGHT - BALL_RADIUS)
		{
			Ball.velocity.y = -abs(Ball.velocity.y);
		}

		Play::UpdateGameObject(Ball, DeltaTime, false, 0);
		Play::DrawObject(Ball);
	}

	// Bricks
	const std::vector<int> BrickIds = Play::CollectGameObjectIDsByType(TYPE_BRICK);
	for (int i = 0; i < BrickIds.size(); i++)
	{
		GameObject& Brick = Play::GetGameObject(BrickIds[i]);
		Play::UpdateGameObject(Brick, DeltaTime, false, 0);
		Play::DrawObject(Brick);
	}

	// Ball/brick collision
	for (int i = 0; i < BallIds.size(); i++)
	{
		GameObject& Ball = Play::GetGameObject(BallIds[i]);
		for (int j = 0; j < BrickIds.size(); j++)
		{
			GameObject& Brick = Play::GetGameObject(BrickIds[j]);
			if (Play::IsColliding(Ball, Brick))
			{
				// Horizontal bounce
				if (abs(Ball.oldPos.x - Brick.pos.x) > abs(Ball.oldPos.y - Brick.pos.y) * BRICK_RADII.x / BRICK_RADII.y)
				{
					// Right collision
					if (Ball.oldPos.x > Brick.pos.x)
					{
						Ball.velocity.x = abs(Ball.velocity.x);
					}
					// Left collision
					else if (Ball.oldPos.x < Brick.pos.x)
					{
						Ball.velocity.x = -abs(Ball.velocity.x);
					}
				}
				// Vertical bounce
				else
				{
					// Up collision
					if (Ball.oldPos.y > Brick.pos.y)
					{
						Ball.velocity.y = abs(Ball.velocity.y);
					}
					// Down collision
					else if (Ball.oldPos.y <= Brick.pos.y)
					{
						Ball.velocity.y = -abs(Ball.velocity.y);
					}
				}

				Play::DestroyGameObject(BrickIds[j]);
			}
		}
	}

	// Paddle
	UpdatePaddle(paddle, DeltaTime);
	DrawPaddle(paddle);
}