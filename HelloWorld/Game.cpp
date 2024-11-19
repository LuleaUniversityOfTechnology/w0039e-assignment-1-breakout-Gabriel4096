#define PLAY_IMPLEMENTATION
#include "Game.h"
#include "Paddle.h"
#include <fstream>

Paddle paddle;
int HighscoresCount;
unsigned int* HighScores = new unsigned int[HighscoresCount] {};
unsigned int Score = 0;

void SpawnBall(Play::Point2f Position)
{
	const int ObjectId = Play::CreateGameObject(TYPE_BALL, Position, 0.5f * Play::GetSpriteWidth(BALL_SPRITE), BALL_SPRITE);
	GameObject& Ball = Play::GetGameObject(ObjectId);
	Ball.velocity = BALL_START_SPEED * normalize({ 1, 1 });
}

void SetupScene()
{
	const Play::Vector2f BRICK_DIMENSIONS = Play::Graphics::GetSpriteSize(Play::GetSpriteId(BRICK_SPRITE));
	for (float x = 0.f; x < 32.f; x++)		// float++
	{
		const float NewPosX = x * (BRICK_DIMENSIONS.x + BRICK_SPACE) + 0.5f * BRICK_DIMENSIONS.x + BRICK_SPACE;
		for (float y = 0.f; y < 8.f; y++)		// float++
		{
			const float NewPosY = DISPLAY_HEIGHT - (y * (BRICK_DIMENSIONS.y + BRICK_SPACE) + 0.5f * BRICK_DIMENSIONS.y + BRICK_SPACE);
			Play::CreateGameObject(ObjectType::TYPE_BRICK, Play::Point2D(NewPosX, NewPosY), 0.5f * BRICK_DIMENSIONS.x, BRICK_SPRITE);		// Set the collision radius to half the width of the brick
		}
	}
}

void StepFrame(float DeltaTime)
{
	const float BALL_RADIUS = 0.5f * Play::GetSpriteWidth(BALL_SPRITE);
	const Play::Vector2f BRICK_RADII = 0.5f * Play::Graphics::GetSpriteSize(Play::GetSpriteId(BRICK_SPRITE));

	// Bricks
	const std::vector<int> BrickIds = Play::CollectGameObjectIDsByType(TYPE_BRICK);
	for (int i = 0; i < BrickIds.size(); i++)
	{
		GameObject& Brick = Play::GetGameObject(BrickIds[i]);
		Play::UpdateGameObject(Brick, DeltaTime);
		Play::DrawObject(Brick);
	}

	// Paddle (update)
	bool bPaddleCollision = false;
	UpdatePaddle(paddle, DeltaTime);

	// Balls
	std::vector<int> BallIds = Play::CollectGameObjectIDsByType(TYPE_BALL);		// Not const since the balls can be removed
	for (int i = 0; i < BallIds.size(); i++)
	{
		GameObject& Ball = Play::GetGameObject(BallIds[i]);

		// Border collision
		if (Ball.pos.x < BALL_RADIUS && Ball.velocity.x < 0.f || Ball.pos.x > DISPLAY_WIDTH - BALL_RADIUS && Ball.velocity.x > 0.f)
		{
			Ball.velocity.x = -Ball.velocity.x;
		}
		if (Ball.pos.y < -BALL_RADIUS)
		{
			// Restart
			Play::DestroyAllGameObjects();		// Destory all objects when the ball is down off-screen
			InsertHighscore(Score);
			Score = 0;
			SpawnBall({ paddle.Position.x, 40.f });
			SetupScene();
			break;
		}
		else if (Ball.pos.y > DISPLAY_HEIGHT - BALL_RADIUS && Ball.velocity.y > 0.f)
		{
			Ball.velocity.y = -Ball.velocity.y;
		}
		Play::UpdateGameObject(Ball, DeltaTime);
		Play::DrawObject(Ball);

		// Ball/brick collision
		for (int j = 0; j < BrickIds.size(); j++)
		{
			GameObject& Brick = Play::GetGameObject(BrickIds[j]);
			if (Play::IsColliding(Ball, Brick))
			{
				Ball.velocity = RectangleBounce(Ball, Brick.pos, BRICK_RADII);
				Play::DestroyGameObject(BrickIds[j]);
				Score++;
			}
		}

		// Ball/paddle collision
		if (IsColliding(paddle, Ball))
		{
			bPaddleCollision = true;
			Ball.velocity = RectangleBounce(Ball, paddle.Position, PADDLE_RADII);
			Ball.velocity.x = BALL_START_SPEED * 0.5f * (Ball.pos.x - paddle.Position.x) / PADDLE_RADII.x;		// Overwrite the ball's x-speed depending on where on the paddle it hits
			Ball.velocity.x += 0.5f * (paddle.Velocity.x - Ball.velocity.x);		// Movement of the paddle will also influence the horizontal speed of the ball
		}
	}

	// Paddle (render)
	BallIds = Play::CollectGameObjectIDsByType(TYPE_BALL);		// Recount the balls
	if (BallIds.size() > 0)
	{
		paddle.Colour = bPaddleCollision ? PADDLE_COLLISION_COLOUR : PADDLE_COLOUR;		// Change colour when a ball is overlapping
	}
	else
	{
		paddle.Colour = { 75, 0, 75 };
	}
	DrawPaddle(paddle);

	// Highscores
	for (int i = 0; i < HighscoresCount; i++)
	{
		std::string Text = "N" + std::to_string(i + 1) + ": " + std::to_string(HighScores[i]);
		Play::DrawDebugText({ DISPLAY_WIDTH * 0.9375f, DISPLAY_HEIGHT * (0.05f * (HighscoresCount - i)) }, Text.c_str(), Play::Colour(100, 100, 0));
	}

	// Current Highscore
	Play::DrawDebugText({ DISPLAY_WIDTH * 0.0625f, DISPLAY_HEIGHT * 0.09375f }, std::to_string(Score).c_str(), Play::Colour(100, 100, 0));
}

void InsertHighscore(unsigned int score)
{
	int InsertIndex = 0;
	for (; InsertIndex < HighscoresCount; InsertIndex++)		// Check where to insert the new score
	{
		if (score >= HighScores[InsertIndex])
		{
			break;
		}
	}
	if (InsertIndex == HighscoresCount)		// Score didn't make it to the list
	{
		return;
	}
	for (int i = HighscoresCount - 2; i >= InsertIndex; i--)		// Move down lower scores
	{
		HighScores[i + 1] = HighScores[i];
	}
	HighScores[InsertIndex] = score;
}

void LoadHighscores()
{
	string Line;
	fstream File("scores", ios::in);
	if (File.is_open())
	{
		for (HighscoresCount = 0; getline(File, Line); HighscoresCount++);		// Count number of highscores
		File.clear();		// Clear error states from reaching the last line
		File.seekg(0);
		HighScores = new unsigned int[HighscoresCount] {};
		for (int i = 0; getline(File, Line); i++)
		{
			HighScores[i] = stoul(Line);		// Crashes if empty line!
		}
	}
	else
	{
		HighscoresCount = 5;
		HighScores = new unsigned int[HighscoresCount] {};
	}
}

void SaveHighscores()
{
	fstream File("scores", ios::out);
	if (File.is_open())
	{
		for (int i = 0; i < HighscoresCount; i++)
		{
			File << HighScores[i] << "\n";
		}
	}
	File.close();
	delete[] HighScores;
}

Play::Vector2f RectangleBounce(const Play::GameObject& Object, Play::Vector2D RectanglePos, Play::Vector2f RectangleRadii)
{
	Play::Vector2f NewVelocity = Object.velocity;

	// Horizontal bounce
	if (abs(Object.oldPos.x - RectanglePos.x) > abs(Object.oldPos.y - RectanglePos.y) * RectangleRadii.x / RectangleRadii.y)		// oldPos gives better results than pos
	{
		// Right collision
		if (Object.oldPos.x > RectanglePos.x)
		{
			NewVelocity.x = abs(NewVelocity.x);
		}
		// Left collision
		else if (Object.oldPos.x < RectanglePos.x)
		{
			NewVelocity.x = -abs(NewVelocity.x);
		}
	}
	// Vertical bounce
	else
	{
		// Up collision
		if (Object.oldPos.y > RectanglePos.y)
		{
			NewVelocity.y = abs(NewVelocity.y);
		}
		// Down collision
		else if (Object.oldPos.y < RectanglePos.y)
		{
			NewVelocity.y = -abs(NewVelocity.y);
		}
	}

	return NewVelocity;
}