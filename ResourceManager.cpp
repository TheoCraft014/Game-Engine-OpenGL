#include "stdafx.h"
#include "rapidxml.hpp"
#include "ResourceManager.h"
#include "Texture.h"
#include "Model.h"
#include "Shader.h"
#include "Vertex.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
using namespace rapidxml;

ResourceManager* ResourceManager::spInstance = NULL;

void ResourceManager::print_node(xml_node<>* pNode, int nr_spaces)
{
	if (strcmp(pNode->name(), "models") == 0)
	{
		string folder;
		if (strcmp(pNode->first_node()->name(), "folder") == 0)
		{
			folder = pNode->first_node()->first_attribute()->value();
		}
		xml_node<>* auxNode = pNode->first_node()->first_node();
		if (strcmp(auxNode->name(), "model") == 0)
		{
			while (auxNode)
			{
				m = new ModelResource();
				if (strcmp(auxNode->first_node()->name(), "file") == 0)
					m->file = folder + auxNode->first_node()->value();
				modelMap.insert(pair<int, ModelResource*>(stoi(auxNode->first_attribute()->value()), m));
				auxNode = auxNode->next_sibling();
			}
		}
	}

	if (strcmp(pNode->name(), "shaders") == 0)
	{
		string folder;
		if (strcmp(pNode->first_node()->name(), "folder") == 0)
		{
			folder = pNode->first_node()->first_attribute()->value();
		}

		xml_node<>* auxNode = pNode->first_node()->first_node();
		if (strcmp(auxNode->name(), "shader") == 0)
		{
			while (auxNode)
			{
				xml_node<>* attrNode = auxNode->first_node();
				s = new ShaderResource();
				while (attrNode)
				{
					if (strcmp(attrNode->name(), "vs") == 0)
						s->vsFile = folder + attrNode->value();
					if (strcmp(attrNode->name(), "fs") == 0)
						s->fsFile = folder + attrNode->value();
					attrNode = attrNode->next_sibling();
				}
				shaderMap.insert(pair<int, ShaderResource*>(stoi(auxNode->first_attribute()->value()), s));
				auxNode = auxNode->next_sibling();
			}
		}
	}

	if (strcmp(pNode->name(), "textures") == 0)
	{
		string folder;
		if (strcmp(pNode->first_node()->name(), "folder") == 0)
		{
			folder = pNode->first_node()->first_attribute()->value();
		}

		xml_node<>* auxNode = pNode->first_node()->first_node();
		if (strcmp(auxNode->name(), "texture") == 0)
		{
			while (auxNode)
			{
				t = new TextureResource();
				xml_node<>* attrNode = auxNode->first_node();
				t->type = auxNode->first_attribute("type")->value();

				while (attrNode)
				{
					if (strcmp(attrNode->name(), "file") == 0)
						t->file = folder + attrNode->value();
					if (strcmp(attrNode->name(), "min_filter") == 0)
						t->min_filter = textureAttrib.find(attrNode->value())->second;
					if (strcmp(attrNode->name(), "mag_filter") == 0)
						t->mag_filter = textureAttrib.find(attrNode->value())->second;
					if (strcmp(attrNode->name(), "wrap_s") == 0)
						t->wrap_s = textureAttrib.find(attrNode->value())->second;
					if (strcmp(attrNode->name(), "wrap_t") == 0)
						t->wrap_t = textureAttrib.find(attrNode->value())->second;
					// trec la urm nod
					attrNode = attrNode->next_sibling();

				}
				textureMap.insert(pair<int, TextureResource*>(stoi(auxNode->first_attribute()->value()), t));
				auxNode = auxNode->next_sibling();
			}
		}
	}

}

void ResourceManager::print_xml(xml_node<>* pRoot, int nr_spaces)
{
	if (pRoot)
	{
		ResourceManager::print_node(pRoot, nr_spaces);
		if (pRoot->first_node() && pRoot->value_size() == 0)
		{
			ResourceManager::print_xml(pRoot->first_node(), nr_spaces + 4);
		}
		ResourceManager::print_xml(pRoot->next_sibling(), nr_spaces);
	}
}

ResourceManager* ResourceManager::getInstance()
{
    if (!spInstance)
    {   
        spInstance = new ResourceManager();
    }
    return spInstance;
}

ResourceManager::~ResourceManager() {
	for (auto& it : lModelMap)
	{
		delete it.second;
		it.second = nullptr;
	}
	lModelMap.clear();

	for (auto& it : lTextureMap)
	{
		delete it.second;
		it.second = nullptr;
	}
	lTextureMap.clear();

	for (auto& it : lShaderMap)
	{
		delete it.second;
		it.second = nullptr;
	}
	lShaderMap.clear();

	for (auto& it : modelMap)
	{
		delete it.second;
		it.second = nullptr;
	}
	modelMap.clear();

	for (auto& it : textureMap)
	{
		delete it.second;
		it.second = nullptr;
	}
	textureMap.clear();

	for (auto& it : shaderMap)
	{
		delete it.second;
		it.second = nullptr;
	}
	shaderMap.clear();

	delete terenModel;
	terenModel = nullptr;
}

void ResourceManager::Delete() {
	if (spInstance) {
		delete spInstance;
		spInstance = nullptr;
	}
}

void ResourceManager::InitResource()
{
	cout << "--- RESOURCE MANAGER INITIALISED ---\n\n";
	textureAttrib.insert(pair<string, GLint>("LINEAR", GL_LINEAR));
	textureAttrib.insert(pair<string, GLint>("GL_NEAREST", GL_NEAREST));
	textureAttrib.insert(pair<string, GLint>("CLAMP_TO_EDGE", GL_CLAMP_TO_EDGE));
	textureAttrib.insert(pair<string, GLint>("GL_REPEAT", GL_REPEAT));
	textureAttrib.insert(pair<string, GLint>("GL_MIRRORED_REPEAT", GL_MIRRORED_REPEAT));

	xml_document<> doc;
	std::ifstream file("../resourceManager.xml");
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());
	doc.parse<0>(&content[0]);

	xml_node<>* pRoot = doc.first_node();
	ResourceManager::print_xml(pRoot, 0);
}

// creez model, textura sau shader bazat pe informatiile luate din xml
// preiau sirul retinut in ModelMap, aplic Load(file) si apoi adaug noul Model in map lModelMap
//analog pt shader si textura

Model* ResourceManager::loadModel(int id)
{
    // verifici daca exista id-ul asociat
    if (lModelMap.find(id) != lModelMap.end())
        // returneaza modelul deja existent
        return lModelMap.find(id)->second;

    Model *newModel = new Model();
	newModel->mr = modelMap.find(id)->second;
    newModel->Load(modelMap.find(id)->second->file);

    // baga noul model in map-ul cu modelurile incarcate
    lModelMap.insert(pair<int, Model*>(id, newModel));

    return newModel;
}

Model* ResourceManager::GenerateModel(float DimCell,int CellNr, float offset)
{
	terenModel = new Model();
	terenModel->mr = NULL;
	terenModel->GenerateTerrain(DimCell, CellNr, offset);

	return terenModel;
}

Texture* ResourceManager::loadTexture(int id)
{
    // verifici daca exista id-ul asociat
    if (lTextureMap.find(id) != lTextureMap.end())
        // returneaza textura deja existenta
        return lTextureMap.find(id)->second;

    Texture *newTexture = new Texture();
	newTexture->tr = textureMap.find(id)->second;
    newTexture->Load(textureMap.find(id)->second->file);

    // baga noua textura in map-ul cu texturile incarcate
    lTextureMap.insert(pair<int, Texture*>(id, newTexture));

    return newTexture;
}

Shader* ResourceManager::loadShader(int id)
{
    // verifica daca exista id-ul asociat
    if (lShaderMap.find(id) != lShaderMap.end())
        // returneaza shader-ul deja existent
        return lShaderMap.find(id)->second;

    Shader *newShader = new Shader();
	newShader->sr = shaderMap.find(id)->second;
    // verifica daca a putut fi incarcat shaderul 
    int ok = newShader->Load(shaderMap.find(id)->second->vsFile, 
							shaderMap.find(id)->second->fsFile);

    // daca s-a incarcat ca lumea fisierul il inserez
    if (ok == 0)
    {
        lShaderMap.insert(pair<int, Shader*>(id, newShader));
	} else {
		delete newShader;
		return nullptr;
	}
        
    return newShader;
}

void ResourceManager::freeResources()
{

}