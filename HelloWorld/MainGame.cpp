#include "Game.h"

// The entry point for a PlayBuffer program
void MainGameEntry(PLAY_IGNORE_COMMAND_LINE)
{
	Play::CreateManager(DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE);
	SpawnBall({ DISPLAY_WIDTH / 2.f, DISPLAY_HEIGHT / 8.f });
	for (int i = 0; i < 32; i++)
	{
		SpawnBall({ rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT});
	}
	SetupScene();
}

// Called by PlayBuffer every frame (60 times a second!)
bool MainGameUpdate(float ElapsedTime)
{
	Play::ClearDrawingBuffer(Play::Colour(0, 0, 25));

	//Play::DrawDebugText( { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, "Hello World!" );

	StepFrame(ElapsedTime);

	Play::PresentDrawingBuffer();
	return Play::KeyDown( Play::KEY_ESCAPE );
}

// Gets called once when the player quits the game 
int MainGameExit()
{
	Play::DestroyManager();
	return PLAY_OK;
}
