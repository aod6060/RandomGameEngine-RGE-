#ifndef TRANSFORM_H
#define TRANSFORM_H



class Transform {
private:
	glm::vec3 pos;
	glm::vec3 rot;
public:
	void init(const glm::vec3& pos, const glm::vec3& rot);
	void getModelMatrix(Program& prog);
	void setPos(const glm::vec3& pos);
	glm::vec3 getPos();
	void setRot(const glm::vec3& rot);
	glm::vec3 getRot();
};

class Camera {
private:
	glm::vec3 pos;
	glm::vec2 rot;
	float fov;
public:
	void init(const glm::vec3& pos, const glm::vec2& rot, float fov);
	void getProjectionMatrix(Program& prog);
	void getViewMatrix(Program& prog);
	void fixedUpdate();
	glm::vec3 getPos();
	void setPos(const glm::vec3& pos);
	glm::vec2 getRot();
	void setRot(const glm::vec2& rot);
	float getFOV();
	void setFOV(float fov);
};
#endif
