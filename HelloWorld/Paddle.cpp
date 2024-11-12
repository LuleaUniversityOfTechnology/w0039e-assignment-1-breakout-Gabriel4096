#include "Paddle.h"

void UpdatePaddle(Paddle& paddle, float DeltaTime)
{
	paddle.Velocity.x = 0.f;
	if (Play::KeyDown(Play::KEY_RIGHT))
	{
		paddle.Velocity.x += PADDLE_SPEED;
	}
	if (Play::KeyDown(Play::KEY_LEFT))
	{
		paddle.Velocity.x -= PADDLE_SPEED;
	}
	
	paddle.Position += paddle.Velocity * DeltaTime;

	// Clamp to screen
	paddle.Position.x = Max(PADDLE_RADII.x, Min(paddle.Position.x, DISPLAY_WIDTH - PADDLE_RADII.x));

	// Calculate corners
	paddle.BottomLeft = paddle.Position - PADDLE_RADII;
	paddle.TopRight = paddle.Position + PADDLE_RADII;
}

void DrawPaddle(const Paddle& paddle)
{
	Play::DrawRect(paddle.BottomLeft, paddle.TopRight, paddle.Colour, true);
}

bool IsColliding(const Paddle& paddle, const Play::GameObject& object)
{
	const float dx = object.pos.x - Max(paddle.BottomLeft.x, Min(object.pos.x, paddle.TopRight.x));
	const float dy = object.pos.y - Max(paddle.BottomLeft.y, Min(object.pos.y, paddle.TopRight.y));
	return (dx * dx + dy * dy) < (object.radius * object.radius);
}