#define PLAY_IMPLEMENTATION
#include "Game.h"
#include "Paddle.h"
#include <fstream>

Paddle paddle;
unsigned int* Scores;
unsigned int ScoresCount;
unsigned int ScoresCapacity;
unsigned int Score = 0;
unsigned int ScoreIncrement = 1;
unsigned int HitBricks = 0;		// Number of bricks hit during one paddle bounce.
bool bPaddleCollision = false;

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
	UpdatePaddle(paddle, DeltaTime);

	// Balls
	const std::vector<int> BallIds = Play::CollectGameObjectIDsByType(TYPE_BALL);		// Not const since the balls can be removed
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
			if (BrickIds.size() > 0)
			{
				InsertHighscore();
			}
			Score = 0;
			ScoreIncrement = 1;
			SpawnBall({ 0.5f * DISPLAY_WIDTH, 40.f });
			paddle.Position = PADDLE_START;
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
				Score += ScoreIncrement;
				if (BrickIds.size() > 1)
				{
					ScoreIncrement++;
					HitBricks++;
				}
				else
				{
					InsertHighscore();		// Record score automatically when destroying the last brick
					ScoreIncrement = 1;		// Removes combo text
					HitBricks = 0;		// Removes combo text next bounce
				}
				Play::DestroyGameObject(BrickIds[j]);
			}
		}

		// Ball/paddle collision
		if (IsColliding(paddle, Ball))
		{
			if (!bPaddleCollision && ScoreIncrement > HitBricks + 1)		// Avoids multi frame collisions && The paddle only decrements the combo if the bounce started with a combo (combo = ScoreIncrement - 1). 
			{
				switch (HitBricks)
				{
					case 0:
						ScoreIncrement = 1;		// If no brick are hit during one bounce, the combo is lost.
						break;
					case 1:
						ScoreIncrement = Max<unsigned int>(2, ScoreIncrement - 2);		// If only hitting 1 brick, the combo value the next bounce will decrement.
						break;
					default:
						ScoreIncrement = Max<unsigned int>(2, ScoreIncrement - 1);		// If hitting more than one brick, the higher combo value will be kept the next bounce.
						break;
				}
			}
			bPaddleCollision = true;
			HitBricks = 0;
			Ball.velocity = RectangleBounce(Ball, paddle.Position, PADDLE_RADII);
			Ball.velocity.x = BALL_START_SPEED * 0.5f * (Ball.pos.x - paddle.Position.x) / PADDLE_RADII.x;		// Overwrite the ball's x-speed depending on where on the paddle it hits
			Ball.velocity.x += 0.5f * (paddle.Velocity.x - Ball.velocity.x);		// Movement of the paddle will also influence the horizontal speed of the ball
		}
		else
		{
			bPaddleCollision = false;
		}
	}

	// Paddle (render)
	paddle.Colour = bPaddleCollision ? PADDLE_COLLISION_COLOUR : PADDLE_COLOUR;		// Change colour when a ball is overlapping
	DrawPaddle(paddle);

	// Highscores
	for (int i = 0; i < 5; i++)
	{
		std::string Text = "Nr " + std::to_string(i + 1) + ": " + std::to_string(i < ScoresCount ? Scores[i] : 0);
		Play::DrawDebugText({ DISPLAY_WIDTH * 0.9375f, DISPLAY_HEIGHT * (0.046875f * (5 - i)) }, Text.c_str(), Play::Colour(100, 100, 0));
	}

	// Current score
	Play::DrawDebugText({ DISPLAY_WIDTH * 0.0625f, DISPLAY_HEIGHT * 0.09375f }, std::to_string(Score).c_str(), Play::Colour(0, 100, 0));
	if (ScoreIncrement - 1 > 0)
	{
		Play::DrawDebugText({ DISPLAY_WIDTH * 0.0625f, DISPLAY_HEIGHT * 0.046875f }, std::string("Combo: " + std::to_string(ScoreIncrement - 1)).c_str(), Play::Colour(100, 100, 0));
	}
}

void LoadHighscores()
{
	fstream File("scores", ios::in);
	if (File.is_open())
	{
		string Line;
		for (ScoresCount = 0; getline(File, Line); ScoresCount++);		// Count number of highscores
		File.clear();		// Clear error states, from reaching the last line
		File.seekg(0);
		ScoresCapacity = Max<unsigned int>(5, ScoresCount * 2);
		Scores = new unsigned int[ScoresCapacity];
		for (int i = 0; getline(File, Line); i++)
		{
			Scores[i] = stoul(Line);		// Incorrect file format will crash the game here!
		}
	}
	else
	{
		ScoresCount = 0;
		ScoresCapacity = 5;
		Scores = new unsigned int[ScoresCapacity];
	}
}

void InsertHighscore()
{
	if (ScoresCount == ScoresCapacity)
	{
		// Resize array
		ScoresCapacity *= 2;
		unsigned int* NewScores = new unsigned int[ScoresCapacity];
		for (unsigned int i = 0; i < ScoresCount; i++)
		{
			NewScores[i] = Scores[i];
		}
		delete[] Scores;
		Scores = NewScores;
	}

	// Check where to insert the new score
	unsigned int InsertIndex = 0;
	for (; InsertIndex < ScoresCount && Score <= Scores[InsertIndex]; InsertIndex++);
	
	// Move down lower scores
	for (unsigned int i = ScoresCount; i > InsertIndex; i--)
	{
		Scores[i] = Scores[i - 1];
	}

	Scores[InsertIndex] = Score;
	ScoresCount++;
}

void SaveHighscores()
{
	fstream File("scores", ios::out);
	if (File.is_open())
	{
		for (int i = 0; i < ScoresCount; i++)
		{
			File << Scores[i] << "\n";
		}
	}
	File.close();
	delete[] Scores;
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