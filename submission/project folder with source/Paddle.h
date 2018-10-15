// Paddle.h

#ifndef __PADDLE_H__
#define __PADDLE_H__

// Includes
#include "Object.h"
#include "Position.h"
#include "EventKeyboard.h"

class Paddle : public df::Object {
private:

public:
	// Initial constructor.
	Paddle();

	// Destructor.
	~Paddle();

	// Event handler.
	int eventHandler(const df::Event *p_e);

	// Moves the paddle based on player input.
	void move(const df::EventKeyboard *p_keyboard_event);
};

#endif