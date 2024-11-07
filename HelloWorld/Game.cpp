#define PLAY_IMPLEMENTATION
#include "Game.h"

void SpawnBall(Play::Point2f Position)
{
	const int ObjectId = Play::CreateGameObject(TYPE_BALL, Position, 4, "ball");
	GameObject& Ball = Play::GetGameObject(ObjectId);
	Ball.velocity = normalize({ (rand() * 2.f - RAND_MAX) / RAND_MAX,  (rand() * 2.f - RAND_MAX) / RAND_MAX }) * BALL_SPEED;
	Ball.acceleration = { 0.f, -GRAVITY };
}

void SetupScene()
{
	for (int x = 0; x < 32; x++)
	{
		for (int y = 1; y <= 32; y++)
		{
			Play::CreateGameObject(ObjectType::TYPE_BRICK, { x * (BRICK_DIMENSIONS[0] + 2) + 2 , DISPLAY_HEIGHT - (y * (BRICK_DIMENSIONS[1] + 2)) }, 7, "brick");
		}
	}
}

void StepFrame(float DeltaTime)
{
	// Balls
	const std::vector<int> BallIds = Play::CollectGameObjectIDsByType(TYPE_BALL);
	for (int i = 0; i < BallIds.size(); i++)
	{
		// Border collision
		GameObject& Ball = Play::GetGameObject(BallIds[i]);
		if (Ball.pos.x <= 0.f)
		{
			Ball.velocity.x = abs(Ball.velocity.x);
		}
		else if (Ball.pos.x >= DISPLAY_WIDTH - 2.f * BALL_RADIUS)
		{
			Ball.velocity.x = -abs(Ball.velocity.x);
		}

		if (Ball.pos.y <= 0.f)
		{
			Ball.velocity.y = abs(Ball.velocity.y);
		}
		else if (Ball.pos.y >= DISPLAY_HEIGHT - 2.f * BALL_RADIUS)
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
			if (Play::IsColliding(Ball, Brick))		// THIS FUNCTION DOES NOT WORK CORRECTLY, IT'S MEASURING A RADIUS FOR A BLOCK
			{
				Play::DestroyGameObject(BrickIds[j]);

				Point2D BallCenter = Ball.pos + normalize(Vector2f(1.f, 1.f)) * BALL_RADIUS;
				//Point2D BrickCenter = Brick.pos + Point2D(BRICK_DIMENSIONS[0] / 2, BRICK_DIMENSIONS[1] / 2);

				const float EdgePointX = Max(Brick.pos.x, Min(BallCenter.x, Brick.pos.x + BRICK_DIMENSIONS[0]));
				const float EdgePointY = Max(Brick.pos.y, Min(BallCenter.y, Brick.pos.y + BRICK_DIMENSIONS[1]));

				// Horizontal bounce
				if (abs(BallCenter.x - EdgePointX) > abs(BallCenter.y - EdgePointY) * BRICK_DIMENSIONS[0] / BRICK_DIMENSIONS[1])
				{
					Ball.velocity.x = -Ball.velocity.x;
				}
				// Vertical bounce
				else
				{
					Ball.velocity.y = -Ball.velocity.y;
				}
			}
		}
	}
}