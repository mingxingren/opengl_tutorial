#include <string>
#include <iostream>
#include <glad/glad.h>
#include "shader/fragment_shader.h"
#include "shader/vertex_shader.h"

class CShaderload{
private:
    unsigned int ID;

public:
    CShaderload() {
        unsigned int vertex_shader, fragment_shader;
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        std::cout << "vertex content: " << vertex_content << std::endl;
        glShaderSource(vertex_shader, 1, &vertex_content, nullptr);
        glCompileShader(vertex_shader);
        CheckCompileErrors(vertex_shader, "VERTEX");

        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        std::cout << "fragment content: " << fragment_content << std::endl;
        glShaderSource(fragment_shader, 1, &fragment_content, nullptr);
        glCompileShader(fragment_shader);
        CheckCompileErrors(vertex_shader, "FRAGMENT");

        this->ID = glCreateProgram();
        glAttachShader(this->ID, vertex_shader);
        glAttachShader(this->ID, fragment_shader);

        glLinkProgram(this->ID);
        CheckCompileErrors(this->ID, "PROGRAM");
        glDeleteShader(fragment_shader);
        glDeleteShader(vertex_shader);
    }
    ~CShaderload() {
        glDeleteProgram(this->ID);
        ID = 0;
    }

    void Use() {
        glUseProgram(this->ID);
    }

    void SetInt(const std::string &name, int value) const {
        GLint variable_id = glGetUniformLocation(this->ID, name.c_str());
        glUniform1i(variable_id, value);
    }

    inline unsigned int GetID() {
        return this->ID;
    }

private:
    void CheckCompileErrors(GLuint shader, std::string type) {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
            else {
                std::cout << "success: " << type << std::endl;
            }
        }
        else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
            else {
                std::cout << "success: " << type << std::endl;
            }
        }
    }
};