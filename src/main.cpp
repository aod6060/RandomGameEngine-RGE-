#include "sys.h"

int main(int argc, char** argv) {


	ScratchPadApp app;

	WindowManager* wm = WindowManager::getInstance();

	wm->init("RandomGameEngine RGB", 800, 600, &app);

	wm->update();

	WindowManager::destroy();

	return 0;
}