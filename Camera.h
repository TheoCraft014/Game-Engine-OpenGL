#pragma once
#include "../Utilities/Math.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

class Camera
{
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;
	float totaltime;
	float angle;
	float step;
	GLfloat moveSpeed;
	GLfloat rotateSpeed;
	GLfloat nearPlane;
	GLfloat farPlane;
	GLfloat fov;
	GLfloat deltaTime;
	Vector3 xAxis;
	Vector3 yAxis;
	Vector3 zAxis;
	Matrix viewMatrix;
	Matrix worldMatrix;
	Matrix perspectiveMatrix;

	Camera();
	void moveOx(int sens);
	void moveOy(int sens);
	void moveOz(int sens);
	void rotateOx(int sens);
	void rotateOy(int sens);
	void rotateOz(int sens);
	void updateAxes();
	void updateWorldView();
	void setDeltaTime(GLfloat time);
};

