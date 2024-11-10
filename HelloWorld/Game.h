#pragma once

#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "Constants.h"



void SpawnBall(Play::Point2f Position);
void SetupScene();
void StepFrame(float DeltaTime);