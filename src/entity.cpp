#include "sys.h"

void Entity::init(Json::Value& value) {

	this->meshName = value["mesh-name"].asString();
	this->materialName = value["material-name"].asString();

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
	scene.getMaterial(this->materialName)->bind();
	this->transform.getModelMatrix(*scene.getProgram());
	Material::setUniforms(*scene.getProgram(), *scene.getMaterial(this->materialName));
	scene.getMesh(this->meshName)->render(*scene.getProgram());
	scene.getMaterial(this->materialName)->unbind();
}

void Entity::release() {
	// Do nothing
}