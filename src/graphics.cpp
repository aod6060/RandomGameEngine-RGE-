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