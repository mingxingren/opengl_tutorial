#pragma once
#include <vector>
#include "game_object.h"

class CGameLevel
{
public:
	std::vector<CGameObject> Bricks;

	CGameLevel() {}
	// 从文件中加载关卡
	void Load(const GLchar* file, GLuint levelWidth, GLuint levelHeight);
	// 渲染关卡
	void Draw(SpriteRenderer& renderer);
	// 检查一个关卡是否已经完成(所有非坚硬的瓷砖均被摧毁)
	GLboolean IsCompleted();

private:
	 // 由砖块数据初始化关卡
	void init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};

