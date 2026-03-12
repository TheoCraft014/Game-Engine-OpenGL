#include "stdafx.h"
#include "Model.h"

void Model::Load(std::string file)
{
	int size;
	std::vector<Vertex> v;
	std::ifstream f(file);
	std::string s;
	int i;
	int vertexNr;
	std::vector<unsigned short> indices;
	{
		std::getline(f, s, ':');
		std::getline(f, s, '\n');
		size = stof(s);

		Vertex elem;
		vertexNr = size;
		for (i = 0; i < size; i++)
		{
			std::getline(f, s, '[');
			std::getline(f, s, ',');
			elem.pos.x = stof(s);
			std::getline(f, s, ',');
			elem.pos.y = stof(s);
			std::getline(f, s, ']');
			elem.pos.z = stof(s);

			std::getline(f, s, '[');
			std::getline(f, s, ',');
			elem.norm.x = stof(s);
			std::getline(f, s, ',');
			elem.norm.y = stof(s);
			std::getline(f, s, ']');
			elem.norm.z = stof(s);

			std::getline(f, s, '[');
			std::getline(f, s, ',');
			elem.binorm.x = stof(s);
			std::getline(f, s, ',');
			elem.binorm.y = stof(s);
			std::getline(f, s, ']');
			elem.binorm.z = stof(s);

			std::getline(f, s, '[');
			std::getline(f, s, ',');
			elem.tgt.x = stof(s);
			std::getline(f, s, ',');
			elem.tgt.y = stof(s);
			std::getline(f, s, ']');
			elem.tgt.z = stof(s);

			std::getline(f, s, '[');
			std::getline(f, s, ',');
			elem.uv.x = stof(s);
			std::getline(f, s, ']');
			elem.uv.y = stof(s);

			elem.color.x = 1;
			elem.color.y = 1;
			elem.color.z = 1;

			v.push_back(elem);
		}

		std::getline(f, s, ':');

		std::getline(f, s, '\n');
		size = stof(s);
		int indicesNr = size;
		this->nrIndices = indicesNr;
		unsigned short x;
		for (i = 0; i < size / 3; i++)
		{
			std::getline(f, s, '.');
			std::getline(f, s, ',');
			x = stof(s);
			indices.push_back(x);

			std::getline(f, s, ',');
			x = stof(s);
			indices.push_back(x);

			std::getline(f, s, '\n');
			x = stof(s);
			indices.push_back(x);
		}
	}

	//buffer object
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * v.size(), v.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//buffer object
	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indices.size(), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	this->nrVertex = Vvertex.size();
}

void Model::GenerateTerrain(float DimCell, int CellNr, float offset)
{
	int dim = CellNr + 1;
	int size = dim * dim;
	int size2 = CellNr * CellNr;
	std::vector<Vertex> VVvertex(size);
	std::vector<GLushort> Iindices(6 * size2);
	int mij = dim / 2, i, j;
	for (i = 0; i < dim; i++)
		for (j = 0; j < dim; j++) {
			Vertex el = Vertex();
			el.pos.x = (i - mij) * DimCell;
			el.pos.y = offset;
			el.pos.z = (j - mij) * DimCell;

			el.uv.x = i;
			el.uv.y = j;

			el.uv2.x = float(i) / CellNr;
			el.uv2.y = float(j) / CellNr;

			el.norm.x = 0;
			el.norm.y = 1;
			el.norm.z = 0;
			this->Vvertex.push_back(el);
		}

	int k = 0;
	for (i = 0; i < CellNr; i++) {
		for (j = 0; j < CellNr; j++) {
			int base = i * (CellNr + 1) + j;
			Iindices[k++] = base;
			Iindices[k++] = base + 1;
			Iindices[k++] = base + 1 + CellNr;
			Iindices[k++] = base + 1;
			Iindices[k++] = base + 2 + CellNr;
			Iindices[k++] = base + 1 + CellNr;
		}
	}

	this->nrIndices = Iindices.size();
	this->nrVertex = VVvertex.size();

	//buffer object
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * this->Vvertex.size(), this->Vvertex.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//buffer object
	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * Iindices.size(), Iindices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model::UpdateTerrain(float DimCell, int CellNr, int mutare)
{
	int dim = CellNr + 1, i, j;
	if (mutare == -1)
		return;

	if (mutare == 0) { // OX pozitiv
		for (i = 0; i < dim; i++)
			for (j = 0; j < dim; j++) {
				this->Vvertex[i * dim + j].uv2.x += 1.0 / CellNr;
			}
	}else if (mutare == 1) { // OX negativ
		for (i = 0; i < dim; i++)
			for (j = 0; j < dim; j++) {
				this->Vvertex[i * dim + j].uv2.x -= 1.0 / CellNr;
			}
	}else if (mutare == 2) { // OZ pozitiv
		for (i = 0; i < dim; i++)
			for (j = 0; j < dim; j++) {
				this->Vvertex[i * dim + j].uv2.y += 1.0 / CellNr;
			}
	}else if (mutare == 3) { // OZ negativ
		for (i = 0; i < dim; i++)
			for (j = 0; j < dim; j++) {
				this->Vvertex[i * dim + j].uv2.y -= 1.0 / CellNr;
			}
	}

	//buffer object
	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	glBufferSubData(
		GL_ARRAY_BUFFER,
		0,
		sizeof(Vertex) * Vvertex.size(),
		Vvertex.data()
	);

}