// BigPaddle.h

#ifndef __BIG_PADDLE_H__
#define __BIG_PADDLE_H__

// Includes
#include "Object.h"
#include "Position.h"
#include "EventKeyboard.h"
#include "Paddle.h"

class BigPaddle : public Paddle {
private:

public:
	// Initial constructor.
	BigPaddle();

	// Destructor.
	~BigPaddle();

	// Event handler.
	int eventHandler(const df::Event *p_e);

	// Moves the paddle based on player input.
	void move(const df::EventKeyboard *p_keyboard_event);
};

#endif