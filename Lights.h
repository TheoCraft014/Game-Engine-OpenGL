#pragma once
#include "../Utilities/Math.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include


class Light
{
public:
	Light();
	Vector3 position; //direction
	Vector3 specular_color;
	Vector3 difuse_color;
	Vector3 ambiental_color;
	int ID;
	int obj_id;
	float SpecPower;
	float ratio;
};