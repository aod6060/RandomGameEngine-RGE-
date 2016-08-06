#ifndef SKYBOX_H
#define SKYBOX_H

class Skybox {
public:
	class MeshDataSkybox : public MeshOBJ::IMeshData {
	private:
		StaticVertexBuffer<glm::vec3> vertices;
	public:
		virtual void init(
			MeshOBJ* mesh,
			std::vector<glm::vec3>& v,
			std::vector<glm::vec2>& vt,
			std::vector<glm::vec3>& vn,
			std::vector<MeshOBJ::Face>& f);
		virtual GLuint size();
		virtual void getVertexData(std::vector<glm::vec3>& v);
		virtual void release();
	};
private:
	Program prog;
	MeshOBJ sphere32;
public:

	void init();

	void render(Camera& camera, Light& light);

	void release();
};

#endif
