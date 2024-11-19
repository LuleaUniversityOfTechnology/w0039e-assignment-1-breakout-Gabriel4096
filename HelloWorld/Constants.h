#pragma once

const int FRAMES_PER_SECONDS = 120;
const int DISPLAY_WIDTH = 578;
const int DISPLAY_HEIGHT = 434;
const float DISPLAY_SCALE = 4;

const char BALL_SPRITE[] = "ball";
const float BALL_START_SPEED = 512.f;

const char BRICK_SPRITE[] = "brick";
const float BRICK_SPACE = 2.f;

enum ObjectType : char
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