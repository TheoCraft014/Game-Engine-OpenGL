#include "stdafx.h"
#include "Shader.h"

Shader::Shader()
{

}

int Shader::Load(std::string vsFile, std::string fsFile)
{
	//VS_FILE
	char* char_array = new char[vsFile.size() + 1];
	std::strcpy(char_array, vsFile.c_str());

	GLint vertexShader = esLoadShader(GL_VERTEX_SHADER, char_array);
	delete[] char_array;

	if (vertexShader == 0)
		return -1;

	//FS_FILE
	char* char_array2 = new char[fsFile.size() + 1];
	std::strcpy(char_array2, fsFile.c_str());

	GLint fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, char_array2);
	delete[] char_array2;

	if (fragmentShader == 0)
	{
		glDeleteShader(vertexShader);
		return -2;
	}
	
	this->vertexShader = vertexShader;
	this->fragmentShader = fragmentShader;
	programId = esLoadProgram(vertexShader, fragmentShader);

	////finding location of uniforms / attributes
	positionAttribute = glGetAttribLocation(programId, "a_posL"); //pozitie
	textureUniform[0] = glGetUniformLocation(programId, "u_texture0"); //--texturi--|
	textureUniform[1] = glGetUniformLocation(programId, "u_texture1"); //			|
	textureUniform[2] = glGetUniformLocation(programId, "u_texture2"); //			|
	textureUniform[3] = glGetUniformLocation(programId, "u_texture3"); //-----------|
	textureAttribute = glGetAttribLocation(programId, "a_uv"); //coordonate vertecsi uv |
	textureAttribute2 = glGetAttribLocation(programId, "a_uv2"); //						|
	heightU = glGetUniformLocation(programId, "u_height");
	uniMvp = glGetUniformLocation(programId, "uMVP");
	uniModel = glGetUniformLocation(programId, "uM");
	CameraPosUniform = glGetUniformLocation(programId, "cameraPos");

	//fog
	smallradius = glGetUniformLocation(programId, "r");
	bigradius = glGetUniformLocation(programId, "R");
	UniformFogColor = glGetUniformLocation(programId, "fogColor");
	UniformTime = glGetUniformLocation(programId, "u_Time");
	UniformDispMax = glGetUniformLocation(programId, "u_DispMax");

	//light
	CLAmb = glGetUniformLocation(programId, "c_Lamb");
	CLSpec = glGetUniformLocation(programId, "c_Lspec");
	CLDif = glGetUniformLocation(programId, "c_Ldif");
	directionLight = glGetUniformLocation(programId, "u_light_dir");
	NormalVect = glGetAttribLocation(programId, "a_norm");
	SpecPower = glGetUniformLocation(programId, "SpecPower");
	ratio = glGetUniformLocation(programId, "ratio");

	return 0;
}