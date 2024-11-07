#define PLAY_IMPLEMENTATION
#include "Game.h"

void SpawnBall(Play::Point2f Position)
{
	const int ObjectId = Play::CreateGameObject(TYPE_BALL, Position, 1, "ball");
	GameObject& Ball = Play::GetGameObject(ObjectId);
	Ball.velocity = normalize({ rand() / (float)RAND_MAX ,  rand() / (float)RAND_MAX }) * BALL_SPEED;
}



void StepFrame(float DeltaTime)
{
	const std::vector<int> BallIds = Play::CollectGameObjectIDsByType(TYPE_BALL);
	for (int i = 0; i < BallIds.size(); i++)
	{
		GameObject& Ball = Play::GetGameObject(BallIds[i]);
		if (Ball.pos.x <= 0.f)
		{
			Ball.velocity.x = abs(Ball.velocity.x);
		}
		else if (Ball.pos.x >= DISPLAY_WIDTH)
		{
			Ball.velocity.x = -abs(Ball.velocity.x);
		}

		if (Ball.pos.y <= 0.f)
		{
			Ball.velocity.y = abs(Ball.velocity.y);
		}
		else if (Ball.pos.y >= DISPLAY_HEIGHT)
		{
			Ball.velocity.y = -abs(Ball.velocity.y);
		}
		Play::UpdateGameObject(Ball, false, 0, true);
		Play::DrawObject(Ball);
	}
}