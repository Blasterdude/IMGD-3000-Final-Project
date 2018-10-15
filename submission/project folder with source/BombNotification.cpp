// BombNotification.cpp

// Includes
#include "EventStep.h"
#include "BombNotification.h"
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

BombNotification::BombNotification() {
	// Set object type.
	setType("Bomb Notification");

	setAltitude(0);

	// Link to "bomb notification" sprite.
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	df::Sprite *p_temp_sprite = resource_manager.getSprite("bomb notification");
	if (!p_temp_sprite) {
		df::LogManager &log_manager = df::LogManager::getInstance();
		log_manager.writeLog("BombNotification::BombNotification(): Warning! Sprite '%s' not found", "bomb notification");
		return;
	}

	// Set sprite to temp_sprite.
	setSprite(p_temp_sprite);

	// Set solidness of object.
	setSolidness(df::SPECTRAL);
}