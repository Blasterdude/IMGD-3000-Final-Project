// PowerNotification.cpp

// Includes
#include "EventStep.h"
#include "PowerNotification.h"
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

PowerNotification::PowerNotification() {
	// Set object type.
	setType("Power Notification");

	setAltitude(0);

	// Link to "bomb notification" sprite.
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	df::Sprite *p_temp_sprite = resource_manager.getSprite("power notification");
	if (!p_temp_sprite) {
		df::LogManager &log_manager = df::LogManager::getInstance();
		log_manager.writeLog("PowerNotification::PowerNotification(): Warning! Sprite '%s' not found", "power notification");
		return;
	}

	// Set sprite to temp_sprite.
	setSprite(p_temp_sprite);

	// Set solidness of object.
	setSolidness(df::SPECTRAL);
}