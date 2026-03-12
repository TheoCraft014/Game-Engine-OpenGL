#pragma once
#include "stdafx.h"
#include "rapidxml.hpp"
#include <map>
#include <vector>
#include "ResourceManager.h"

using namespace std;
class SceneObject
{
public:
	// preluat din documentul xml
	int id;
	string name;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	Model* loadedmodel;
	Shader* loadedshader;
	vector<Texture*> loadedtexture;
	int nr_textures = 0;

	// depthTest;
	virtual void DrawObj(ESContext* escontext);
	virtual void sendCommonData(ESContext* escontext);
	virtual void sendSpecificData(ESContext* escontext);

	virtual ~SceneObject();
};