#include "sys.h"

// This is for initiaization
void ScratchPadApp::init() {
	std::cout << "Hello, from init. " << std::endl;

	input = InputManager::getInstance(InputManager::IMT_VARIABLE);
}
// Variable Updates
void ScratchPadApp::update(float delta) {
	if (input->isKeyHit(InputManager::KB_LEFT)) {
		std::cout << "Hello, World from left (hit)" << std::endl;
	}
}
// Fixed Updates
void ScratchPadApp::fixedUpdate() {
}
// Rendering
void ScratchPadApp::render() {
}
// Release 
void ScratchPadApp::release() {
	input = 0;
	std::cout << "Hello, from release. " << std::endl;
}