#pragma once

#include <GL/glew.h>

class Texture2D {
public:
	GLuint ID;
	GLuint Width, Height;
	GLuint Internal_Format;		// GPU 存储格式
	GLuint Image_Format;		// CPU 图片格式
	
	GLuint Wrap_S;
	GLuint Wrap_T;
	GLuint Filter_Min;
	GLuint Filter_Max;
	
	Texture2D();
	void Generate(GLuint width, GLuint height, unsigned char* data);
	void Bind() const;

};
