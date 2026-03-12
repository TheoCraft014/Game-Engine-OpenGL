#include "stdafx.h"
#include "Camera.h"
#include "../Utilities/Math.h"
#include "Globals.h"

Camera::Camera()
{
	totaltime = 0;
	angle = 0;
	step = 0.01;

	moveSpeed = 30;
	rotateSpeed = 5;
	nearPlane = 0.2;
	farPlane = 1500;
	fov = 0.785;

	up = Vector3(0, 1, 0);
	target = Vector3(0, 0, 0);
	position = Vector3(0, 0 , -300);

	updateAxes();
	updateWorldView();
	perspectiveMatrix.SetPerspective(fov, Globals::screenWidth * 1.0 / Globals::screenHeight, nearPlane, farPlane);
}

void Camera::moveOx(int sens)
{
	Vector3 forward;
	Vector3 vectorDeplasare;
	forward = xAxis * sens;
	vectorDeplasare = forward * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;
	updateWorldView();
}

void Camera::moveOy(int sens)
{
	Vector3 forward;
	Vector3 vectorDeplasare;
	forward = yAxis * sens;
	vectorDeplasare = forward * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;
	updateWorldView();
}

void Camera::moveOz(int sens)
{
	Vector3 forward;
	Vector3 vectorDeplasare;
	forward = zAxis * sens;
	vectorDeplasare = forward * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;
	updateWorldView();
}

void Camera::rotateOx(int sens)
{
	float angle = sens * rotateSpeed * deltaTime;

	Vector3 forward = (target - position).Normalize();
	float dist = (target - position).Length();

	Matrix rot;
	rot.SetRotationAngleAxis(angle, xAxis.x, xAxis.y, xAxis.z);

	Vector4 forward_v4 = Vector4(forward, 0.0f) * rot;
	forward = Vector3(forward_v4.x, forward_v4.y, forward_v4.z);

	Vector4 up_v4 = Vector4(up, 0.0f) * rot;
	up = Vector3(up_v4.x, up_v4.y, up_v4.z);

	target = position + forward * dist;

	updateWorldView();
}

void Camera::rotateOy(int sens)
{
	float angle = sens * rotateSpeed * deltaTime;

	Vector3 forward = (target - position).Normalize();
	float dist = (target - position).Length();

	Matrix rot;
	rot.SetRotationAngleAxis(angle, yAxis.x, yAxis.y, yAxis.z);

	Vector4 forward_v4 = Vector4(forward, 0.0f) * rot;
	forward = Vector3(forward_v4.x, forward_v4.y, forward_v4.z);

	target = position + forward * dist;

	updateWorldView();
}

void Camera::rotateOz(int sens)
{
	float angle = sens * rotateSpeed * deltaTime;

	Matrix rot;
	rot.SetRotationAngleAxis(angle, zAxis.x, zAxis.y, zAxis.z);

	Vector4 up_v4 = Vector4(up, 0.0f) * rot;
	up = Vector3(up_v4.x, up_v4.y, up_v4.z).Normalize();

	updateWorldView();
}

void Camera::updateAxes()
{
	zAxis = (position - target).Normalize();
	xAxis = up.Cross(zAxis).Normalize();
	yAxis = zAxis.Cross(xAxis).Normalize();
}

void Camera::updateWorldView()
{
	updateAxes();
	Matrix R, T;

	R.m[0][0] = xAxis.x;
	R.m[0][1] = xAxis.y;
	R.m[0][2] = xAxis.z;
	R.m[0][3] = 0;

	R.m[1][0] = yAxis.x;
	R.m[1][1] = yAxis.y;
	R.m[1][2] = yAxis.z;
	R.m[1][3] = 0;

	R.m[2][0] = zAxis.x;
	R.m[2][1] = zAxis.y;
	R.m[2][2] = zAxis.z;
	R.m[2][3] = 0;

	R.m[3][0] = 0; 
	R.m[3][1] = 0;
	R.m[3][2] = 0;
	R.m[3][3] = 1;

	T.SetTranslation(position);

	worldMatrix = R * T;
	T.SetTranslation(-position);
	R = R.Transpose();
	viewMatrix = T * R;
}

void Camera::setDeltaTime(GLfloat time)
{
	deltaTime = time;
}