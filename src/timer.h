#ifndef TIMER_H
#define TIMER_H

//#define UPS_60 0.0333333
#define UPS_60 0.0166667
//#define UPS_60 0.00833333

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
