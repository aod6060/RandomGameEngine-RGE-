#ifndef WIN_H
#define WIN_H


class IApp {
public:
	// This is for initiaization
	virtual void init() = 0;
	// Variable Updates
	virtual void update(float delta) = 0;
	// Fixed Updates
	virtual void fixedUpdate() = 0;
	// Rendering
	virtual void render() = 0;
	// Release 
	virtual void release() = 0;
};

class WindowManager {
private:
	static WindowManager* instance;

	Uint32 width; // Width of screen
	Uint32 height; // height of screen
	SDL_Window* window;
	SDL_GLContext context;
	bool isRunning;
	IApp* app;

	Timer timer;

	// Constuctors
	WindowManager();

	WindowManager(WindowManager&) {}

	WindowManager& operator = (WindowManager&) {}

public:

	static WindowManager* getInstance();

	static void destroy();

	void init(std::string cap, Uint32 width, Uint32 height, IApp* app = 0);

	void update();

	void release();

	Uint32 getWidth();

	Uint32 getHeight();

	void exit();

};

#endif
