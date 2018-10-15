// Timer.cpp

// Includes
#include "Timer.h"
#include "EventStep.h"
#include "LogManager.h"
#include "WorldManager.h"
#include "EventView.h"

// Initial constructor.
Timer::Timer() {
	// Sets object type.
	setType("Timer");

	// Sets initial time to 30.
	time = 30;

	// Sets HUD for timer to 30.
	df::WorldManager &world_manager = df::WorldManager::getInstance();
	df::EventView ev("Powerup Time Remaining:", +30, true);
	world_manager.onEvent(&ev);

	// Register for "step" event.
	registerInterest(df::STEP_EVENT);
}

// Deconstructor.
Timer::~Timer() {

}

// Handles events.
int Timer::eventHandler(const df::Event *p_e) {
	// Every second, decrease timer by 1.
	if (p_e->getType() == df::STEP_EVENT) {
		if (static_cast <const df::EventStep *> (p_e)->getStepCount() % 30 == 0) {
			time--;

			if (time < 0) {
				return 0;
			}

			df::WorldManager &world_manager = df::WorldManager::getInstance();
			df::EventView ev("Powerup Time Remaining:", -1, true);
			world_manager.onEvent(&ev);

			if (time > 0) {
				df::LogManager &log_manager = df::LogManager::getInstance();
				log_manager.writeLog("Time Left = %d.\n", time);
			}

			if (time == 0) {
				df::LogManager &log_manager = df::LogManager::getInstance();
				log_manager.writeLog("Powerup Time ended.\n");
			}

			return 1;
		
		}		
	}

	// Reached here, event ignored.
	return 0;
}

// Sets the time left on the timer. 
void Timer::setTime(int powerup_time) {
	time = powerup_time;
}

// Gets the time left on the timer.
int Timer::getTime() const {
	return time;
}