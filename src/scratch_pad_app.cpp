#include "sys.h"

// This is for initiaization
void ScratchPadApp::init() {
	wm = WindowManager::getInstance();

	glEnable(GL_DEPTH_TEST);

	// Init Program
	prog.init("data/shaders/main_vert.glsl", "data/shaders/main_frag.glsl");
	prog.bind();
	// Init Attributes (This will change when I start working with layouts)
	prog.getAttributes()->create("vertices");
	// Init Uniforms
	prog.getUniforms()->create("projection");
	prog.getUniforms()->create("view");
	prog.getUniforms()->create("model");
	prog.unbind();
	// Init Static Vertex Buffer
	vertices.add(glm::vec3(-1.0f, -1.0f, 0.0f));
	vertices.add(glm::vec3(0.0f, 1.0f, 0.0f));
	vertices.add(glm::vec3(1.0f, -1.0f, 0.0f));
	vertices.init();
}
// Variable Updates
void ScratchPadApp::update(float delta) {

}
// Fixed Updates
void ScratchPadApp::fixedUpdate() {

}
// Rendering
void ScratchPadApp::render() {
	glm::mat4 projection, view, model;


	projection = glm::perspective(
		glm::radians(45.0f),
		(float)wm->getWidth()/(float)wm->getHeight(),
		0.1f,
		1024.0f
	);

	view = glm::mat4(1.0f);

	model = glm::translate(glm::vec3(0.0f, 0.0f, -5.0f));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	prog.bind();

	prog.getUniforms()->uniformMat4("projection", projection);
	prog.getUniforms()->uniformMat4("view", view);
	prog.getUniforms()->uniformMat4("model", model);

	vertices.bind();
	prog.getAttributes()->pointer("vertices", 3, GL_FLOAT);
	vertices.unbind();

	prog.getAttributes()->enable("vertices");
	glDrawArrays(GL_TRIANGLES, 0, vertices.count());
	prog.getAttributes()->disable("vertices");

	prog.unbind();
}
// Release 
void ScratchPadApp::release() {
	vertices.release();
	prog.release();
	wm = 0;
}