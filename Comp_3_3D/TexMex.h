#pragma once

#include <glad/glad.h>
#include <string>

class TexMex
{
public:
	unsigned int ID;
	std::string type;
	std::string path;

	TexMex();
	TexMex(const char* imagePath, const std::string& typeName, bool alpha = false);
	void LoadTexture(const char* imagePath, bool alpha);
	void UseTexture(GLenum textureUnit = GL_TEXTURE0);

private:
	void GenerateTexture(unsigned char* data, int width, int height, bool alpha);
};

