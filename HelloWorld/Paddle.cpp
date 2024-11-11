#include "Paddle.h"

void UpdatePaddle(Paddle& paddle, float DeltaTime)
{
	if (Play::KeyDown(Play::KEY_RIGHT))
	{
		paddle.Position.x += BALL_SPEED * DeltaTime;
	}
	if (Play::KeyDown(Play::KEY_LEFT))
	{
		paddle.Position.x -= BALL_SPEED * DeltaTime;
	}
}

void DrawPaddle(const Paddle& paddle)
{
	Play::Point2D BottomLeft = {
		paddle.Position.x - PADDLE_RADII.x,
		paddle.Position.y - PADDLE_RADII.y };
	Play::Point2D TopRight = {
		paddle.Position.x + PADDLE_RADII.x,
		paddle.Position.y + PADDLE_RADII.y };
	Play::DrawRect(BottomLeft, TopRight, PADDLE_COLOUR, true);
}
