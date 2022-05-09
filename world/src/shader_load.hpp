#ifndef SHADER_LOAD_H
#define SHADER_LOAD_H

#include <filesystem>
#include <memory>
#include <fstream>
#include <sstream>
#include "../deps/glm/glm.hpp"
#include "../deps/glm/gtc/type_ptr.hpp"
#include "../deps/glad/glad.h"
#include "common/macrodef.h"

class CShader {
public:
    static std::shared_ptr<CShader> 
    load_shader_width_path(const char* _vertex_file_path, 
                        const char * _pixel_file_path, 
                        const char * _geometry_file_path) {
        STD_NAMESPACE_DECLARE

        auto read_file_func = [](const filesystem::path &_file_path) {
            if (!filesystem::exists(_file_path)) {
                stringstream msg;
                msg << _file_path << " cant find!" << std::endl;
                LOG(msg.str())
                return string();
            }

            ifstream shader_file;
            try {
                shader_file.open(_file_path, ios::binary | ios::in);
                stringstream content;
                content << shader_file.rdbuf();
                return content.str();
            }
            catch(std::ifstream::failure& error) {
                stringstream msg;
                msg << _file_path << " open fail! error: " << error.what() << std::endl;
                LOG(msg.str())
                return string();
            }   
        };

        string vertex_shader = read_file_func(_vertex_file_path);
        if (vertex_shader.empty()) {
            return nullptr;
        }

        string pixel_shader = read_file_func(_pixel_file_path);
        if (pixel_shader.empty()) {
            return nullptr;
        }

        string geometry_shader = "";
        if (_geometry_file_path != nullptr) {
            geometry_shader = read_file_func(_geometry_file_path);
        }

        return CShader::load_shader(vertex_shader.c_str(), 
                                    pixel_shader.c_str(), 
                                    geometry_shader.empty() ? nullptr : geometry_shader.c_str());

    }

    static std::shared_ptr<CShader> 
    load_shader(const char* _vertex_shader, 
                const char* _pixel_shader, 
                const char* _geometry_shader) {
        STD_NAMESPACE_DECLARE
        enum class SHADER_FILE_TYPE{
            VERTEX_SHADER = 0,
            PIXEL_SHADER,
            GEOMETRY_SHADER,
            PROGRAM,
        };

        auto check_compile_error_func = [](SHADER_FILE_TYPE _shader_type, GLuint _shader_id){
            GLint success;
            GLchar infoLog[1024];
            constexpr const char* shader_file_type_str[3] = { "vertex shader", "pixel shader", "gemotry shader" };
            if (_shader_type != SHADER_FILE_TYPE::PROGRAM) {
                glGetShaderiv(_shader_id, GL_COMPILE_STATUS, &success);
                if (!success) {
                    glGetShaderInfoLog(_shader_id, 1024, NULL, infoLog);
                    std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << shader_file_type_str[static_cast<int>(_shader_type)] << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
                else {
                    std::cout << "success: " << shader_file_type_str[static_cast<int>(_shader_type)] << std::endl;
                }
            }
            else {
                glGetProgramiv(_shader_id, GL_LINK_STATUS, &success);
                if (!success) {
                    glGetProgramInfoLog(_shader_id, 1024, NULL, infoLog);
                    std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << shader_file_type_str[static_cast<int>(_shader_type)] << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
                else {
                    std::cout << "success: " << shader_file_type_str[static_cast<int>(_shader_type)] << std::endl;
                }
            }
        };
        
        GLuint program_id = glCreateProgram();

        GLuint vertex_shader_id, pixel_shader_id;
        vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader_id, 1, &_vertex_shader, NULL);
        glCompileShader(vertex_shader_id);
        check_compile_error_func(SHADER_FILE_TYPE::VERTEX_SHADER, vertex_shader_id);
        glAttachShader(program_id, vertex_shader_id);

        pixel_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(pixel_shader_id, 1, &_pixel_shader, NULL);
        glCompileShader(pixel_shader_id);
        check_compile_error_func(SHADER_FILE_TYPE::PIXEL_SHADER, pixel_shader_id);
        glAttachShader(program_id, pixel_shader_id);

        GLuint geometry_shader_id = 0;
        if (_geometry_shader != nullptr) {
            glShaderSource(pixel_shader_id, 1, &_pixel_shader, NULL);
            glCompileShader(pixel_shader_id);
            check_compile_error_func(SHADER_FILE_TYPE::PIXEL_SHADER, pixel_shader_id);
            glAttachShader(program_id, geometry_shader_id);
        }

        glLinkProgram(program_id);
        check_compile_error_func(SHADER_FILE_TYPE::PROGRAM, program_id);
        glDeleteShader(vertex_shader_id);
        glDeleteShader(pixel_shader_id);
        if (geometry_shader_id != 0) {
            glDeleteShader(geometry_shader_id);
        }

        std::shared_ptr<CShader> shader_ptr(new CShader(program_id));
        return shader_ptr;
    }
    

public:
    ~CShader() {
        glDeleteProgram(this->m_id);
    }

    void use() {
        glUseProgram(this->m_id);
    }

    void set_int(const std::string& _name, int _value) const {
        glUniform1i(glGetUniformLocation(this->m_id, _name.c_str()), _value);
    }

    void set_flaot(const std::string& _name, int _value) const {
        glUniform1f(glGetUniformLocation(this->m_id, _name.c_str()), _value);
    }

    void set_vec_2(const std::string& _name, glm::vec2 _value) const {
        glUniform2fv(glGetUniformLocation(this->m_id, _name.c_str()), 1, glm::value_ptr(_value));
    }

    void set_matrix(const std::string& _name, const glm::mat4& _mat) const {
        glUniformMatrix4fv(glGetUniformLocation(this->m_id, _name.c_str()), 1, GL_FALSE, glm::value_ptr(_mat));
    }

private:
    CShader(unsigned int _id) : m_id(_id) {
        
    }
    CShader(const CShader &) = delete;

private:
    unsigned int m_id;
};

#endif // SHADER_LOAD_H