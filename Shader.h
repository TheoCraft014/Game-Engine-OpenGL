#pragma once
#include <string>
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

#define MAX_TEXTURES 4

struct ShaderResource 
{
	std::string vsFile, fsFile;
};

class Shader 
{
public:
	Shader();
	~Shader() {};
	ShaderResource* sr = NULL;
	GLuint programId;
	int Load(std::string vsFile, std::string fsFile);

	GLuint vertexShader, fragmentShader;
	GLint positionAttribute;
	GLint uniMvp;
	GLint textureUniform[MAX_TEXTURES];
	GLint textureAttribute;
	GLint textureAttribute2;
	GLint heightU;
	GLint smallradius;
	GLint bigradius;
	GLint uniModel;
	GLint colorUniform;
	GLint UniformFogColor;
	GLint CameraPosUniform;
	GLint UniformTime;
	GLint UniformDispMax;

	//light
	GLint CLAmb;
	GLint CLSpec;
	GLint CLDif;
	GLint directionLight;
	GLint NormalVect; // normala vectorului
	GLint SpecPower;
	GLint ratio;
};