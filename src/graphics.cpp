#include "sys.h"


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

// Program::Attributes
Program::Attributes::Attributes() {
	this->program = 0;
}

void Program::Attributes::init(Program* program) {
	this->program = program;
}

void Program::Attributes::create(std::string name) {
	this->values[name] = glGetAttribLocation(program->getID(), name.c_str());
}

void Program::Attributes::enable(std::string name) {
	glEnableVertexAttribArray(this->values[name]);
}

void Program::Attributes::disable(std::string name) {
	glDisableVertexAttribArray(this->values[name]);
}

void Program::Attributes::pointer(std::string name, GLuint size, GLenum type, bool normalize) {
	glVertexAttribPointer(
		this->values[name],
		size,
		type,
		(normalize) ? GL_TRUE : GL_FALSE,
		0,
		0);
}

void Program::Attributes::release() {
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
	this->attributes.init(this);
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
	this->attributes.release();
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

Program::Attributes* Program::getAttributes() {
	return &this->attributes;
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
void MeshOBJ::init(std::string fn) {
	// Content
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
	}
	// Initialize StaticVertexBuffers
	vertices.init();
	texCoords.init();
	normals.init();

}
void MeshOBJ::render(Program& program) {
	vertices.bind();
	program.getAttributes()->pointer("vertices", 3, GL_FLOAT);
	texCoords.bind();
	program.getAttributes()->pointer("texCoords", 2, GL_FLOAT);
	normals.bind();
	program.getAttributes()->pointer("normals", 3, GL_FLOAT);
	normals.unbind();
	program.getAttributes()->enable("vertices");
	program.getAttributes()->enable("texCoords");
	program.getAttributes()->enable("normals");
	glDrawArrays(GL_TRIANGLES, 0, vertices.count());
	program.getAttributes()->disable("vertices");
	program.getAttributes()->disable("texCoords");
	program.getAttributes()->disable("normals");
}
void MeshOBJ::release() {
	this->vertices.release();
	this->texCoords.release();
	this->normals.release();
}
void MeshOBJ::getVertexVector(std::vector<glm::vec3>& v) {
	this->vertices.getList(v);
}

// Light
Light::Light(const glm::vec3& ambient,
	const glm::vec3& diffuse,
	const glm::vec3& specular,
	const glm::vec3& position) {

	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->position = position;
}

void Light::createUniforms(Program& prog) {
	prog.getUniforms()->create("light.ambient");
	prog.getUniforms()->create("light.diffuse");
	prog.getUniforms()->create("light.specular");
	prog.getUniforms()->create("light.position");
}

void Light::setUniforms(Program& prog, Light& light) {
	prog.getUniforms()->uniform3f("light.ambient", light.ambient);
	prog.getUniforms()->uniform3f("light.diffuse", light.diffuse);
	prog.getUniforms()->uniform3f("light.specular", light.specular);
	prog.getUniforms()->uniform3f("light.position", light.position);
}