#pragma once

#include "Play.h"

const Play::Vector2f PADDLE_RADII = { 48.f, 6.f };
const Play::Vector2f PADDLE_START = { DISPLAY_WIDTH / 2.f, DISPLAY_HEIGHT / 16.f };
const Play::Colour PADDLE_COLOUR = { 75, 0, 0 };

struct Paddle
{
	Play::Vector2f Position;

	Paddle()
	{
		Position = PADDLE_START;
	}
};

void UpdatePaddle(Paddle& paddle, float DeltaTime);
void DrawPaddle(const Paddle& paddle);