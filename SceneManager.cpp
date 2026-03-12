#include "stdafx.h"
#include <ctime>
#include "SceneManager.h"

using namespace std;


SceneManager::SceneManager()
{
	R = 800;
	r = 400;
	color.x = 0.2f;
	color.y = 1.0f;
	color.z = 0.1f;

	obj = nullptr;
	teren = nullptr;
	skybox = nullptr;
	fire = nullptr;
	light = nullptr;
}

SceneManager* SceneManager::spSInstance = nullptr;

SceneManager* SceneManager::getSInstance()
{
    if (!spSInstance)
    {
        spSInstance = new SceneManager();
    }
    return spSInstance;
}

SceneManager::~SceneManager() {
	for (auto& it : objectMap)
	{
		delete it.second;
		it.second = nullptr;
	}
	objectMap.clear();

	for (auto& it : cameraMap)
	{
		delete it.second;
		it.second = nullptr;
	}
	cameraMap.clear();

	commandMap.clear();

	for (auto& it : fireMap)
	{
		delete it.second;
		it.second = nullptr;
	}
	fireMap.clear();

	for (auto& it : lightMap)
	{
		delete it.second;
		it.second = nullptr;
	}
	lightMap.clear();

	delete teren;
	delete skybox;
}

void SceneManager::Delete() {
	if (spSInstance) {
		delete spSInstance;
		spSInstance = nullptr;
	}
}

/*-------------------------SCENA-------------------------*/

void SceneManager::print_xmlS(xml_node<>* pRoot, int nr_spaces)
{
	if (pRoot)
	{
		SceneManager::print_nodeS(pRoot, nr_spaces);
		if (pRoot->first_node() && pRoot->value_size() == 0)
		{
			SceneManager::print_xmlS(pRoot->first_node(), nr_spaces + 4);
		}
		SceneManager::print_xmlS(pRoot->next_sibling(), nr_spaces);
	}
}

void SceneManager::print_nodeS(xml_node<>* pNode, int nr_spaces)
{

	if (!(pNode && pNode->first_node())) {
		return;
	}

	xml_node<>* auxNode = pNode->first_node();
	xml_node<>* childNode = auxNode->first_node();
	xml_node<>* pNode1;

	//parsez ceata
	pNode1 = pNode->first_node("fog");
	if (pNode1) {
	auxNode = pNode1->first_node("color");
	childNode = auxNode->first_node();
	this->color.x = atof(auxNode->first_node("r")->value());
	this->color.y = atof(auxNode->first_node("g")->value());
	this->color.z = atof(auxNode->first_node("b")->value());

	auxNode = pNode1->first_node("R");
	this->R = atof(auxNode->value());

	auxNode = pNode1->first_node("r");
	this->r = atof(auxNode->value());
	}

	// parsez camerele
	pNode->first_node("cameras");
		auxNode = pNode->first_node("camera");
		while (auxNode)
		{
			this->cam = new Camera();

			childNode = auxNode->first_node("position");
			cam->position.x = atof(childNode->first_node("x")->value());
			cam->position.y = atof(childNode->first_node("y")->value());
			cam->position.z = atof(childNode->first_node("z")->value());

			childNode = auxNode->first_node("target");
			cam->target.x = atof(childNode->first_node("x")->value());
			cam->target.y = atof(childNode->first_node("y")->value());
			cam->target.z = atof(childNode->first_node("z")->value());

			childNode = auxNode->first_node("up");
			cam->up.x = atof(childNode->first_node("x")->value());
			cam->up.y = atof(childNode->first_node("y")->value());
			cam->up.z = atof(childNode->first_node("z")->value());

			childNode = auxNode->first_node("translationSpeed");
			cam->moveSpeed = atof(childNode->first_node()->value());

			childNode = auxNode->first_node("rotationSpeed");
			cam->rotateSpeed = atof(childNode->first_node()->value());

			childNode = auxNode->first_node("fov");
			cam->fov = atof(childNode->first_node()->value());

			childNode = auxNode->first_node("near");
			cam->nearPlane = atof(childNode->first_node()->value());

			childNode = auxNode->first_node("far");
			cam->farPlane = atof(childNode->first_node()->value());

			int camera_id = atoi(auxNode->first_attribute()->value());
			cameraMap.insert(pair<int, Camera*>(camera_id, cam));
			auxNode = auxNode->next_sibling();
		}

	//parsez comanda
	pNode->first_node("controls");
	
		auxNode = pNode->first_node("control");
		while (auxNode)
		{
			childNode = auxNode->first_node("key");
			char key_aux[2];
			strcpy(key_aux, childNode->first_node()->value());
			key = key_aux[0];

			childNode = auxNode->first_node("action");
			action = childNode->first_node()->value();

			commandMap.insert(pair<char, string>(key, action));
			auxNode = auxNode->next_sibling();
		}

	//parsez lumina
	pNode->first_node("lights");
	
		auxNode = pNode->first_node("light");
		while (auxNode) {
			int light_id = atoi(auxNode->first_attribute()->value());
			this->light = new Light();

			childNode = auxNode->first_node("ambiental_color");
			light->ambiental_color.x = atof(childNode->first_node("r")->value());
			light->ambiental_color.y = atof(childNode->first_node("g")->value());
			light->ambiental_color.z = atof(childNode->first_node("b")->value());

			childNode = auxNode->first_node("difuse_color");
			light->difuse_color.x = atof(childNode->first_node("r")->value());
			light->difuse_color.y = atof(childNode->first_node("g")->value());
			light->difuse_color.z = atof(childNode->first_node("b")->value());

			childNode = auxNode->first_node("specular_color");
			light->specular_color.x = atof(childNode->first_node("r")->value());
			light->specular_color.y = atof(childNode->first_node("g")->value());
			light->specular_color.z = atof(childNode->first_node("b")->value());

			childNode = auxNode->first_node("position");
			light->position.x = atof(childNode->first_node("x")->value());
			light->position.y = atof(childNode->first_node("y")->value());
			light->position.z = atof(childNode->first_node("z")->value());

			childNode = auxNode->first_node("obj_id");
			light->obj_id = atoi(childNode->value());

			childNode = auxNode->first_node("specPower");
			light->SpecPower = atoi(childNode->value());

			lightMap.insert(pair<int, Light*>(light_id, light));
			auxNode = auxNode->next_sibling();
		}


	//parsez obiectele
	pNode->first_node("objects");
		auxNode = pNode->first_node("object");
		while (auxNode)
		{
			childNode = auxNode->first_node("type"); //type

			//verific ca modelul sa fie normal
			if (!strcmp(childNode->value(), "normal")) {

				int obj_id = atoi(auxNode->first_attribute()->value());
				this->obj = new SceneObject();

				childNode = auxNode->first_node("model");
				int model_id = atoi(childNode->value());
				obj->loadedmodel = ResourceManager::getInstance()->loadModel(model_id);

				childNode = auxNode->first_node("shader");
				int shader_id = atoi(childNode->value());
				obj->loadedshader = ResourceManager::getInstance()->loadShader(shader_id);

				childNode = auxNode->first_node("name");
				obj->name = childNode->value();

				childNode = auxNode->first_node("textures");
				xml_node<>* child2Node = childNode->first_node("texture");

				//parcurg id-urile texturii
				while (child2Node) {
					int texture_id = atoi(child2Node->first_attribute()->value());
					Texture* el = ResourceManager::getInstance()->loadTexture(texture_id);
					obj->loadedtexture.push_back(el);
					obj->nr_textures++;
					child2Node = child2Node->next_sibling();
				}

				childNode = auxNode->first_node("position");
				obj->position.x = atof(childNode->first_node("x")->value());
				obj->position.y = atof(childNode->first_node("y")->value());
				obj->position.z = atof(childNode->first_node("z")->value());

				childNode = auxNode->first_node("rotation");
				obj->rotation.x = atof(childNode->first_node("x")->value());
				obj->rotation.y = atof(childNode->first_node("y")->value());
				obj->rotation.z = atof(childNode->first_node("z")->value());

				childNode = auxNode->first_node("scale");
				obj->scale.x = atof(childNode->first_node("x")->value());
				obj->scale.y = atof(childNode->first_node("y")->value());
				obj->scale.z = atof(childNode->first_node("z")->value());

				obj->id = obj_id;
				objectMap.insert(pair<int, SceneObject*>(obj_id, obj));
			}
			else if (!strcmp(childNode->value(), "terrain")) {
				int o_id = atoi(auxNode->first_attribute()->value());
				this->teren = new Teren();

				childNode = auxNode->first_node("model");
				float DimCell = teren->dimCell;
				int CellNr = teren->CellNr;
				float offset = teren->offsetY;
				teren->loadedmodel = ResourceManager::getInstance()->GenerateModel(DimCell, CellNr, offset);

				childNode = auxNode->first_node("shader");
				int shader_id = atoi(childNode->value());
				teren->loadedshader = ResourceManager::getInstance()->loadShader(shader_id);

				childNode = auxNode->first_node("name");
				teren->name = childNode->value();

				childNode = auxNode->first_node("textures");
				xml_node<>* child2Node = childNode->first_node("texture");

				//parcurg id-urile texturii
				while (child2Node) {
					int texture_id = atoi(child2Node->first_attribute()->value());
					Texture* el = ResourceManager::getInstance()->loadTexture(texture_id);
					teren->loadedtexture.push_back(el);
					teren->nr_textures++;
					child2Node = child2Node->next_sibling();
				}

				childNode = auxNode->first_node("position");
				teren->position.x = atof(childNode->first_node("x")->value());
				teren->position.y = atof(childNode->first_node("y")->value());
				teren->position.z = atof(childNode->first_node("z")->value());

				childNode = auxNode->first_node("rotation");
				teren->rotation.x = atof(childNode->first_node("x")->value());
				teren->rotation.y = atof(childNode->first_node("y")->value());
				teren->rotation.z = atof(childNode->first_node("z")->value());

				childNode = auxNode->first_node("scale");
				teren->scale.x = atof(childNode->first_node("x")->value());
				teren->scale.y = atof(childNode->first_node("y")->value());
				teren->scale.z = atof(childNode->first_node("z")->value());

				childNode = auxNode->first_node("inaltimi");
				teren->inaltimi.x = atof(childNode->first_node("r")->value());
				teren->inaltimi.y = atof(childNode->first_node("g")->value());
				teren->inaltimi.z = atof(childNode->first_node("b")->value());

				teren->id = o_id;
			}
			else if (!strcmp(childNode->value(), "SkyBox")) {

				int sky_id = atoi(auxNode->first_attribute()->value());
				this->skybox = new Skybox();

				childNode = auxNode->first_node("model");
				int model_id = atoi(childNode->value());
				skybox->loadedmodel = ResourceManager::getInstance()->loadModel(model_id);

				childNode = auxNode->first_node("shader");
				int shader_id = atoi(childNode->value());
				skybox->loadedshader = ResourceManager::getInstance()->loadShader(shader_id);

				childNode = auxNode->first_node("name");
				skybox->name = childNode->value();

				childNode = auxNode->first_node("textures");
				xml_node<>* child2Node = childNode->first_node("texture");

				//parcurg id-urile texturii
				while (child2Node) {
					int texture_id = atoi(child2Node->first_attribute()->value());
					Texture* el = ResourceManager::getInstance()->loadTexture(texture_id);
					skybox->loadedtexture.push_back(el);
					skybox->nr_textures++;
					child2Node = child2Node->next_sibling();
				}

				childNode = auxNode->first_node("position");
				skybox->position.y = atof(childNode->first_node("y")->value());

				childNode = auxNode->first_node("rotation");
				skybox->rotation.x = atof(childNode->first_node("x")->value());
				skybox->rotation.y = atof(childNode->first_node("y")->value());
				skybox->rotation.z = atof(childNode->first_node("z")->value());

				childNode = auxNode->first_node("scale");
				skybox->scale.x = atof(childNode->first_node("x")->value());
				skybox->scale.y = atof(childNode->first_node("y")->value());
				skybox->scale.z = atof(childNode->first_node("z")->value());

				childNode = auxNode->first_node("followingcamera");
				if (childNode->first_node("ox")) {
					skybox->FollowingCamera.x = 1;
				}
				if (childNode->first_node("oy")) {
					skybox->FollowingCamera.y = 1;
				}
				if (childNode->first_node("oz")) {
					skybox->FollowingCamera.z = 1;
				}

				skybox->id = sky_id;

			}
			else if (!strcmp(childNode->value(), "fire")) {
				int obj_id = atoi(auxNode->first_attribute()->value());
				this->fire = new Fire();

				childNode = auxNode->first_node("model");
				int model_id = atoi(childNode->value());
				fire->loadedmodel = ResourceManager::getInstance()->loadModel(model_id);

				childNode = auxNode->first_node("shader");
				int shader_id = atoi(childNode->value());
				fire->loadedshader = ResourceManager::getInstance()->loadShader(shader_id);

				childNode = auxNode->first_node("name");
				fire->name = childNode->value();

				childNode = auxNode->first_node("textures");
				xml_node<>* child2Node = childNode->first_node("texture");

				//parcurg id-urile texturii
				while (child2Node) {
					int texture_id = atoi(child2Node->first_attribute()->value());
					Texture* el = ResourceManager::getInstance()->loadTexture(texture_id);
					fire->loadedtexture.push_back(el);
					fire->nr_textures++;
					child2Node = child2Node->next_sibling();
				}

				childNode = auxNode->first_node("position");
				fire->position.x = atof(childNode->first_node("x")->value());
				fire->position.y = atof(childNode->first_node("y")->value());
				fire->position.z = atof(childNode->first_node("z")->value());

				childNode = auxNode->first_node("rotation");
				fire->rotation.x = atof(childNode->first_node("x")->value());
				fire->rotation.y = atof(childNode->first_node("y")->value());
				fire->rotation.z = atof(childNode->first_node("z")->value());

				childNode = auxNode->first_node("scale");
				fire->scale.x = atof(childNode->first_node("x")->value());
				fire->scale.y = atof(childNode->first_node("y")->value());
				fire->scale.z = atof(childNode->first_node("z")->value());

				childNode = auxNode->first_node("DispMax");
				fire->DispMax = atof(childNode->value());

				fire->id = obj_id;
				fireMap.insert(pair<int, Fire*>(obj_id, fire));
			}
			auxNode = auxNode->next_sibling();
		}
}

// initializez scena
void SceneManager::InitScene(ESContext *esContext)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// for a
	//correct application over the background(the other drawn objects)

	std::cout << "----SCENE MANAGER INITIALISED----\n\n";
    xml_document<> doc;
    std::ifstream file("../sceneManager.xml");
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::string content(buffer.str());
    doc.parse<0>(&content[0]);

    xml_node<>* pRoot = doc.first_node();
    SceneManager::print_xmlS(pRoot, 0);
}

/*-------------------------ECRAN-------------------------*/

//parsez titlu, background si screen size
void SceneManager::print_xmlD(xml_node<>* pRoot, int nr_spaces)
{
	if (pRoot)
	{
		string title;
		xml_node<>* auxNode = pRoot->first_node();
		auxNode = pRoot->first_node("gameName");
		// parsez denumirea ecranului
		Globals::title = auxNode->value();

		//parsez ecranul
		auxNode = pRoot->first_node("defaultScreenSize");

		xml_node<>* childNode = auxNode->first_node();
		childNode = auxNode->first_node("width");
		Globals::screenWidth = atoi(childNode->first_node()->value());
		childNode = auxNode->first_node("height");
		Globals::screenHeight = atoi(childNode->first_node()->value());

		//parsez culoarea de background
		auxNode = pRoot->first_node("backgroundColor");

		Vector3 bkg;
		childNode = auxNode->first_node("r");
		Globals::r = atof(childNode->first_node()->value());
		childNode = auxNode->first_node("g");
		Globals::g = atof(childNode->first_node()->value());
		childNode = auxNode->first_node("b");
		Globals::b = atof(childNode->first_node()->value());
	}
}

// initializez fereastra
void SceneManager::InitWindow(ESContext *esContext)
{
	std::cout << "----INIT WINDOW----\n\n";
	xml_document<> doc;
	std::ifstream file("../sceneManager.xml");
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());
	doc.parse<0>(&content[0]);

	xml_node<>* pRoot = doc.first_node();
	SceneManager::print_xmlD(pRoot, 0);

	std::cout << "Title: " << Globals::title << endl;
	std::cout <<"Background: " <<"r: "<< Globals::r <<" g: " << Globals::g <<" b: "<< Globals::b <<endl;
	std::cout << "Ecran: " << "width: " << Globals::screenWidth << " height: " << Globals::screenHeight << endl << endl;

	std::cout << "WINDOW INITIALISED " <<endl;
	std::cout << endl;

}

void SceneManager::DrawScene(ESContext* escontext)
{
	map<int, SceneObject*>::iterator itrobj;
	for (itrobj = objectMap.begin(); itrobj != objectMap.end(); itrobj++) {
		itrobj->second->DrawObj(escontext);
	}

	if (teren) {
		teren->DrawObj(escontext);
	}

	if (skybox) {
		skybox->DrawObj(escontext);
	}

	map<int, Fire*>::iterator itrfire;
	for (itrfire = fireMap.begin(); itrfire != fireMap.end(); itrfire++) {
		itrfire->second->DrawObj(escontext);
	}
}

void SceneManager::UpdateTeren(ESContext* escontext, float deltatime)
{
	if (!teren || !cam)
		return;

	float dx = teren->position.x - cam->position.x;

	while (fabs(dx) > teren->dimCell)
	{
		if (dx > 0)
		{
			teren->position.x -= teren->dimCell;
			teren->loadedmodel->UpdateTerrain(teren->dimCell, teren->CellNr, 1);
		}
		else
		{
			teren->position.x += teren->dimCell;
			teren->loadedmodel->UpdateTerrain(teren->dimCell, teren->CellNr, 0);
		}

		dx = teren->position.x - cam->position.x;
	}

	float dz = teren->position.z - cam->position.z;

	while (fabs(dz) > teren->dimCell)
	{
		if (dz > 0)
		{
			teren->position.z -= teren->dimCell;
			teren->loadedmodel->UpdateTerrain(teren->dimCell, teren->CellNr, 3);
		}
		else
		{
			teren->position.z += teren->dimCell;
			teren->loadedmodel->UpdateTerrain(teren->dimCell, teren->CellNr, 2);
		}

		dz = teren->position.z - cam->position.z;
	}
}

void SceneManager::UpdateSkyBox(ESContext* escontext, float deltatime)
{
	if (skybox) {
		Camera* camera = this->cam;
		if (skybox->FollowingCamera.x == 1) {
			skybox->position.x = camera->position.x;
		}

		if (skybox->FollowingCamera.y == 1) {
			skybox->position.y = camera->position.y;
		}

		if (skybox->FollowingCamera.z == 1) {
			skybox->position.z = camera->position.z;
		}
	}
}

void SceneManager::UpdateFires(ESContext* escontext, float deltatime)
{
	map<int, Fire*>::iterator FireItr;
	for (FireItr = fireMap.begin(); FireItr != fireMap.end(); FireItr++) {
		FireItr->second->UpdateFire(escontext, deltatime);
	}
}