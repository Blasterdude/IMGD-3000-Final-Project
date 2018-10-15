// Squill.h

#ifndef __SQUILL_H__
#define __SQUILL_H__

// Includes
#include "Object.h"
#include "Position.h"
#include <string.h>
#include "EventCollision.h"
#include "Sound.h"

class Squill : public df::Object {
private:
	std::string type;				// Type of Squill.
	int hitpoints;					// Number of hits a squill can take.
	bool levelcomplete;				// True if level completed.
	bool win;						// True if game has been won.
	df::Sound *p_sound;				// For playing sounds when projectile hits Squill.
	bool gameover;					// True if game quits.

public:
	// Count of squills on screen.
	static int squill_count;
	static int level;

	// Initial constructor.
	Squill();

	// Destructor.
	~Squill();

	// Handles events.
	int eventHandler(const df::Event *p_e);

	// Sets the type of Squill.
	void setSquillType(std::string squill_type);

	// Gets the type of Squill.
	std::string getSquillType() const;

	// Sets the hitpoints of the Squill.
	void setHitpoints(int hp);

	// Gets the hitpoints of the Squill.
	int getHitpoints() const;

	// Sets the value of the number of squill on screen.
	void setSquillCount(int count);

	// Gets the value of the number of squill on screen.
	int getSquillCount() const;

	// If a Squill is hit by the projectile, and it has 1 hitpoint, mark it for delete. Otherwise, decrease its hitpoints by a value equal to the projectile's power.
	void hit(const df::EventCollision *p_collision_event, int power);
};

#endif