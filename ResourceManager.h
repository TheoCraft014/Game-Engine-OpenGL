#pragma once
#include "stdafx.h"
#include "rapidxml.hpp"
#include <map>
#include "Shader.h"
#include "Model.h"
#include "Texture.h"

using namespace std;
using namespace rapidxml;


class ResourceManager
{
	private:
		static ResourceManager* spInstance;
		ResourceManager() {}

		// tii minte datele parsate
		//calea fisierului + atribute
		map<int, ModelResource*> modelMap;
		map<int, TextureResource*> textureMap;
		map<int, ShaderResource*> shaderMap;

		// tii minte astea incarate deja
		// ID-ul bufferului 
		map<int, Model*> lModelMap;
		map<int, Texture*> lTextureMap;
		map<int, Shader*> lShaderMap;

		// retin date ce tin de resurse
		ModelResource* m = nullptr;
		TextureResource* t = nullptr;
		ShaderResource* s = nullptr;
		Model* terenModel = nullptr;
		map<string, GLint> textureAttrib;

	public:
		//initializari - pot fi si in constructor
		void InitResource();
		static ResourceManager* getInstance();

		// sterge instanta
		static void Delete();

		//eliberarea zonelor de memorie alocate dinamic - se poate realiza si in destructor
		void freeResources();
		Model* loadModel(int id);
		Texture* loadTexture(int id);
		Shader* loadShader(int id);
		Model* GenerateModel(float DimCell, int CellNr, float offset); //pt teren

		// functii de parcurgere in xml
		void print_xml(xml_node<>* pRoot, int nr_spaces);
		void print_node(xml_node<>* pNode, int nr_spaces);
		~ResourceManager();
};
