#pragma once
#include "SceneObject.h"
#include "Math.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

class Skybox : public SceneObject {
public:
	Skybox();
	Vector3 FollowingCamera;
};