#ifndef GRAPHICS_H
#define GRAPHICS_H

template<typename T>
class StaticVertexBuffer {
private:
	std::vector<T> list;
	GLuint id;
public:
	StaticVertexBuffer() {
		id = 0;
	}
	void add(const T& t) {
		list.push_back(t);
	}
	void init() {
		glGenBuffers(1, &id);
		bind();
		glBufferData(GL_ARRAY_BUFFER, size(), &list[0], GL_STATIC_DRAW);
		unbind();
	}
	void bind() {
		glBindBuffer(GL_ARRAY_BUFFER, this->id);
	}
	void unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void release() {
		glDeleteBuffers(1, &id);
	}
	GLuint type_size() {
		return sizeof(T);
	}
	GLuint count() {
		return list.size();
	}
	GLuint size() {
		return type_size() * count();
	}
	void getList(std::vector<T>& v) {
		for (int i = 0; i < list.size(); i++) {
			v.push_back(list[i]);
		}
	}
};

class Program {
public:
	class Shader {
	private:
		GLuint id;
		void printLog();
	public:
		enum ShaderType {
			ST_VERTEX = 0,
			ST_FRAGMENT,
			ST_SIZE
		};
		Shader();
		void init(std::string fn, ShaderType type);
		GLuint getID();
		void release();
	};
	class Uniforms {
	private:
		Program* program;
		std::map<std::string, GLuint> values;
	public:
		Uniforms();
		void init(Program* program);
		void create(std::string name);
		// Integer Uniforms
		void uniform1i(std::string name, GLint i);
		void uniform2i(std::string name, const glm::ivec2& v);
		void uniform3i(std::string name, const glm::ivec3& v);
		void uniform4i(std::string name, const glm::ivec4& v);
		// Float Uniforms
		void uniform1f(std::string name, GLfloat f);
		void uniform2f(std::string name, const glm::vec2& v);
		void uniform3f(std::string name, const glm::vec3& v);
		void uniform4f(std::string name, const glm::vec4& v);
		// Matricies
		void uniformMat2(std::string name, const glm::mat2& m, bool transpose = false);
		void uniformMat3(std::string name, const glm::mat3& m, bool transpose = false);
		void uniformMat4(std::string name, const glm::mat4& m, bool transpose = false);
		void release();
	};
	class Attributes {
	private:
		Program* program;
		std::map<std::string, GLuint> values;
	public:
		Attributes();
		void init(Program* program);
		void create(std::string name);
		void enable(std::string name);
		void disable(std::string name);
		void pointer(std::string name, GLuint size, GLenum type, bool normalize = false);
		void release();
	};
private:
	Shader vertexShader;
	Shader fragmentShader;
	Uniforms uniforms;
	Attributes attributes;
	GLuint id;
	void printLog();
public:
	Program();
	void init(std::string vfn, std::string ffn);
	void bind();
	void unbind();
	void release();
	GLuint getID();
	Uniforms* getUniforms();
	Attributes* getAttributes();
};

class MeshOBJ {
public:
	struct Attribute {
		GLuint v1;
		GLuint v2;
		GLuint v3;
	};
	struct Face {
		Attribute vertice;
		Attribute texCoord;
		Attribute normal;
	};
private:
	StaticVertexBuffer<glm::vec3> vertices;
	StaticVertexBuffer<glm::vec2> texCoords;
	StaticVertexBuffer<glm::vec3> normals;
	// Private Methods
	void handleFace(std::string str, GLuint& vertice, GLuint& texCoord, GLuint& normal);
public:
	MeshOBJ();
	void init(std::string fn);
	void render(Program& program);
	void release();
	void getVertexVector(std::vector<glm::vec3>& v);
};

struct Light {
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 position;

	Light() {}

	Light(const glm::vec3& diffuse,
		const glm::vec3& specular,
		const glm::vec3& position);

	static void createUniforms(Program& prog);

	static void setUniforms(Program& prog, Light& light);

};
#endif
