#include "sys.h"

void VertexArray::init() {
	glGenVertexArrays(1, &this->id);
}

void VertexArray::bind() {
	glBindVertexArray(this->id);
}

void VertexArray::unbind() {
	glBindVertexArray(0);
}

void VertexArray::release() {
	glDeleteVertexArrays(1, &this->id);
}

void VertexArray::enable(VertexArrayType type) {
	glEnableVertexAttribArray(type);
}

void VertexArray::disable(VertexArrayType type) {
	glDisableVertexAttribArray(type);
}

void VertexArray::pointer(VertexArrayType type, GLuint size, GLenum gltype, bool normalize) {
	glVertexAttribPointer(
		type, 
		size, 
		gltype, 
		(normalize) ? GL_TRUE : GL_FALSE, 
		0, 
		0);
}


// Program::Shader
Program::Shader::Shader() {
	this->id = 0;
}

void Program::Shader::printLog() {
	int len;
	char infoLog[1024];
	glGetShaderiv(this->id, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		glGetShaderInfoLog(id, len, 0, infoLog);
		std::cout << infoLog << std::endl;
	}
}

void Program::Shader::init(std::string fn, ShaderType type) {
	if (type == ShaderType::ST_VERTEX) {
		this->id = glCreateShader(GL_VERTEX_SHADER);
	} else if(type == ShaderType::ST_FRAGMENT) {
		this->id = glCreateShader(GL_FRAGMENT_SHADER);
	}
	std::string src = util_loadFile(fn);
	const char* csrc = src.c_str();
	glShaderSource(this->id, 1, &csrc, 0);
	glCompileShader(this->id);
	this->printLog();
}

GLuint Program::Shader::getID() {
	return this->id;
}

void Program::Shader::release() {
	glDeleteShader(this->id);
}
// Program::Uniforms
Program::Uniforms::Uniforms() {
	this->program = 0;
}

void Program::Uniforms::init(Program* program) {
	this->program = program;
}

void Program::Uniforms::create(std::string name) {
	this->values[name] = glGetUniformLocation(this->program->getID(), name.c_str());
	std::cout << "Uniform ID: " << this->values[name] << ": Name: " << name << std::endl;
}

// Integer Uniforms
void Program::Uniforms::uniform1i(std::string name, GLint i) {
	glUniform1i(values[name], i);
}

void Program::Uniforms::uniform2i(std::string name, const glm::ivec2& v) {
	glUniform2i(values[name], v.x, v.y);
}

void Program::Uniforms::uniform3i(std::string name, const glm::ivec3& v) {
	glUniform3i(values[name], v.x, v.y, v.z);
}

void Program::Uniforms::uniform4i(std::string name, const glm::ivec4& v) {
	glUniform4i(values[name], v.x, v.y, v.z, v.w);
}

// Float Uniforms
void Program::Uniforms::uniform1f(std::string name, GLfloat f) {
	glUniform1f(values[name], f);
}

void Program::Uniforms::uniform2f(std::string name, const glm::vec2& v) {
	glUniform2f(values[name], v.x, v.y);
}

void Program::Uniforms::uniform3f(std::string name, const glm::vec3& v) {
	glUniform3f(values[name], v.x, v.y, v.z);
}

void Program::Uniforms::uniform4f(std::string name, const glm::vec4& v) {
	glUniform4f(values[name], v.x, v.y, v.z, v.w);
}

// Matricies
void Program::Uniforms::uniformMat2(std::string name, const glm::mat2& m, bool transpose) {
	glUniformMatrix2fv(values[name], 1, (transpose) ? GL_TRUE : GL_FALSE, &m[0][0]);
}

void Program::Uniforms::uniformMat3(std::string name, const glm::mat3& m, bool transpose) {
	glUniformMatrix3fv(values[name], 1, (transpose) ? GL_TRUE : GL_FALSE, &m[0][0]);
}

void Program::Uniforms::uniformMat4(std::string name, const glm::mat4& m, bool transpose) {
	glUniformMatrix4fv(values[name], 1, (transpose) ? GL_TRUE : GL_FALSE, &m[0][0]);
}

void Program::Uniforms::release() {
	this->program = 0;
}
// Program
Program::Program() {
	this->id = 0;
}

void Program::printLog() {
	int len;
	char infoLog[1024];
	glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		glGetProgramInfoLog(this->id, len, 0, infoLog);
		std::cout << infoLog << std::endl;
	}
}

void Program::init(std::string vfn, std::string ffn) {
	vertexShader.init(vfn, Shader::ST_VERTEX);
	fragmentShader.init(ffn, Shader::ST_FRAGMENT);
	this->id = glCreateProgram();
	glAttachShader(this->id, vertexShader.getID());
	glAttachShader(this->id, fragmentShader.getID());
	glLinkProgram(this->id);
	this->printLog();
	// Init Attributes
	//this->attributes.init(this);
	// Init Uniforms
	this->uniforms.init(this);
}

void Program::bind() {
	glUseProgram(this->id);
}

void Program::unbind() {
	glUseProgram(0);
}

void Program::release() {
	this->uniforms.release();
	//this->attributes.release();
	glDetachShader(this->id, vertexShader.getID());
	glDetachShader(this->id, fragmentShader.getID());
	glDeleteProgram(this->id);
	vertexShader.release();
	fragmentShader.release();
}

GLuint Program::getID() {
	return this->id;
}

Program::Uniforms* Program::getUniforms() {
	return &this->uniforms;
}

// Texture2D
// Default Albedo
Texture2D Texture2D::albedo = Texture2D();
// Default roughness
Texture2D Texture2D::roughness = Texture2D();
// Default Reflective
Texture2D Texture2D::reflective = Texture2D();
// Default normalMap
Texture2D Texture2D::normal = Texture2D();
// Default heightMap
Texture2D Texture2D::heightMap = Texture2D();

Texture2D::Texture2D() {
	this->id = 0;
	this->width = 0;
	this->height;
}

void Texture2D::init(std::string fn) {
	SDL_Surface* surf = 0;

	surf = IMG_Load(fn.c_str());

	if (surf == 0) {
		std::cerr << fn << ": doesn't exist." << std::endl;
	}

	this->width = surf->w;
	this->height = surf->h;


	glGenTextures(1, &this->id);

	//GLuint format = GL_RGB;
	/*
	GLuint format = GL_SRGB;

	if (surf->format->BytesPerPixel == 4) {
		format = GL_SRGB_ALPHA;
	}
	*/

	GLuint internalFormat = GL_RGB;
	GLuint format = GL_RGB;

	if (surf->format->BytesPerPixel == 4) {
		internalFormat = GL_RGBA;
		format = GL_RGBA;
	}

	this->bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		internalFormat,
		this->width,
		this->height,
		0,
		format,
		GL_UNSIGNED_BYTE,
		surf->pixels);

	glGenerateMipmap(GL_TEXTURE_2D);

	this->unbind();

	SDL_FreeSurface(surf);
}

void Texture2D::bind(GLenum e) {
	glActiveTexture(e);
	glBindTexture(GL_TEXTURE_2D, this->id);
}

void Texture2D::unbind(GLenum e) {
	glActiveTexture(e);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::release() {
	glDeleteTextures(1, &id);
}

GLuint Texture2D::getID() {
	return this->id;
}

GLuint Texture2D::getWidth() {
	return this->height;
}

GLuint Texture2D::getHeight() {
	return this->width;
}

bool Texture2D::isInited() {
	return this->id != 0;
}

void Texture2D::initDefaults() {
	Texture2D::albedo.init("data/textures/default/albedo.png");
	Texture2D::roughness.init("data/textures/default/roughness.png");
	Texture2D::reflective.init("data/textures/default/reflective.png");
	Texture2D::normal.init("data/textures/default/normal.png");
	Texture2D::heightMap.init("data/textures/default/height.png");
}

void Texture2D::releaseDefaults() {
	Texture2D::albedo.release();
	Texture2D::roughness.release();
	Texture2D::reflective.release();
	Texture2D::normal.release();
	Texture2D::heightMap.release();
}

// MeshOBJ::MeshDataRegular
void MeshOBJ::MeshDataRegular::init(
	MeshOBJ* mesh, 
	std::vector<glm::vec3>& v, 
	std::vector<glm::vec2>& vt, 
	std::vector<glm::vec3>& vn, 
	std::vector<Face>& f) {
	
	for (GLuint i = 0; i < f.size(); i++) {
		// Vertices
		vertices.add(v[f[i].vertice.v1]);
		vertices.add(v[f[i].vertice.v2]);
		vertices.add(v[f[i].vertice.v3]);
		// TexCoords
		texCoords.add(vt[f[i].texCoord.v1]);
		texCoords.add(vt[f[i].texCoord.v2]);
		texCoords.add(vt[f[i].texCoord.v3]);
		// Normals
		normals.add(vn[f[i].normal.v1]);
		normals.add(vn[f[i].normal.v2]);
		normals.add(vn[f[i].normal.v3]);
		// Caculating Edges
		glm::vec3 edge1 = v[f[i].vertice.v2] - v[f[i].vertice.v1];
		glm::vec3 edge2 = v[f[i].vertice.v3] - v[f[i].vertice.v1];
		// Caculating Delta UV Coords
		glm::vec2 deltaUV1 = vt[f[i].texCoord.v2] - vt[f[i].texCoord.v1];
		glm::vec2 deltaUV2 = vt[f[i].texCoord.v3] - vt[f[i].texCoord.v1];
		// Caculate f
		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
		// Tangents
		glm::vec3 tangent;
		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent = glm::normalize(tangent);
		this->tangents.add(tangent);
		this->tangents.add(tangent);
		this->tangents.add(tangent);
		// BiTangents
		glm::vec3 bitangent;
		bitangent.x = (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent.y = (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent.z = (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent = glm::normalize(bitangent);
		this->bitangents.add(bitangent);
		this->bitangents.add(bitangent);
		this->bitangents.add(bitangent);
	}

	// Initialize StaticVertexBuffers
	vertices.init();
	texCoords.init();
	normals.init();
	tangents.init();
	bitangents.init();
	mesh->getVertexArray()->bind();
	vertices.bind();
	mesh->getVertexArray()->pointer(VertexArray::VT_VERTICES, 3, GL_FLOAT);
	mesh->getVertexArray()->enable(VertexArray::VT_VERTICES);
	texCoords.bind();
	mesh->getVertexArray()->pointer(VertexArray::VT_TEXCOORDS, 2, GL_FLOAT);
	mesh->getVertexArray()->enable(VertexArray::VT_TEXCOORDS);
	normals.bind();
	mesh->getVertexArray()->pointer(VertexArray::VT_NORMALS, 3, GL_FLOAT);
	mesh->getVertexArray()->enable(VertexArray::VT_NORMALS);
	tangents.bind();
	mesh->getVertexArray()->pointer(VertexArray::VT_TANGENTS, 3, GL_FLOAT);
	mesh->getVertexArray()->enable(VertexArray::VT_TANGENTS);
	bitangents.bind();
	mesh->getVertexArray()->pointer(VertexArray::VT_BITANGENTS, 3, GL_FLOAT);
	mesh->getVertexArray()->enable(VertexArray::VT_BITANGENTS);
	mesh->getVertexArray()->unbind();
	bitangents.unbind();
	mesh->getVertexArray()->disable(VertexArray::VT_VERTICES);
	mesh->getVertexArray()->disable(VertexArray::VT_TEXCOORDS);
	mesh->getVertexArray()->disable(VertexArray::VT_NORMALS);
	mesh->getVertexArray()->disable(VertexArray::VT_TANGENTS);
	mesh->getVertexArray()->disable(VertexArray::VT_BITANGENTS);
}

GLuint MeshOBJ::MeshDataRegular::size() {
	return this->vertices.count();
}

void MeshOBJ::MeshDataRegular::getVertexData(std::vector<glm::vec3>& v) {
	this->vertices.getList(v);
}

void MeshOBJ::MeshDataRegular::release() {
	this->vertices.release();
	this->texCoords.release();
	this->normals.release();
	this->tangents.release();
	this->bitangents.release();
}
// MeshOBJ
MeshOBJ::MeshOBJ() {}
void MeshOBJ::handleFace(std::string str, GLuint& vertice, GLuint& texCoord, GLuint& normal) {
	std::vector<std::string> temp;
	util_strSplit(str, '/', temp);
	vertice = util_toInt(temp[0]) - 1;
	texCoord = util_toInt(temp[1]) - 1;
	normal = util_toInt(temp[2]) - 1;
	temp.clear();
}
void MeshOBJ::init(std::string fn, IMeshData* data) {
	// Content
	this->data = data;
	std::vector<std::string> contents;
	std::vector<std::string> temp;
	// Holders for object file stuff
	std::vector<glm::vec3> v;
	std::vector<glm::vec2> vt;
	std::vector<glm::vec3> vn;
	std::vector<Face> f;
	// Load File
	util_loadFile(fn, contents);
	// Parse Object File
	for (GLint i = 0; i < contents.size(); i++) {
		util_strSplit(contents[i], ' ', temp);
		if (temp[0] == "v") {
			glm::vec3 t;
			t.x = util_toFloat(temp[1]);
			t.y = util_toFloat(temp[2]);
			t.z = util_toFloat(temp[3]);
			v.push_back(t);
		} else if(temp[0] == "vt") {
			glm::vec2 t;
			t.x = util_toFloat(temp[1]);
			t.y = 1.0f - util_toFloat(temp[2]);
			vt.push_back(t);
		} else if(temp[0] == "vn") {
			glm::vec3 t;
			t.x = util_toFloat(temp[1]);
			t.y = util_toFloat(temp[2]);
			t.z = util_toFloat(temp[3]);
			vn.push_back(glm::normalize(t));
		} else if(temp[0] == "f") {
			Face face;
			handleFace(temp[1], face.vertice.v1, face.texCoord.v1, face.normal.v1);
			handleFace(temp[2], face.vertice.v2, face.texCoord.v2, face.normal.v2);
			handleFace(temp[3], face.vertice.v3, face.texCoord.v3, face.normal.v3);
			f.push_back(face);
		}
		temp.clear();
	}
	// Add data to StaticVertexBuffer

	vertexArray.init();

	this->data->init(this, v, vt, vn, f);
}
void MeshOBJ::render(Program& program) {
	vertexArray.bind();
	glDrawArrays(GL_TRIANGLES, 0, data->size());
	vertexArray.unbind();
}
void MeshOBJ::release() {
	this->data->release();
	delete data;
	data = 0;
	this->vertexArray.release();
}
void MeshOBJ::getVertexVector(std::vector<glm::vec3>& v) {
	data->getVertexData(v);
}

MeshOBJ::IMeshData* MeshOBJ::createDefaultMeshData() {
	return new MeshDataRegular();
}

VertexArray* MeshOBJ::getVertexArray() {
	return &this->vertexArray;
}
// Light
Light::Light(const glm::vec3& diffuse,
			 const glm::vec3& specular,
			 const glm::vec3& position) {

	this->diffuse = diffuse;
	this->specular = specular;
	this->position = position;
}

void Light::createUniforms(Program& prog) {
	prog.getUniforms()->create("light.diffuse");
	prog.getUniforms()->create("light.specular");
	prog.getUniforms()->create("light.position");
}

void Light::setUniforms(Program& prog, Light& light) {
	prog.getUniforms()->uniform3f("light.diffuse", light.diffuse);
	prog.getUniforms()->uniform3f("light.specular", light.specular);
	prog.getUniforms()->uniform3f("light.position", light.position);
}
// Material
Material::Material() {
	this->albedo = glm::vec3(0, 0, 0);
	this->fresnel = 0;
	this->roughness = 0;
}

Material::Material(std::string fn) {
	this->albedo = glm::vec3(0, 0, 0);
	this->fresnel = 0;
	this->roughness = 0;

	std::ifstream in(fn.c_str(), std::ifstream::binary);

	Json::Value root;

	in >> root;

	if (!root["albedo"].isNull()) {
		this->albedo = util_jsonToVec3(root["albedo"]);
		std::cout << "Albedo: " <<  this->albedo.x << ", " << this->albedo.y << ", " << this->albedo.z << std::endl;
	}

	if (!root["fresnel"].isNull()) {
		this->fresnel = root["fresnel"].asFloat();
		std::cout << "Fresnel: " << this->fresnel << std::endl;
	}

	if (!root["roughness"].isNull()) {
		this->roughness = root["roughness"].asFloat();
		std::cout << "Roughness: " << this->roughness << std::endl;
	}

	if (!root["albedoMap"].isNull()) {
		this->albedoMap.init(root["albedoMap"].asString());
	}

	if (!root["roughnessMap"].isNull()) {
		this->roughnessMap.init(root["roughnessMap"].asString());
	}

	if (!root["reflectiveMap"].isNull()) {
		this->reflectiveMap.init(root["reflectiveMap"].asString());
	}

	if (!root["normalMap"].isNull()) {
		this->normalMap.init(root["normalMap"].asString());
	}

	if (!root["heightMap"].isNull()) {
		this->heightMap.init(root["heightMap"].asString());
	}

	in.close();
}

void Material::bind() {
	if (this->albedoMap.isInited()) {
		this->albedoMap.bind(GL_TEXTURE0);
	}
	else {
		Texture2D::albedo.bind(GL_TEXTURE0);
	}
	if (this->roughnessMap.isInited()) {
		this->roughnessMap.bind(GL_TEXTURE1);
	} 
	else {
		Texture2D::roughness.bind(GL_TEXTURE1);
	}
	if (this->reflectiveMap.isInited()) {
		this->reflectiveMap.bind(GL_TEXTURE2);
	} else {
		Texture2D::reflective.bind(GL_TEXTURE2);
	}
	if (this->normalMap.isInited()) {
		this->normalMap.bind(GL_TEXTURE3);
	}
	else {
		Texture2D::normal.bind(GL_TEXTURE3);
	}
	if (this->heightMap.isInited()) {
		this->heightMap.bind(GL_TEXTURE4);
	}
	else {
		Texture2D::heightMap.bind(GL_TEXTURE4);
	}
}

void Material::unbind() {
	if (this->albedoMap.isInited()) {
		this->albedoMap.unbind(GL_TEXTURE0);
	}
	else {
		Texture2D::albedo.unbind(GL_TEXTURE0);
	}

	if (this->roughnessMap.isInited()) {
		this->roughnessMap.unbind(GL_TEXTURE1);
	} 
	else {
		Texture2D::roughness.unbind(GL_TEXTURE1);
	}
	if (this->reflectiveMap.isInited()) {
		this->reflectiveMap.unbind(GL_TEXTURE2);
	}
	else {
		Texture2D::reflective.unbind(GL_TEXTURE2);
	}
	if (this->normalMap.isInited()) {
		this->normalMap.unbind(GL_TEXTURE3);
	}
	else {
		Texture2D::normal.unbind(GL_TEXTURE3);
	}
	if (this->heightMap.isInited()) {
		this->heightMap.unbind(GL_TEXTURE4);
	} else {
		Texture2D::heightMap.unbind(GL_TEXTURE4);
	}
}

void Material::release() {
	if (this->albedoMap.isInited()) {
		this->albedoMap.release();
	}
	if (this->roughnessMap.isInited()) {
		this->roughnessMap.release();
	}
	if (this->reflectiveMap.isInited()) {
		this->reflectiveMap.release();
	}
	if (this->normalMap.isInited()) {
		this->normalMap.release();
	}
	if (this->heightMap.isInited()) {
		this->heightMap.release();
	}
}

void Material::createUniforms(Program& prog) {
	prog.getUniforms()->create("material.albedo");
	prog.getUniforms()->create("material.fresnel");
	prog.getUniforms()->create("material.roughness");
	prog.getUniforms()->create("material.albedoMap");
	prog.getUniforms()->uniform1i("material.albedoMap", 0);
	prog.getUniforms()->create("material.roughnessMap");
	prog.getUniforms()->uniform1i("material.roughnessMap", 1);
	prog.getUniforms()->create("material.reflectiveMap");
	prog.getUniforms()->uniform1i("material.reflectiveMap", 2);
	prog.getUniforms()->create("material.normalMap");
	prog.getUniforms()->uniform1i("material.normalMap", 3);
	prog.getUniforms()->create("material.heightMap");
	prog.getUniforms()->uniform1i("material.heightMap", 4);
}

void Material::setUniforms(Program& prog, Material& material) {
	prog.getUniforms()->uniform3f("material.albedo", material.albedo);
	prog.getUniforms()->uniform1f("material.fresnel", material.fresnel);
	prog.getUniforms()->uniform1f("material.roughness", material.roughness);
}