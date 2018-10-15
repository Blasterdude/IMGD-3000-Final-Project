// Level2Notification.cpp

// Includes
#include "EventStep.h"
#include "Level2Notification.h"
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

Level2Notification::Level2Notification() {
	// Set object type.
	setType("Level 2 Notification");

	setAltitude(0);

	// Link to "Level 2 notification" sprite.
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	df::Sprite *p_temp_sprite = resource_manager.getSprite("level 2");
	if (!p_temp_sprite) {
		df::LogManager &log_manager = df::LogManager::getInstance();
		log_manager.writeLog("Level2Notification::Level2Notification(): Warning! Sprite '%s' not found", "level 2");
		return;
	}

	// Set sprite to temp_sprite.
	setSprite(p_temp_sprite);

	// Set solidness of object.
	setSolidness(df::SPECTRAL);
}