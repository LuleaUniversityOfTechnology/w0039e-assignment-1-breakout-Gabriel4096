#pragma once

const int FRAMES_PER_SECONDS = 120;
const int DISPLAY_WIDTH = 578;
const int DISPLAY_HEIGHT = 434;
const float DISPLAY_SCALE = 3;

const char BALL_SPRITE[] = "ball";
const float BALL_SPEED = 384.f;

const char BRICK_SPRITE[] = "brick";
const float BRICK_SPACE = 2.f;

const float GRAVITY_FORCE = 256.f;

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

inline char Sign(float x)
{
	return x > 0.f ? 1 : (x < 0.f ? -1 : 0);
}