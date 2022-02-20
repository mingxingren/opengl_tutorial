#include "shader.h"
#include <iostream>

Shader& Shader::Use() {
	glUseProgram(this->ID);
	return *this;
}

void Shader::Compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource) {
	GLuint sVertex, sFragment, gShader;

	sVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(sVertex, 1, &vertexSource, NULL);
	glCompileShader(sVertex);
	checkCompileErrors(sVertex, "VERTEX");

	sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &fragmentSource, NULL);
	glCompileShader(sFragment);
	checkCompileErrors(sFragment, "FRAGMENT");

	if (geometrySource != nullptr) {
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShader, 1, &geometrySource, NULL);
		glCompileShader(gShader);
		checkCompileErrors(gShader, "GEOMETRY");
	}

	this->ID = glCreateProgram();
	glAttachShader(this->ID, sVertex);
	glAttachShader(this->ID, sFragment);
	if (geometrySource != nullptr) {
		glAttachShader(this->ID, gShader);
	}
	glLinkProgram(this->ID);
	checkCompileErrors(this->ID, "PROGRAM");

	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
	if (geometrySource != nullptr) {
		glDeleteShader(gShader);
	}
}

void Shader::SetFloat(const GLchar* name, GLfloat value, GLboolean useShader) {
	if (useShader) {
		this->Use();
	}
	glUniform1f(glGetUniformLocation(this->ID, name), value);
}

void Shader::SetInteger(const GLchar *name, GLint value, GLboolean useShader) {
	if (useShader) {
		this->Use();
	}
	glUniform1i(glGetUniformLocation(this->ID, name), value);
}

void Shader::SetVector2f(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader) {
	if (useShader) {
		this->Use();
	}
	glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}

void Shader::checkCompileErrors(GLuint object, std::string type) {
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
	else {
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
}