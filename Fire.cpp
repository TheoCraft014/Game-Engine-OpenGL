#include "stdafx.h"
#include <math.h>
#include "Fire.h"
#include "SceneManager.h"
#include "SceneObject.h"
#define PI 3.141592653

void Fire::SendCommonData(ESContext* esContext)
{
	int i;
	//model
	GLuint vboID = this->loadedmodel->vboId;
	GLuint iboID = this->loadedmodel->iboId;
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);

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

	if (this->loadedshader->uniMvp != -1) {
		glUniformMatrix4fv(this->loadedshader->uniMvp, 1, GL_FALSE, (GLfloat*)mvp.m);
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

void Fire::sendSpecificData(ESContext* escontext)
{
	//foc
	if (this->loadedshader->UniformDispMax != -1) {
		glUniform1f(this->loadedshader->UniformDispMax, this->DispMax);
	}

	if (this->loadedshader->UniformTime != -1) {
		glUniform1f(this->loadedshader->UniformTime, this->u_Time);
	}
}

#define REDUCTION_FACTOR 200
void Fire::UpdateFire(ESContext* escontext, float deltatime)
{
	this->u_Time = clock();
	if (this->u_Time >= 1) {
		this->u_Time /= REDUCTION_FACTOR;
	}
}

void Fire::DrawObj(ESContext* escontext)
{
	glUseProgram(this->loadedshader->programId);
	sendCommonData(escontext);
	this->sendSpecificData(escontext);

	glDrawElements(GL_TRIANGLES, this->loadedmodel->nrIndices, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

}
