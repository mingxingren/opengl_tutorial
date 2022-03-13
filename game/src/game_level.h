#pragma once
#include <vector>
#include "game_object.h"

class CGameLevel
{
public:
	std::vector<CGameObject> Bricks;

	CGameLevel() {}
	// ���ļ��м��عؿ�
	void Load(const GLchar* file, GLuint levelWidth, GLuint levelHeight);
	// ��Ⱦ�ؿ�
	void Draw(SpriteRenderer& renderer);
	// ���һ���ؿ��Ƿ��Ѿ����(���зǼ�Ӳ�Ĵ�ש�����ݻ�)
	GLboolean IsCompleted();

private:
	 // ��ש�����ݳ�ʼ���ؿ�
	void init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};

