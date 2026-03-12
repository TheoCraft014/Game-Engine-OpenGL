#pragma once
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include <string>

struct TextureResource 
{
	std::string type;
	std::string file;
	GLint min_filter, mag_filter, wrap_s, wrap_t;
};

class Texture 
{
public:
	TextureResource* tr = NULL;
	GLuint textureId;
	Texture() {}
	~Texture() {}
	void Load(std::string file);
};