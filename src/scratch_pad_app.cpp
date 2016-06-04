#include "sys.h"

// This is for initiaization
void ScratchPadApp::init() {
	wm = WindowManager::getInstance();
	input = InputManager::getInstance(InputManager::IMT_FIXED);

	glEnable(GL_DEPTH_TEST);

	// Init Program
	prog.init("data/shaders/main_vert.glsl", "data/shaders/main_frag.glsl");
	prog.bind();
	// Init Attributes (This will change when I start working with layouts)
	prog.getAttributes()->create("vertices");
	prog.getAttributes()->create("texCoords");
	prog.getAttributes()->create("normals");
	// Init Uniforms
	prog.getUniforms()->create("projection");
	prog.getUniforms()->create("view");
	prog.getUniforms()->create("model");
	prog.unbind();
	// Init Mesh
	mesh.init("data/mesh/monkey.obj");
	floor.init("data/mesh/floor.obj");

	// Init Camera
	camera.init(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec2(0.0f, 0.0f), 45.0f);
}
// Variable Updates
void ScratchPadApp::update(float delta) {
	//camera.fixedUpdate();
}
// Fixed Updates
void ScratchPadApp::fixedUpdate() {

	if (input->isKeyHit(SDL_SCANCODE_TAB)) {
		InputManager::toggleGrab();
	}

	camera.fixedUpdate();
}
// Rendering
void ScratchPadApp::render() {
	glm::mat4 model;

	/*
	projection = glm::perspective(
		glm::radians(45.0f),
		(float)wm->getWidth()/(float)wm->getHeight(),
		0.1f,
		1024.0f
	);

	view = glm::mat4(1.0f);
	*/


	model = glm::translate(glm::vec3(0.0f, 2.0f, -5.0f));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	prog.bind();

	camera.getProjectionMatrix(prog);
	camera.getViewMatrix(prog);
	
	model = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
	prog.getUniforms()->uniformMat4("model", model);
	floor.render(prog);

	model = glm::translate(glm::vec3(0.0f, 2.0f, -5.0f));
	prog.getUniforms()->uniformMat4("model", model);
	mesh.render(prog);

	prog.unbind();
}
// Release 
void ScratchPadApp::release() {
	floor.release();
	mesh.release();
	prog.release();
	wm = 0;
}