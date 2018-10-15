// Bomb.cpp

// Includes
#include "EventStep.h"
#include "Bomb.h"
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

Bomb::Bomb() {
	// Set object type.
	setType("Bomb");
	
	// Link to "explosion" sprite.
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	df::Sprite *p_temp_sprite = resource_manager.getSprite("bomb");
	if (!p_temp_sprite) {
		df::LogManager &log_manager = df::LogManager::getInstance();
		log_manager.writeLog("Bomb::Bomb(): Warning! Sprite '%s' not found", "bomb");
		return;
	}

	setSprite(p_temp_sprite);
	setSpriteSlowdown(30);
	setAltitude(3);
	setTransparency('#');

	df::WorldManager &world_manager = df::WorldManager::getInstance();
	df::ObjectList updates = world_manager.getAllObjects();
	df::ObjectListIterator oli(&updates);

	while (!oli.isDone()) {
		if (oli.currentObject()->getType() == "SquillDeath.h") {
			world_manager.markForDelete(oli.currentObject());
		}

		oli.next();
	}

	// Sets time that the explosion has to live.
	time_to_live = getSprite()->getFrameCount();

	// Register interest in step event.
	registerInterest(df::STEP_EVENT);

	// Set solidness of object.
	setSolidness(df::SPECTRAL);
}

// Handle event.
// Return 0 if ignored, else 1.
int Bomb::eventHandler(const df::Event *p_e) {

	if (p_e->getType() == df::STEP_EVENT) {
		step();
		return 1;
	}

	// If get here, have ignored this event.
	return 0;
}

// Count down until explosion finished.
void Bomb::step() {
	time_to_live--;
	if (time_to_live <= 0){
		df::WorldManager &world_manager = df::WorldManager::getInstance();
		world_manager.markForDelete(this);
	}
}