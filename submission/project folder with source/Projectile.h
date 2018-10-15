// Projectile.h

#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

// Includes 
#include "Object.h"
#include "Position.h"
#include "EventCollision.h"
#include "EventKeyboard.h"
#include "Timer.h"
#include "Sound.h"

class Projectile : public df::Object {
private:
	int power;				// How much damage the projectile does to a Squill.
	bool gameover;			// True if game is over.
	int direction;			// int 0 through 35 representing the direction the projectile is traveling in.
	double baseSpeed;		// Base speed of projectile.
	Timer *timer;			// Timer for powerups.
	df::Sound *p_sound;		// For playing sounds when projectile hits various objects.
	int lvl;				// Current level being played.

public:

	// Initial constructor.
	Projectile();

	Projectile(int level);

	// Destructor.
	~Projectile();

	// Handles events.
	int eventHandler(const df::Event *p_e);

	// Sets the power of the projectile.
	void setPower(int pow);

	// Gets the power of the projectile.
	int getPower() const;

	// Moves the paddle based on player input.
	void move(const df::EventKeyboard *p_keyboard_event);

	// Changes projectile's direction if hits bottom of an object.
	int hitBottom(int direction);

	// Changes projectile's direction if hits top of an object.
	int hitTop(int direction);

	// Changes projectile's direction if hits left of an object.
	int hitLeft(int direction);

	// Changes projectile's direction if hits right of an object.
	int hitRight(int direction);

	// Takes in new direction and adjusts x and y velocities accordingly.
	void newVelocity(int direction);
};

#endif