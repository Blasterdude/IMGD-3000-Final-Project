// Bomb.cpp

// Includes
#include "EventStep.h"
#include "SquillDeath.h"
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

SquillDeath::SquillDeath() {
	// Set object type.
	setType("SquillDeath");

	// Link to "squill death" sprite.
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	df::Sprite *p_temp_sprite = resource_manager.getSprite("death");
	if (!p_temp_sprite) {
		df::LogManager &log_manager = df::LogManager::getInstance();
		log_manager.writeLog("SquillDeath::SquillDeath(): Warning! Sprite '%s' not found", "death");
		return;
	}

	setSprite(p_temp_sprite);
	setSpriteSlowdown(30);
	setAltitude(2);

	// Sets time that the explosion has to live.
	time_to_live = getSprite()->getFrameCount();

	// Register interest in step event.
	registerInterest(df::STEP_EVENT);

	// Set solidness of object.
	setSolidness(df::SPECTRAL);
}

// Handle event.
// Return 0 if ignored, else 1.
int SquillDeath::eventHandler(const df::Event *p_e) {

	if (p_e->getType() == df::STEP_EVENT) {
		step();
		return 1;
	}

	// If get here, have ignored this event.
	return 0;
}

// Count down until explosion finished.
void SquillDeath::step() {
	time_to_live--;
	if (time_to_live <= 0){
		df::WorldManager &world_manager = df::WorldManager::getInstance();
		world_manager.markForDelete(this);
	}
}