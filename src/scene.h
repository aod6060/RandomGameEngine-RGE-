#ifndef SCENE_H
#define SCENE_H

class Scene {
private:
	Program program;
	std::map<std::string, MeshOBJ> meshes;
	std::vector<Entity> entities;
	Camera camera;
	Light light;
public:

	void init(std::string fn);

	void update(float delta);

	void fixedUpdate();

	void render();

	void release();

	Program* getProgram();

	MeshOBJ* getMesh(std::string name);
};

#endif
