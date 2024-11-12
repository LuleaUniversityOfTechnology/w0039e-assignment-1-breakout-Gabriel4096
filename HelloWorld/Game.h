#pragma once

#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "Constants.h"

// Spawns a ball and initializes it's velocity.
// @param Position: spawn position
void SpawnBall(Play::Point2f Position);

// Spawns the brick in the level.
void SetupScene();

// Updates all game objects for the comming frame.
// @param DeltaTime: time since last frame
void StepFrame(float DeltaTime);

// Calculates a new velocity from a bounce with a rectangle.
// @param Object: the game object that is bouncing
// @param RectanglePos: the centre of the rectangle
// @param RectangleRadii: a vector containing the aspect ratio of the rectangle
// @return The new velocity of the game object after the bounce
Play::Vector2f RectangleBounce(const Play::GameObject& Object, Play::Vector2D RectanglePos, Play::Vector2f RectangleRadii);