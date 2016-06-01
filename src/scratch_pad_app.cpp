#include "sys.h"

// This is for initiaization
void ScratchPadApp::init() {
	std::cout << "Hello, from init. " << std::endl;
}
// Variable Updates
void ScratchPadApp::update(float delta) {
	std::cout << "Hello, from update. " << std::endl;
}
// Fixed Updates
void ScratchPadApp::fixedUpdate() {
	std::cout << "Hello, from fixedUpdate. " << std::endl;
}
// Rendering
void ScratchPadApp::render() {
	std::cout << "Hello, from render. " << std::endl;
}
// Release 
void ScratchPadApp::release() {
	std::cout << "Hello, from release. " << std::endl;
}