#include "stdafx.h"
#include "rapidxml.hpp"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Vertex.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "SceneObject.h"

#define PI 3.141592653
void SceneObject::sendCommonData(ESContext* esContext)
{
	int i;
	//model
	GLuint vboID = this->loadedmodel->vboId;
	GLuint iboID = this->loadedmodel->iboId;
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);

	Camera *myCamera = SceneManager::getSInstance()->cam;

	myCamera->angle += myCamera->step;
	if (myCamera->angle >= 2 * PI)
		myCamera->angle -= 2 * PI;

	Matrix mvp;
	Matrix model, Scale, RX, RY, RZ, Translatie;
	Scale.SetScale(scale.x, scale.y, scale.z);
	RX.SetRotationX(rotation.x);
	RY.SetRotationY(rotation.y);
	RZ.SetRotationY(rotation.z);
	Translatie.SetTranslation(position);

	model = Scale * RX * RY * RZ * Translatie;
	mvp = model * myCamera->viewMatrix * myCamera->perspectiveMatrix;

	if (this->loadedshader->uniMvp != -1) {
		glUniformMatrix4fv(this->loadedshader->uniMvp, 1, GL_FALSE, (GLfloat*)mvp.m);
	}

	if (this->loadedshader->uniModel != -1) {
		glUniformMatrix4fv(this->loadedshader->uniModel, 1, GL_FALSE, (GLfloat*)model.m);
	}

	if (this->loadedshader->CameraPosUniform != -1) {
		glUniform3f(this->loadedshader->CameraPosUniform, myCamera->position.x, myCamera->position.y, myCamera->position.z);
	}

	if (this->loadedshader->positionAttribute != -1)
	{
		glEnableVertexAttribArray(this->loadedshader->positionAttribute);
		glVertexAttribPointer(this->loadedshader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	for (i = 0; i < this->loadedtexture.size(); i++) {
		if (this->loadedshader->textureUniform[i] != -1) {
			glActiveTexture(GL_TEXTURE0 + i);

			if (this->loadedtexture[i]->tr->type == "2d")
				glBindTexture(GL_TEXTURE_2D, this->loadedtexture[i]->textureId);
			else if (this->loadedtexture[i]->tr->type == "cube_map")
				glBindTexture(GL_TEXTURE_CUBE_MAP, this->loadedtexture[i]->textureId);

			glUniform1i(this->loadedshader->textureUniform[i], i);

		}
	}

	if (this->loadedshader->textureAttribute != -1)
	{
		glEnableVertexAttribArray(this->loadedshader->textureAttribute);
		glVertexAttribPointer(this->loadedshader->textureAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(Vector3)));
	}

	//ceata
	int R = SceneManager::getSInstance()->R;
	int r = SceneManager::getSInstance()->r;
	Vector3 colors = SceneManager::getSInstance()->color;
	if (this->loadedshader->bigradius != -1) {
		glUniform1f(this->loadedshader->bigradius, R);
	}

	if (this->loadedshader->smallradius != -1) {
		glUniform1f(this->loadedshader->smallradius, r);
	}

	if (this->loadedshader->uniModel != -1) {
		glUniformMatrix4fv(this->loadedshader->uniModel, 1, GL_FALSE, (GLfloat*)model.m);
	}

	if (this->loadedshader->UniformFogColor != -1) {
		glUniform3f(this->loadedshader->UniformFogColor, colors.x, colors.y, colors.z);
	}

	//lumina
	if (this->loadedshader->directionLight != -1 && SceneManager::getSInstance()->lightMap.size() > 0) {
		map<int, Light*>::iterator itrLight = SceneManager::getSInstance()->lightMap.begin();
		Light* l = itrLight->second;
		glUniform3f(this->loadedshader->directionLight, l->position.x, l->position.y, l->position.z);
	}

	if (this->loadedshader->CLAmb != -1 && SceneManager::getSInstance()->lightMap.size() > 0) {
		map<int, Light*>::iterator itrLight = SceneManager::getSInstance()->lightMap.begin();
		Light* l = itrLight->second;
		glUniform3f(this->loadedshader->CLAmb, l->ambiental_color.x, l->ambiental_color.y, l->ambiental_color.z);
	}

	if (this->loadedshader->CLDif != -1 && SceneManager::getSInstance()->lightMap.size() > 0) {
		map<int, Light*>::iterator itrLight = SceneManager::getSInstance()->lightMap.begin();
		Light* l = itrLight->second;
		glUniform3f(this->loadedshader->CLDif, l->difuse_color.x, l->difuse_color.y, l->difuse_color.z);
	}

	if (this->loadedshader->CLSpec != -1 && SceneManager::getSInstance()->lightMap.size() > 0) {
		map<int, Light*>::iterator itrLight = SceneManager::getSInstance()->lightMap.begin();
		Light* l = itrLight->second;
		glUniform3f(this->loadedshader->CLSpec, l->specular_color.x, l->specular_color.y, l->specular_color.z);
	}

	if (this->loadedshader->NormalVect != -1) {
		glEnableVertexAttribArray(this->loadedshader->NormalVect);
		glVertexAttribPointer(this->loadedshader->NormalVect, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(Vector3)));
	}

	if (this->loadedshader->SpecPower != -1 && SceneManager::getSInstance()->lightMap.size() > 0) {
		map<int, Light*>::iterator itrLight = SceneManager::getSInstance()->lightMap.begin();
		Light* l = itrLight->second;
		glUniform1f(this->loadedshader->SpecPower, l->SpecPower);
	}

	if (this->loadedshader->ratio != -1 && SceneManager::getSInstance()->lightMap.size() > 0) {
		map<int, Light*>::iterator itrLight = SceneManager::getSInstance()->lightMap.begin();
		Light* l = itrLight->second;
		glUniform1f(this->loadedshader->ratio, l->ratio);

	}
}

void SceneObject::sendSpecificData(ESContext* escontext)
{
	
}

void SceneObject::DrawObj(ESContext* escontext)
{
	glUseProgram(this->loadedshader->programId);
	sendCommonData(escontext);
	sendSpecificData(escontext);

	glDrawElements(GL_TRIANGLES, this->loadedmodel->nrIndices, GL_UNSIGNED_SHORT ,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

SceneObject::~SceneObject() {
	loadedmodel = nullptr;
	loadedshader = nullptr;
	loadedtexture.clear();
}
