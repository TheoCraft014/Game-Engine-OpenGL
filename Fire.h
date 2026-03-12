#pragma once
#include "SceneObject.h"
#include "../Utilities/Math.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

class Fire : public SceneObject
{
public:
	Fire(){}
	float DispMax;
	float u_Time;
	void SendCommonData(ESContext *escontext);
	void UpdateFire(ESContext *escontext, float deltatime);
	void DrawObj(ESContext* escontext);
	void sendSpecificData(ESContext* escontext);
};