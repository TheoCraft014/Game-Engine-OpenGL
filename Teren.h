#pragma once
#include "Camera.h"
#include "SceneObject.h"
#include "Math.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

class Teren : public SceneObject {

public:
	Teren();
	int CellNr;
	float dimCell;
	float offsetY;
	Vector3 inaltimi;
	void sendCommonData(ESContext* esContext);
	void DrawObj(ESContext* escontext);

	void GenerateTerrain();
	void UpdateTerrain(int mutare);
	void Update(Camera* camera);
};