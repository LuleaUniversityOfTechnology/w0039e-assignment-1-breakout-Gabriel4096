#define PLAY_IMPLEMENTATION
#include "Game.h"

void SpawnBall(Play::Point2f Position)
{
	const int ObjectId = Play::CreateGameObject(TYPE_BALL, Position, 256, "Ball");
	GameObject& Ball = Play::GetGameObject(ObjectId);
	Ball.velocity = normalize({ rand() / (float)RAND_MAX ,  rand() / (float)RAND_MAX }) * BALL_SPEED;
}



void StepFrame(float DeltaTime)
{
	const std::vector<int> BallIds = Play::CollectGameObjectIDsByType(TYPE_BALL);
	for (int i = 0; i < BallIds.size(); i++)
	{
		GameObject& Ball = Play::GetGameObject(BallIds[i]);
		if (Ball.pos.x <= 0.f || Ball.pos.x >= DISPLAY_WIDTH)
		{
			Ball.velocity.x = -Ball.velocity.x;
		}
		if (Ball.pos.y <= 0.f || Ball.pos.y >= DISPLAY_HEIGHT)
		{
			Ball.velocity.y = -Ball.velocity.y;
		}
		Play::UpdateGameObject(Ball, false, 0, true);
		Play::DrawObject(Ball);
	}
}