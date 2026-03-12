#pragma once
#include "stdafx.h"
#include "rapidxml.hpp"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "SceneObject.h"
#include "Teren.h"
#include "Fire.h"
#include "Vertex.h"
#include "Globals.h"
#include "Camera.h"
#include "Skybox.h"
#include "Lights.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

using namespace std;
using namespace rapidxml;

class SceneManager
{
private:
	static SceneManager* spSInstance;
	SceneManager();
	// tii minte datele
	//calea fisierului + atribute
	map<int, SceneObject*> objectMap;
	map<int, Camera*> cameraMap;
	map<char, string> commandMap;
	map<int, Fire*> fireMap;

	//date comanda
	char key;
	string action;

	//retin date ce tin de scena
	SceneObject* obj;
	Teren* teren;
	Skybox* skybox;
	Fire* fire;
	Light* light;

public:
	static SceneManager* getSInstance();

	// retin camera
	Camera* cam;

	// sterge instanta
	static void Delete();

	map<int, Light*> lightMap;

	//fog
	float r;
	float R;
	Vector3 color;

	//initializari
	void InitScene(ESContext *escontext);
	void InitWindow(ESContext* escontext);
	void DrawScene(ESContext* escontext);

	//update
	void UpdateTeren(ESContext* escontext, float deltatime);
	void UpdateSkyBox(ESContext* escontext, float deltatime);
	void UpdateFires(ESContext* escontext, float deltatime);

	// functii de parcurgere in xml
	void print_xmlS(xml_node<>* pRoot, int nr_spaces);
	void print_nodeS(xml_node<>* pNode, int nr_spaces);
	void print_xmlD(xml_node<>* pRoot, int nr_spaces);

	~SceneManager();
};