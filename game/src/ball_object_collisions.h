#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "texture.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "game_object.h"

class CBallObject : public CGameObject
{
public:
	GLfloat Radius;
	GLboolean Stuck;

	CBallObject();
	CBallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);

	glm::vec2 Move(GLfloat dt, GLuint window_width);
	void Reset(glm::vec2 position, glm::vec2 velocity);
};

