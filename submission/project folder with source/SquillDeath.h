// SquillDeath.h

#ifndef __SQUILL_DEATH_H__
#define __SQUILL_DEATH_H__

// Includes
#include "Object.h"

class SquillDeath : public df::Object {
private:
	int time_to_live;

public:
	// Initial constructor.
	SquillDeath();

	// Handles events.
	int eventHandler(const df::Event *p_e);

	// Countdown until death finished.
	void step();
};

#endif