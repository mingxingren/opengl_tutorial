#include "sprite_renderer.h"

SpriteRenderer::SpriteRenderer(Shader& shader) {
	this->shader = shader;
	this->initRenderData();
}

SpriteRenderer::~SpriteRenderer() {
	glDeleteVertexArrays(1, &this->quadVAO);
	glDeleteBuffers(1, &this->quadVBO);
}

void SpriteRenderer::DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size, 
	GLfloat rotate, glm::vec3 color) {
	this->shader.Use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
	model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back

	model = glm::scale(model, glm::vec3(size, 1.0f));

	this->shader.SetMatrix4("model", model);

	this->shader.SetVector3f("spriteColor", color);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();
	
	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void SpriteRenderer::initRenderData() {
	GLfloat vertices[] = {
		// Pos	    // Tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
	};

	{
		// ���������������
		glGenVertexArrays(1, &this->quadVAO);
		// �������㻺��
		glGenBuffers(1, &this->quadVBO);

		// ���VBO (���㻺�����)
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// �� VBO�󶨵� VAO��
		glBindVertexArray(this->quadVAO);
		glEnableVertexAttribArray(0);
		// index ��ʾ�� buffer �������� location Ϊ 0
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

		// ��󶥵㻺��
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// ���ǰ�󶨵Ķ����������
		glBindVertexArray(0);
	}
}