// NewTrainingFramework.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "rapidxml.hpp"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include <conio.h>
#include "Globals.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include <sstream>
#include <string>

using namespace std;
using namespace rapidxml;


float angle = 0;
Shaders myShaders;
Camera *myCamera;

float totalTime = 0;
const float Globals::frameTime = 0.05;
#define PI 3.141592653

// setez coordonatele
int lastMouseX = Globals::screenWidth / 2;
int lastMouseY = Globals::screenHeight / 2;
bool firstMouse = true;

int Init(ESContext* esContext)
{
	// parsez documentul xml Resource_Manager
	ResourceManager::getInstance()->InitResource();
	SceneManager::getSInstance()->InitScene(esContext);
	
	myCamera = SceneManager::getSInstance()->cam;

	//creation of shaders and program
	char vertexFile[43] = "../Resources/Shaders/TriangleShaderVS.vs";
	char fragmentFile[43] = "../Resources/Shaders/TriangleShaderFS.fs";
	return myShaders.Init(vertexFile, fragmentFile);
}

void Draw (ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SceneManager::getSInstance()->DrawScene(esContext);

	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface);
}

void Update ( ESContext *esContext, float deltaTime )
{
	SceneManager::getSInstance()->UpdateTeren(esContext, deltaTime);
	SceneManager::getSInstance()->UpdateSkyBox(esContext, deltaTime);
	SceneManager::getSInstance()->UpdateFires(esContext, deltaTime);

	totalTime += deltaTime;
	if (totalTime >= Globals::frameTime) {
		myCamera->setDeltaTime(totalTime);
		myCamera->updateWorldView();
		totalTime = 0;
	}
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	if (bIsPressed) {
		switch (key) {
			// W/A front/back
			// S/D left/right
			// Q/E up/down
			// Z/X roll
			// C/V pitch
			case 'W':
				myCamera->moveOz(-1);
				break;
			case 'A':
				myCamera->moveOx(-1);
				break;
			case 'S':
				myCamera->moveOz(1);
				break;
			case 'D':
				myCamera->moveOx(1);
				break;
			case 'Q':
				myCamera->moveOy(1);
				break;
			case 'E':
				myCamera->moveOy(-1);
				break;
			case 'Z':
				myCamera->rotateOx(2);
				break;
			case 'X':
				myCamera->rotateOx(-2);
				break;
			case 'C':
				myCamera->rotateOy(2);
				break;
			case 'V':
				myCamera->rotateOy(-2);
				break;
			default:
				break;
		}
	}
}

void Mouse(ESContext* esContext, unsigned char btn, bool event, int x ,int y)
{

}

void CleanUp()
{
	SceneManager::Delete();
	ResourceManager::Delete();
}

void start_window(ESContext* esContext)
{
	// citeste intai datele despre fereastra
	SceneManager::getSInstance()->InitWindow(esContext);
	esCreateWindow(esContext, (Globals::title).c_str(), Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);
	glClearColor(Globals::r, Globals::g, Globals::b, 0.0f);
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, _TCHAR* argv[])
{
	//identifying memory leaks

	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	ESContext esContext;

    esInitContext ( &esContext );
	// citeste intai datele despre fereastra
	start_window(&esContext);

	//apelam scenemanager si resourcemanager
	if (Init(&esContext) != 0) {
		std::cout <<"ERROR Init failed with code " << Init(&esContext) << std::endl;
		CleanUp();
		return 0;
	}

	Matrix mvp;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);
	esRegisterMouseFunc ( &esContext, Mouse);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();

	printf("SUCCES! Press any key UwU: \n");
	_getch();
	
	return 0;
}

