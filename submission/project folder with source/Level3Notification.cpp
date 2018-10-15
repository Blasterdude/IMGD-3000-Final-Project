// Level3Notification.cpp

// Includes
#include "EventStep.h"
#include "Level3Notification.h"
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

Level3Notification::Level3Notification() {
	// Set object type.
	setType("Level 3 Notification");

	setAltitude(0);

	// Link to "Level 3 notification" sprite.
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	df::Sprite *p_temp_sprite = resource_manager.getSprite("level 3");
	if (!p_temp_sprite) {
		df::LogManager &log_manager = df::LogManager::getInstance();
		log_manager.writeLog("Level3Notification::Level3Notification(): Warning! Sprite '%s' not found", "level 3");
		return;
	}

	// Set sprite to temp_sprite.
	setSprite(p_temp_sprite);

	// Set solidness of object.
	setSolidness(df::SPECTRAL);
}