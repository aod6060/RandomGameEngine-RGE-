#include "sys.h"

InputManager* InputManager::variableInstance = 0;

InputManager* InputManager::fixedInstance = 0;

void InputManager::create() {
	InputManager::variableInstance = new InputManager();
	InputManager::fixedInstance = new InputManager();
}

void InputManager::createInputMapping(std::string name, const InputMapping& inputMapping) {
	InputManager::variableInstance->_createInputMapping(name, inputMapping);
	InputManager::fixedInstance->_createInputMapping(name, inputMapping);
}

void InputManager::setGrab(bool b) {
	if (b) {
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	else {
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}

bool InputManager::isGrab() {
	return (SDL_GetRelativeMouseMode() == SDL_TRUE) ? true : false;
}

void InputManager::toggleGrab() {
	InputManager::setGrab(!InputManager::isGrab());
}

InputManager* InputManager::getInstance(InputManagerType type) {
	InputManager* instance = 0;

	if (type == IMT_VARIABLE) {
		instance = InputManager::variableInstance;
	} else if(type == IMT_FIXED) {
		instance = InputManager::fixedInstance;
	}

	return instance;
}

void InputManager::destroy() {
	delete InputManager::variableInstance;
	delete InputManager::fixedInstance;
}

// Input Events
void InputManager::onKeyboardEvent(SDL_KeyboardEvent& e) {
	Uint32 sc = e.keysym.scancode;

	bool pressed = (e.state == SDL_PRESSED) ? true : false;

	if (pressed) {

		if (this->keyStates[sc] == IS_RELEASE) {
			this->keyStates[sc] = IS_ONCE;
		}

	}
	else {
		if (this->keyStates[sc] != IS_RELEASE) {
			this->keyStates[sc] = IS_RELEASE;
		}
	}
}

void InputManager::onMouseButtonEvent(SDL_MouseButtonEvent& e) {
	Uint32 sc = e.button - 1;

	bool pressed = (e.state == SDL_PRESSED) ? true : false;

	if (pressed) {
		if (this->mouseStates[sc] == IS_RELEASE) {
			this->mouseStates[sc] = IS_ONCE;
		}
	} else {
		if (this->mouseStates[sc] != IS_RELEASE) {
			this->mouseStates[sc] = IS_RELEASE;
		}
	}
}

void InputManager::onMouseMotionEvent(SDL_MouseMotionEvent& e) {
	if (InputManager::isGrab()) {
		this->mx += e.xrel;
		this->my += e.yrel;
	} else {
		this->mx = e.x;
		this->my = e.y;
	}
}

void InputManager::onMouseWheelEvent(SDL_MouseWheelEvent& e) {
	this->mwx = e.x;
	this->mwy = e.y;
}

InputManager::InputManager() {
	this->mx = 0;
	this->my = 0;
	this->mwx = 0;
	this->mwy = 0;
}

void InputManager::init() {
	for (int i = 0; i < InputManager::KB_SIZE; i++) {
		this->keyStates[i] = IS_RELEASE;
	}

	for (int i = 0; i < InputManager::MB_SIZE; i++) {
		this->mouseStates[i] = IS_RELEASE;
	}
}

void InputManager::update() {

	for (int i = 0; i < InputManager::KB_SIZE; i++) {
		if (this->keyStates[i] == IS_ONCE) {
			this->keyStates[i] = IS_PRESSED;
		}
	}

	for (int i = 0; i < InputManager::MB_SIZE; i++) {
		if (this->mouseStates[i] == IS_ONCE) {
			this->mouseStates[i] = IS_PRESSED;
		}
	}

	if (InputManager::isGrab()) {
		this->mx = 0;
		this->my = 0;
	}

	this->mwx = 0;
	this->mwy = 0;
}

void InputManager::doEvent(SDL_Event& e) {
	switch(e.type) {
	case SDL_KEYDOWN:
	case SDL_KEYUP:
		this->onKeyboardEvent(e.key);
		break;
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		this->onMouseButtonEvent(e.button);
		break;
	case SDL_MOUSEMOTION:
		this->onMouseMotionEvent(e.motion);
		break;
	case SDL_MOUSEWHEEL:
		this->onMouseWheelEvent(e.wheel);
		break;
	}
}

// Keyboard
bool InputManager::isKeyHit(Uint32 k) {
	return this->keyStates[k] == IS_ONCE;
}

bool InputManager::isKeyPress(Uint32 k) {
	return this->keyStates[k] == IS_ONCE || this->keyStates[k] == IS_PRESSED;
}
// Mouse
bool InputManager::isMouseButtonHit(Uint32 b) {
	return this->mouseStates[b] == IS_ONCE;
}

bool InputManager::isMouseButtonPress(Uint32 b) {
	return this->mouseStates[b] == IS_ONCE || this->mouseStates[b] == IS_PRESSED;
}

void InputManager::mousePosition(glm::ivec2& mc) {
	mc.x = mx;
	mc.y = my;
}

void InputManager::mouseWheel(glm::vec2& mw) {
	mw.x = this->mwx;
	mw.y = this->mwy;
}

// Input Mapping
void InputManager::_createInputMapping(std::string name, const InputMapping& inputMapping) {
	inputMaps[name] = inputMapping;
}

bool InputManager::isInputMappingHit(std::string name) {
	bool temp = false;

	if (this->inputMaps[name].isMouse) {
		temp = this->isMouseButtonHit(this->inputMaps[name].button);
	} else {
		temp = this->isKeyHit(this->inputMaps[name].button);
	}

	return temp;
}

bool InputManager::isInputMappingPress(std::string name) {
	bool temp = false;

	if (this->inputMaps[name].isMouse) {
		temp = this->isMouseButtonPress(this->inputMaps[name].button);
	}
	else {
		temp = this->isKeyPress(this->inputMaps[name].button);
	}

	return temp;
}