#include "game_level.h"

#include <fstream>
#include <sstream>
#include "resource_manager.h"

void CGameLevel::Load(const GLchar* file, GLuint levelWidth, GLuint levelHeight) {
	// Clear old data
	this->Bricks.clear();
	// Load from file
	GLuint tileCode = 0;
	CGameLevel level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<GLuint >> tileData;
	if (fstream) {
		while (std::getline(fstream, line)) {	// Read each line from level file
			std::istringstream sstream(line);
			std::vector<GLuint> row;
			while (sstream >> tileCode) {
				row.push_back(tileCode);
			}
			tileData.push_back(row);
		}

		if (tileData.size() > 0) {
			this->init(tileData, levelWidth, levelHeight);
		}
	}
}

void CGameLevel::Draw(SpriteRenderer& renderer) {
	for (CGameObject& tile : this->Bricks) {
		if (!tile.Destroyed) {
			tile.Draw(renderer);
		}
	}
}

GLboolean CGameLevel::IsCompleted() {
	for (CGameObject& tile : this->Bricks) {
		if (!tile.IsSolid && !tile.Destroyed){
			return GL_FALSE;
		}
	}
}

void CGameLevel::init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight) {
	GLuint height = tileData.size();
	GLuint width = tileData[0].size();
	GLfloat unit_width = levelWidth / static_cast<GLfloat>(width);
	GLfloat unit_height = levelHeight / height;

	for (GLuint y = 0; y < height; ++y) {
		for (GLuint x = 0; x < width; ++x) {
			if (tileData[y][x] == 1) {
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				CGameObject obj(pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
				obj.IsSolid = GL_TRUE;
				this->Bricks.push_back(obj);
			}
			else if (tileData[y][x] > 1) {
				glm::vec3 color = glm::vec3(1.0f);
				if (tileData[y][x] == 2) {
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				}
				else if (tileData[y][x] == 3) {
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				}
				else if (tileData[y][x] == 4) {
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				}
				else if (tileData[y][x] == 5) {
					color = glm::vec3(1.0f, 0.5f, 0.0f);
				}

				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				this->Bricks.push_back(CGameObject(pos, size, ResourceManager::GetTexture("block"), color));
			}
		}
	}
}