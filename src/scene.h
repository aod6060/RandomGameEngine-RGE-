#ifndef SCENE_H
#define SCENE_H

class Scene {
private:
	Program program;
	std::map<std::string, MeshOBJ> meshes;
	std::map<std::string, Material> materials;
	std::vector<Entity> entities;
	Camera camera;
	Light light;
	Skybox skybox;

public:

	void init(std::string fn);

	void update(float delta);

	void fixedUpdate();

	void render();

	void release();

	Program* getProgram();

	MeshOBJ* getMesh(std::string name);

	Material* getMaterial(std::string name);
};

#endif
