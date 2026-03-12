#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include "Vertex.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

struct ModelResource {
	std::string file;
	int id;
};

class Model {
private:

public:
	Model() {}
	~Model() {}
	ModelResource *mr;
	GLuint iboId;
	GLuint wiredIboId;
	GLuint vboId;
	int nrIndices, nrIndicesWired;
	int nrVertex;
	std::vector<Vertex> Vvertex;
	void Load(std::string file);
	void GenerateTerrain(float DimCell, int CellNr, float offset);
	void UpdateTerrain(float DimCell, int CellNr, int mutare);
};