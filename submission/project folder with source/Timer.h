// Timer.h

#ifndef __TIMER_H__
#define __TIMER_H__

// Includes
#include "Object.h"

class Timer : public df::Object {
private:
	int time;		// Time left for a powerup before it disappears/ends.

public:
	// Initial constructor.
	Timer();

	// Deconstructor.
	~Timer();

	// Handles events.
	int eventHandler(const df::Event *p_e);

	// Sets the time left on the timer. 
	void setTime(int powerup_time);

	// Gets the time left on the timer.
	int getTime() const;
};

#endif