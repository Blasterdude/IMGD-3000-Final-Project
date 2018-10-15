// Level1Notification.cpp

// Includes
#include "EventStep.h"
#include "Level1Notification.h"
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

Level1Notification::Level1Notification() {
	// Set object type.
	setType("Level 1 Notification");

	setAltitude(0);

	// Link to "Level 1 notification" sprite.
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	df::Sprite *p_temp_sprite = resource_manager.getSprite("level 1");
	if (!p_temp_sprite) {
		df::LogManager &log_manager = df::LogManager::getInstance();
		log_manager.writeLog("Level1Notification::Level1Notification(): Warning! Sprite '%s' not found", "level 1");
		return;
	}

	// Set sprite to temp_sprite.
	setSprite(p_temp_sprite);

	// Set solidness of object.
	setSolidness(df::SPECTRAL);
}