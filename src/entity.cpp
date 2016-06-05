#include "sys.h"

void Entity::init(Json::Value& value) {

	this->meshName = value["mesh-name"].asString();

	this->transform.init(
		util_jsonToVec3(value["transform"]["pos"]),
		util_jsonToVec3(value["transform"]["rot"])
	);
}

void Entity::update(float delta) {
	// Do nothing
}

void Entity::fixedUpdate() {
	// Do nothing
}

void Entity::render(Scene& scene) {
	this->transform.getModelMatrix(*scene.getProgram());
	scene.getMesh(this->meshName)->render(*scene.getProgram());
}

void Entity::release() {
	// Do nothing
}