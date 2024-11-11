#define PLAY_IMPLEMENTATION
#include "Game.h"
#include "Paddle.h"

Paddle paddle;

void SpawnBall(Play::Point2f Position)
{
	const int ObjectId = Play::CreateGameObject(TYPE_BALL, Position, 0.5f * Play::GetSpriteWidth(BALL_SPRITE), BALL_SPRITE);
	GameObject& Ball = Play::GetGameObject(ObjectId);
	Ball.velocity = normalize({ 1, -1 }) * BALL_SPEED;
	//Ball.velocity = normalize(Vector2D((rand() * 2.f - RAND_MAX) / RAND_MAX,  (rand() * 2.f - RAND_MAX) / RAND_MAX)) * BALL_SPEED;
	//Ball.acceleration = { 0.f, -GRAVITY_FORCE };
}

void SetupScene()
{
	const Play::Vector2f BRICK_DIMENSIONS = Play::Graphics::GetSpriteSize(Play::GetSpriteId(BRICK_SPRITE));
	for (float x = 0.f; x < 32.f; x++)
	{
		const float NewPosX = x * (BRICK_DIMENSIONS.x + BRICK_SPACE) + 0.5f * BRICK_DIMENSIONS.x + BRICK_SPACE;
		for (float y = 0.f; y < 8.f; y++)
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
	std::vector<int> BallIds = Play::CollectGameObjectIDsByType(TYPE_BALL);
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

		if (Ball.pos.y < -BALL_RADIUS)
		{
			Ball.velocity.y = abs(Ball.velocity.y);
			Play::DestroyGameObject(BallIds[i]);
			break;
		}
		else if (Ball.pos.y > DISPLAY_HEIGHT - BALL_RADIUS)
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
	BallIds = Play::CollectGameObjectIDsByType(TYPE_BALL);

	for (int i = 0; i < BallIds.size(); i++)
	{
		GameObject& Ball = Play::GetGameObject(BallIds[i]);
		for (int j = 0; j < BrickIds.size(); j++)
		{
			GameObject& Brick = Play::GetGameObject(BrickIds[j]);
			if (Play::IsColliding(Ball, Brick))
			{
				Ball.velocity = RectangleBounce(Ball, Brick.pos, BRICK_RADII);
				Play::DestroyGameObject(BrickIds[j]);
			}
		}
	}

	// Paddle
	UpdatePaddle(paddle, DeltaTime);

	bool bPaddleCollision = false;
	for (int i = 0; i < BallIds.size(); i++)
	{
		GameObject& Ball = Play::GetGameObject(BallIds[i]);
		if (IsColliding(paddle, Ball))
		{
			bPaddleCollision = true;
			Ball.velocity = RectangleBounce(Ball, paddle.Position, PADDLE_RADII);
		}
	}
	paddle.Colour = bPaddleCollision ? PADDLE_COLLISION_COLOUR : PADDLE_COLOUR;
	if (BallIds.size() == 0)
	{
		paddle.Colour = { 75, 0, 75 };
	}

	DrawPaddle(paddle);
}

// Calculates a new velocity from a bounce with a rectangle.
// @param Object: The game object that is bouncing
// @param RectanglePos: The centre of the rectangle
// @param RectangleRadii: A vector containing the aspect ratio of the rectangle
// @return The new velocity of the game object after the bounce
Play::Vector2f RectangleBounce(const Play::GameObject& Object, Play::Vector2D RectanglePos, Play::Vector2f RectangleRadii)
{
	Play::Vector2f NewVelocity = Object.velocity;

	// Horizontal bounce
	if (abs(Object.pos.x - RectanglePos.x) > abs(Object.pos.y - RectanglePos.y) * RectangleRadii.x / RectangleRadii.y)
	{
		// Right collision
		if (Object.pos.x > RectanglePos.x)
		{
			NewVelocity.x = abs(NewVelocity.x);
		}
		// Left collision
		else if (Object.pos.x < RectanglePos.x)
		{
			NewVelocity.x = -abs(NewVelocity.x);
		}
	}
	// Vertical bounce
	else
	{
		// Up collision
		if (Object.pos.y > RectanglePos.y)
		{
			NewVelocity.y = abs(NewVelocity.y);
		}
		// Down collision
		else if (Object.pos.y < RectanglePos.y)
		{
			NewVelocity.y = -abs(NewVelocity.y);
		}
	}

	return NewVelocity;
}
