#include "sys.h"

// Skybox::MeshDataSkybox
void Skybox::MeshDataSkybox::init(
	MeshOBJ* mesh,
	std::vector<glm::vec3>& v,
	std::vector<glm::vec2>& vt,
	std::vector<glm::vec3>& vn,
	std::vector<MeshOBJ::Face>& f) {
	for (int i = 0; i < f.size(); i++) {
		vertices.add(v[f[i].vertice.v1]);
		vertices.add(v[f[i].vertice.v2]);
		vertices.add(v[f[i].vertice.v3]);
	}

	vertices.init();

	mesh->getVertexArray()->bind();
	vertices.bind();
	mesh->getVertexArray()->pointer(VertexArray::VT_VERTICES, 3, GL_FLOAT);
	mesh->getVertexArray()->enable(VertexArray::VT_VERTICES);
	mesh->getVertexArray()->unbind();
	vertices.unbind();
	mesh->getVertexArray()->disable(VertexArray::VT_VERTICES);
}
GLuint Skybox::MeshDataSkybox::size() {
	return this->vertices.count();
}
void Skybox::MeshDataSkybox::getVertexData(std::vector<glm::vec3>& v) {
	this->vertices.getList(v);
}
void Skybox::MeshDataSkybox::release() {
	this->vertices.release();
}

// Skybox
void Skybox::init() {
	this->prog.init("data/shaders/skybox_vert.glsl", "data/shaders/skybox_frag.glsl");

	this->prog.bind();
	Light::createUniforms(prog);
	this->prog.getUniforms()->create("projection");
	this->prog.getUniforms()->create("view");
	this->prog.getUniforms()->create("model");
	this->prog.getUniforms()->create("rayOrigin");
	this->prog.getUniforms()->create("sunIntencity");
	this->prog.getUniforms()->create("innerRadius");
	this->prog.getUniforms()->create("outerRadius");
	this->prog.getUniforms()->create("reisCoefficient");
	this->prog.getUniforms()->create("miesCofficient");
	this->prog.getUniforms()->create("reisScaleHeight");
	this->prog.getUniforms()->create("miesScaleHeight");
	this->prog.getUniforms()->create("miesPreScatDir");
	this->prog.unbind();

	this->sphere32.init("data/skybox/mesh/cube.obj", new Skybox::MeshDataSkybox());
}

void Skybox::render(Camera& camera, Light& light) {
	glDisable(GL_DEPTH_TEST);
	prog.bind();
	camera.getProjectionMatrix(prog);
	camera.getViewMatrix(prog);
	prog.getUniforms()->uniformMat4("model", glm::translate(glm::vec3(0, 0, 0)));
	prog.getUniforms()->uniform3f("rayOrigin", glm::vec3(0, 6372e3, 0));
	prog.getUniforms()->uniform1f("sunIntencity", 22.0);
	prog.getUniforms()->uniform1f("innerRadius", 6371e3);
	prog.getUniforms()->uniform1f("outerRadius", 6471e3);
	prog.getUniforms()->uniform3f("reisCoefficient", glm::vec3(0.0000055, 0.000013, 0.0000224));
	prog.getUniforms()->uniform1f("miesCofficient", 0.000021);
	prog.getUniforms()->uniform1f("reisScaleHeight", 8e3);
	prog.getUniforms()->uniform1f("miesScaleHeight", 1.2e3);
	prog.getUniforms()->uniform1f("miesPreScatDir", 0.758);
	Light::setUniforms(prog, light);
	this->sphere32.render(prog);
	prog.unbind();
	glEnable(GL_DEPTH_TEST);
}

void Skybox::release() {
	this->sphere32.release();
	this->prog.release();
}