#pragma once

#include <map>
#include <string>
#include <GL/glew.h>

#include "texture.h"
#include "shader.h"

class ResourceManager
{
public:
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture2D> Textures;

	static Shader LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile,
		const GLchar* gShaderFile, std::string name);
	static Shader& GetShader(std::string name);
	static Texture2D LoadTexture(const GLchar* file, GLboolean alpha, std::string name);
	static Texture2D& GetTexure(std::string name);
	static void Clear();

private:
	ResourceManager() = default;
	static Shader loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile = nullptr);
	static Texture2D loadTextureFromFile(const GLchar* file, GLboolean alpha);
};

