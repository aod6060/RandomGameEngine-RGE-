#include "sys.h"


Timer::Timer() {
	this->curr = 0;
	this->pre = 0;
	this->fixedUpdateTime = 0.0f;
	this->variableDelta = 0.0f;
}

void Timer::init() {
	this->pre = SDL_GetTicks();
}

void Timer::update() {
	this->curr = SDL_GetTicks();

	this->variableDelta = ((float)(curr - pre)) / 1000.0f;

	this->pre = this->curr;

	this->fixedUpdateTime += this->variableDelta;
}

float Timer::getVariableDelta() {
	return this->variableDelta;
}

bool Timer::isFixedUpdate() {
	return this->fixedUpdateTime >= UPS_60;
}

void Timer::resetFixedUpdate() {
	this->fixedUpdateTime = 0.0f;
}
