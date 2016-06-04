#ifndef SCRATCH_PAD_APP_H
#define SCRATCH_PAD_APP_H


class ScratchPadApp : public IApp {
	WindowManager* wm;
	InputManager* input;
	Program prog;
	MeshOBJ mesh;
	MeshOBJ floor;
	Camera camera;
public:
	// This is for initiaization
	virtual void init();
	// Variable Updates
	virtual void update(float delta);
	// Fixed Updates
	virtual void fixedUpdate();
	// Rendering
	virtual void render();
	// Release 
	virtual void release();
};
#endif
