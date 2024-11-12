#pragma once

#include "Game.h"

// Consts for the paddle

const float PADDLE_SPEED = 256.f;

const Play::Vector2f PADDLE_RADII = { 48.f, 6.f };
const Play::Vector2f PADDLE_START = { DISPLAY_WIDTH / 2.f, DISPLAY_HEIGHT / 16.f };
const Play::Colour PADDLE_COLOUR = { 75, 0, 0 };
const Play::Colour PADDLE_COLLISION_COLOUR = { 25, 100, 25 };

struct Paddle
{
	Play::Point2D Position;
	Play::Vector2f Velocity;
	Play::Point2D BottomLeft;
	Play::Point2D TopRight;
	Play::Colour Colour;

	Paddle() :
		Position(PADDLE_START),
		Velocity(0.f, 0.f),
		BottomLeft(Position - PADDLE_RADII),
		TopRight(Position + PADDLE_RADII),
		Colour(PADDLE_COLOUR)
	{
	}
};

// Updates the movement of a paddle from keyboard input.
// @param paddle: the paddle object
// @param DeltaTime: time since last frame
void UpdatePaddle(Paddle& paddle, float DeltaTime);

// Renders a paddle.
// @param paddle: the paddle object
void DrawPaddle(const Paddle& paddle);

// Checks for overlap between a paddle and a game object.
// @param paddle: the paddle object
// @param object: the game object
bool IsColliding(const Paddle& paddle, const Play::GameObject& object);