#include <iostream>
#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"

// Game-related State data
SpriteRenderer* Renderer;

Game::Game(GLuint width, GLuint height)
    : State(GameState::GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{

}

void Game::Init()
{
    ResourceManager::LoadShader("sprite.vs", "sprite.fs", nullptr, "sprite");
    // ����ͶӰ����
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    // �� sprite shader ��image���� ��������Ԫ0
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // ���� ����
    ResourceManager::LoadTexture("awesomeface.png", GL_TRUE, "face");
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
}

void Game::Update(GLfloat dt)
{

}


void Game::ProcessInput(GLfloat dt)
{

}

void Game::Render()
{
    Renderer->DrawSprite(ResourceManager::GetTexure("face"),
        glm::vec2(200, 200), glm::vec2(300, 400),
        45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}