// PaddleNotification.cpp

// Includes
#include "EventStep.h"
#include "PaddleNotification.h"
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

PaddleNotification::PaddleNotification() {
	// Set object type.
	setType("Paddle Notification");

	setAltitude(0);

	// Link to "paddle notification" sprite.
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	df::Sprite *p_temp_sprite = resource_manager.getSprite("paddle notification");
	if (!p_temp_sprite) {
		df::LogManager &log_manager = df::LogManager::getInstance();
		log_manager.writeLog("PaddleNotification::PaddleNotification(): Warning! Sprite '%s' not found", "paddle notification");
		return;
	}

	// Set sprite to temp_sprite.
	setSprite(p_temp_sprite);

	// Set solidness of object.
	setSolidness(df::SPECTRAL);
}