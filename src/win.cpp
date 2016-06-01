#include "sys.h"

WindowManager* WindowManager::instance = 0;

WindowManager::WindowManager() {
	this->window = 0;
	this->context = 0;
	this->app = 0;
	this->isRunning = true;
}

WindowManager* WindowManager::getInstance() {

	if (WindowManager::instance == 0) {
		WindowManager::instance = new WindowManager();
	}

	return WindowManager::instance;
}

void WindowManager::destroy() {
	WindowManager::instance->release();

	delete WindowManager::instance;

	WindowManager::instance = 0;
}

void WindowManager::init(std::string cap, Uint32 width, Uint32 height, IApp* app) {
	// Set Variables
	this->width = width;
	this->height = height;
	this->app = app;
	// Setup SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	
	// Opengl Context and double buffer
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	this->window = SDL_CreateWindow(
		cap.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		this->width,
		this->height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
	);

	if (this->window == 0) {
		std::cerr << "Window wasn't create" << std::endl;
		this->exit();
		std::exit(1);
	}

	this->context = SDL_GL_CreateContext(this->window);

	if (this->window == 0) {
		std::cerr << "OpenGL Context Didn't initialize..." << std::endl;
		this->exit();
		std::exit(1);
	}

	// Setup glew
	glewInit();
	// Other Inits 

	// Init App if is not null
	if (app != 0) {
		app->init();
	}
}

void WindowManager::update() {

	SDL_Event e;

	while (this->isRunning) {

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				this->exit();
			}
		}

		if (app != 0) {
			app->fixedUpdate();
			app->update(0.0);
			app->render();
		}

		SDL_GL_SwapWindow(this->window);
	}
}

void WindowManager::release() {

	if (app != 0) {
		app->release();
	}

	SDL_GL_DeleteContext(this->context);
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}

Uint32 WindowManager::getWidth() {
	return this->width;
}

Uint32 WindowManager::getHeight() {
	return this->height;
}

void WindowManager::exit() {
	this->isRunning = false;
}