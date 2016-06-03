#ifndef TIMER_H
#define TIMER_H

#define UPS_60 0.0166667

class Timer {
private:
	Uint32 pre;
	Uint32 curr;
	float variableDelta;

	float fixedUpdateTime;

public:

	Timer();

	void init();

	void update();

	float getVariableDelta();

	bool isFixedUpdate();

	void resetFixedUpdate();

};

#endif
