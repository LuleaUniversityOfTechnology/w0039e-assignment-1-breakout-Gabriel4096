#include "Game.h"

// The entry point for a PlayBuffer program
void MainGameEntry(PLAY_IGNORE_COMMAND_LINE)
{
	Play::CreateManager(DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE);
	SpawnBall({ 0.5f * DISPLAY_WIDTH, 40.f});
	SetupScene();
	LoadHighscores();
	Play::CentreAllSpriteOrigins();
}

// Called by PlayBuffer every frame
bool MainGameUpdate(float DeltaTime)
{
	Play::ClearDrawingBuffer(Play::Colour(0, 0, 25));
	StepFrame(DeltaTime);
	Play::PresentDrawingBuffer();
	return Play::KeyDown( Play::KEY_ESCAPE );
}

// Gets called once when the player quits the game 
int MainGameExit()
{
	SaveHighscores();
	Play::DestroyManager();
	return PLAY_OK;
}