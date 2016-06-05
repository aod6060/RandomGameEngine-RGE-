#include "sys.h"

// This is for initiaization
void ScratchPadApp::init() {
	scene.init("data/scene/test.json");
}
// Variable Updates
void ScratchPadApp::update(float delta) {
	scene.update(delta);
}
// Fixed Updates
void ScratchPadApp::fixedUpdate() {
	InputManager* input = InputManager::getInstance(InputManager::IMT_FIXED);

	if (input->isKeyHit(InputManager::KB_TAB)) {
		InputManager::toggleGrab();
	}

	scene.fixedUpdate();
}
// Rendering
void ScratchPadApp::render() {
	scene.render();
}
// Release 
void ScratchPadApp::release() {
	scene.release();
}