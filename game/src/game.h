#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "game_level.h"

enum class GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

enum class Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat PLAYER_VELOCITY(500.0f);
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const GLfloat BALL_RADIUS = 12.5f;

class Game
{
public:
	GameState State;
	GLboolean Keys[1024];
	GLuint Width, Height;
	std::vector<CGameLevel> Levels;
	GLuint Level;

public:
	Game(GLuint width, GLuint height);
	~Game();

	void Init();
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();

	void DoCollisions();
	void ResetLevel();
	void ResetPlayer();
};

