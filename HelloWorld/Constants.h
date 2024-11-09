#pragma once

const int FRAMES_PER_SECONDS = 120;

const int DISPLAY_WIDTH = 578;
const int DISPLAY_HEIGHT = 434;
const float DISPLAY_SCALE = 3;

const int BALL_RADIUS = 4;
const float BALL_SPEED = 256.f;

const int BRICK_DIMENSIONS[] = {16.f, 10.f};

const float GRAVITY = 16384.f;

enum ObjectType
{
	TYPE_BALL,
	TYPE_BRICK
};

inline float Max(float a, float b)
{
	return a > b ? a : b;
}

inline float Min(float a, float b)
{
	return a < b ? a : b;
}