#include <iostream>
#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "ball_object_collisions.h"

// 初始化球的速度
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// 球的半径
const GLfloat BALL_RADIUS = 12.5f;

CBallObject* Ball = nullptr;

// Game-related State data
SpriteRenderer* Renderer;
CGameObject* Player;

Game::Game(GLuint width, GLuint height)
    : State(GameState::GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
    delete Renderer;
    delete Player;
}

void Game::Init()
{
    ResourceManager::LoadShader("sprite.vs", "sprite.fs", nullptr, "sprite");
    // 设置投影矩阵
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    // 给 sprite shader 的image变量 分配纹理单元0
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // 加载 纹理
    ResourceManager::LoadTexture("background.jpg", GL_FALSE, "background");
    ResourceManager::LoadTexture("awesomeface.png", GL_TRUE, "face");
    ResourceManager::LoadTexture("block.png", GL_FALSE, "block");
    ResourceManager::LoadTexture("block_solid.png", GL_FALSE, "block_solid");
    ResourceManager::LoadTexture("paddle.png", true, "paddle");

    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

    CGameLevel one; one.Load("levels/one.lvl", this->Width, this->Height * 0.5);
    CGameLevel two; two.Load("levels/two.lvl", this->Width, this->Height * 0.5);
    CGameLevel three; three.Load("levels/three.lvl", this->Width, this->Height * 0.5);
    CGameLevel four; four.Load("levels/four.lvl", this->Width, this->Height * 0.5);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;
   
    glm::vec2 playerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
    Player = new CGameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
    Ball = new CBallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));
}

void Game::Update(GLfloat dt)
{
    Ball->Move(dt, this->Width);
}


void Game::ProcessInput(GLfloat dt)
{
    if (this->State == GameState::GAME_ACTIVE) {
        GLfloat velocity = PLAYER_VELOCITY * dt;

        if (this->Keys[GLFW_KEY_A]) {
            if (Player->Position.x > 0) {
                Player->Position.x -= velocity;
                if (Ball->Stuck) {
                    Ball->Position.x -= velocity;
                }
            }
        }

        if (this->Keys[GLFW_KEY_D]) {
            if (Player->Position.x < this->Width - Player->Size.x) {
                Player->Position.x += velocity;
                if (Ball->Stuck) {
                    Ball->Position += velocity;
                }
            }
        }

        if (this->Keys[GLFW_KEY_SPACE]) {
            Ball->Stuck = false;
        }
    }
}

void Game::Render()
{
    this->Levels[this->Level].Draw(*Renderer);
    Player->Draw(*Renderer);
    if (this->State == GameState::GAME_ACTIVE) {
        Ball->Draw(*Renderer);
    }
}