#include "sys.h"

void Scene::init(std::string fn) {
	glEnable(GL_DEPTH_TEST);

	// Init Program
	program.init("data/shaders/main_vert.glsl", "data/shaders/main_frag.glsl");
	program.bind();
	// Init Attributes (This will change when I start working with layouts)
	program.getAttributes()->create("vertices");
	program.getAttributes()->create("texCoords");
	program.getAttributes()->create("normals");
	// Init Uniforms
	program.getUniforms()->create("projection");
	program.getUniforms()->create("view");
	program.getUniforms()->create("model");
	program.unbind();

	// Load scene file
	std::ifstream in(fn.c_str(), std::ifstream::binary);
	Json::Value root;

	in >> root;

	// Load Meshes
	Json::Value meshes = root["meshes"];
	if (meshes.isArray()) {
		for (int i = 0; i < meshes.size(); i++) {
			if (meshes[i].isObject()) {
				std::string name = meshes[i]["name"].asString();
				std::string location = meshes[i]["location"].asString();
				this->meshes[name].init(location);
			}
		}
	}
	// Load Entities
	Json::Value entities = root["entities"];
	if (entities.isArray()) {
		for (int i = 0; i < entities.size(); i++) {
			if (entities[i].isObject()) {
				Entity entity;
				entity.init(entities[i]);
				this->entities.push_back(entity);
			}
		}
	}
	// Setup Camera
	Json::Value camera = root["camera"];
	this->camera.init(
		util_jsonToVec3(camera["pos"]),
		util_jsonToVec2(camera["rot"]),
		camera["fov"].asFloat()
	);

	in.close();
}

void Scene::update(float delta) {
	// Do Nothing for now
	for (int i = 0; i < this->entities.size(); i++) {
		this->entities[i].update(delta);
	}
}

void Scene::fixedUpdate() {
	camera.fixedUpdate();

	for (int i = 0; i < this->entities.size(); i++) {
		this->entities[i].fixedUpdate();
	}
}

void Scene::render() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	program.bind();

	camera.getProjectionMatrix(this->program);
	camera.getViewMatrix(this->program);

	for (int i = 0; i < this->entities.size(); i++) {
		this->entities[i].render(*this);
	}

	program.unbind();
}

void Scene::release() {
	for (int i = 0; i < this->entities.size(); i++) {
		this->entities[i].release();
	}

	program.release();
}

Program* Scene::getProgram() {
	return &this->program;
}

MeshOBJ* Scene::getMesh(std::string name) {
	return &this->meshes[name];
}