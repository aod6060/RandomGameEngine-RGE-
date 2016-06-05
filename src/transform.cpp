#include "sys.h"

// Transform
void Transform::init(const glm::vec3& pos, const glm::vec3& rot) {
	this->setPos(pos);
	this->setRot(rot);
}

void Transform::getModelMatrix(Program& prog) {
	glm::mat4 model;

	model = glm::translate(this->pos) *
			glm::rotate(glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::rotate(glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
	prog.getUniforms()->uniformMat4("model", model);
	prog.getUniforms()->uniformMat4("normalMatrix", glm::transpose(glm::inverse(model)));
}

void Transform::setPos(const glm::vec3& pos) {
	this->pos = pos;
}

glm::vec3 Transform::getPos() {
	return this->pos;
}

void Transform::setRot(const glm::vec3& rot) {
	this->rot = rot;
}

glm::vec3 Transform::getRot() {
	return this->rot;
}
// Camera
void Camera::init(const glm::vec3& pos, const glm::vec2& rot, float fov) {
	this->setPos(pos);
	this->setRot(rot);
	this->fov = fov;
}

void Camera::getProjectionMatrix(Program& prog) {
	glm::mat4 projection;
	WindowManager* wm = WindowManager::getInstance();
	projection = glm::perspective(glm::radians(this->fov), (float)wm->getWidth() / (float)wm->getHeight(), 0.1f, 1024.0f);
	prog.getUniforms()->uniformMat4("projection", projection);
}

void Camera::getViewMatrix(Program& prog) {
	glm::mat4 view;
	view = glm::rotate(glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
		   glm::rotate(glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
		   glm::translate(-this->pos);
	prog.getUniforms()->uniformMat4("view", view);
	prog.getUniforms()->uniform3f("cameraPos", this->pos);
}

void Camera::fixedUpdate() {
	if (InputManager::isGrab()) {
		float cameraSpeed = 3.0f;
		float movementSpeed = 20.0f;

		InputManager* input = InputManager::getInstance(InputManager::IMT_FIXED);

		glm::ivec2 mc;
		
		input->mousePosition(mc);

		rot.x += mc.y * UPS_60 * cameraSpeed;
		rot.y += mc.x * UPS_60 * cameraSpeed;

		if (rot.x < -90) {
			rot.x = -90;
		} else if(rot.x > 90) {
			rot.x = 90;
		}

		if (rot.y < -360.0f) {
			rot.y += 360.0f;
		} else if(rot.y > 360.0f) {
			rot.y -= 360.0f;
		}

		float yrad = glm::radians(rot.y);

		if (input->isKeyPress(InputManager::KB_W)) {
			pos.x += glm::sin(yrad) * UPS_60 * movementSpeed;
			pos.z -= glm::cos(yrad) * UPS_60 * movementSpeed;
		}

		if (input->isKeyPress(InputManager::KB_S)) {
			pos.x -= glm::sin(yrad) * UPS_60 * movementSpeed;
			pos.z += glm::cos(yrad) * UPS_60 * movementSpeed;
		}

		if (input->isKeyPress(InputManager::KB_A)) {
			pos.x -= glm::cos(yrad) * UPS_60 * movementSpeed;
			pos.z -= glm::sin(yrad) * UPS_60 * movementSpeed;
		}

		if (input->isKeyPress(InputManager::KB_D)) {
			pos.x += glm::cos(yrad) * UPS_60 * movementSpeed;
			pos.z += glm::sin(yrad) * UPS_60 * movementSpeed;
		}
	}
}

glm::vec3 Camera::getPos() {
	return this->pos;
}

void Camera::setPos(const glm::vec3& pos) {
	this->pos = pos;
}

glm::vec2 Camera::getRot() {
	return this->rot;
}

void Camera::setRot(const glm::vec2& rot) {
	this->rot = rot;
}

float Camera::getFOV() {
	return this->fov;
}

void Camera::setFOV(float fov) {
	this->fov = fov;
}