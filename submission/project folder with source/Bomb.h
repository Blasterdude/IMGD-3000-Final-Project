// Bomb.h

#ifndef __BOMB_H__
#define __BOMB_H__

// Includes
#include "Object.h"

class Bomb : public df::Object {
private:
	int time_to_live;

public:
	// Initial constructor.
	Bomb();

	// Handles events.
	int eventHandler(const df::Event *p_e);

	// Countdown until explosion finished.
	void step();
};

#endif