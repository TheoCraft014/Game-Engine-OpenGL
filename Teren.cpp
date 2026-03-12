#include "stdafx.h"
#include "Teren.h"
#include "SceneManager.h"
#include "SceneObject.h"
#define PI 3.141592653

Teren::Teren()
{
	CellNr = 100;
	dimCell = 30;
	offsetY = -20;
}

void Teren::sendCommonData(ESContext* esContext)
{
	this->offsetY = this->position.y;

	int i;
	//model
	GLuint vboID = this->loadedmodel->vboId;
	GLuint iboID = this->loadedmodel->iboId;
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);

	//camera
	Camera* myCamera = SceneManager::getSInstance()->cam;

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

	//shadere
	if (this->loadedshader->uniMvp != -1)
		glUniformMatrix4fv(this->loadedshader->uniMvp, 1, GL_FALSE, (GLfloat*)mvp.m);

	if (this->loadedshader->positionAttribute != -1)
	{
		glEnableVertexAttribArray(this->loadedshader->positionAttribute);
		glVertexAttribPointer(this->loadedshader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (this->loadedshader->CameraPosUniform != -1) {
		glUniform3f(this->loadedshader->CameraPosUniform, myCamera->position.x, myCamera->position.y, myCamera->position.z);
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

	if (this->loadedshader->textureAttribute2 != -1)
	{
		glEnableVertexAttribArray(this->loadedshader->textureAttribute2);
		glVertexAttribPointer(this->loadedshader->textureAttribute2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(Vector3) + sizeof(Vector2)));
	}

	if (this->loadedshader->heightU != -1)
	{
		glUniform3f(this->loadedshader->heightU, this->inaltimi.x, this->inaltimi.y, this->inaltimi.z);
	}

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
}

void Teren::DrawObj(ESContext* escontext)
{
	glUseProgram(this->loadedshader->programId);
	sendCommonData(escontext);
	sendSpecificData(escontext);

	glDrawElements(GL_TRIANGLES, this->loadedmodel->nrIndices, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void Teren::Update(Camera* camera) {
	if (!camera)
		return;

	int mutare = -1;

	float dx = position.x - camera->position.x;

	if (fabs(dx) > this->dimCell && dx > 0)
	{
		position.x -= this->dimCell;
		mutare = 1;
		UpdateTerrain(mutare);
	}
	else if (fabs(dx) > this->dimCell && dx < 0)
	{
		position.x += this->dimCell;
		mutare = 0;
		UpdateTerrain(mutare);
	}

	float dz = position.z - camera->position.z;

	if (fabs(dz) > this->dimCell && dz > 0)
	{
		position.z -= this->dimCell;
		mutare = 3;
		UpdateTerrain(mutare);
	}
	else if (fabs(dz) > this->dimCell && dz < 0)
	{
		position.z += this->dimCell;
		mutare = 2;
		UpdateTerrain(mutare);
	}
}

void Teren::GenerateTerrain() {
	loadedmodel->GenerateTerrain(dimCell, CellNr, offsetY);
}

void Teren::UpdateTerrain(int mutare) {
	loadedmodel->UpdateTerrain(dimCell, CellNr, mutare);
}