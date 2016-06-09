#ifndef ENTITY_H
#define ENTITY_H


class Scene;

class Entity {
private:
	std::string meshName;
	std::string materialName;
	Transform transform;
public:

	void init(Json::Value& value);

	void update(float delta);

	void fixedUpdate();

	void render(Scene& scene);

	void release();
};

#endif
