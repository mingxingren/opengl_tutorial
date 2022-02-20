#pragma once

#include <GL/glew.h>

class Texture2D {
public:
	GLuint ID;
	GLuint Width, Height;
	GLuint Internal_Format;		// GPU �洢��ʽ
	GLuint Image_Format;		// CPU ͼƬ��ʽ
	
	GLuint Wrap_S;
	GLuint Wrap_T;
	GLuint Filter_Min;
	GLuint Filter_Max;
	
	Texture2D();
	void Generate(GLuint width, GLuint height, unsigned char* data);
	void Bind() const;

};
