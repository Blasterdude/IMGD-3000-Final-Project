// LevelComplete.cpp

// Includes.
#include "EventStep.h"
#include "EventKeyboard.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "GraphicsManager.h"
#include "GameStart.h"
#include "GameManager.h"
#include "WorldManager.h"
#include "Paddle.h"
#include "Projectile.h"
#include "LevelComplete.h"

LevelComplete::LevelComplete() {

	// Sets object type.
	setType("Level Complete");

	// Link to "level complete" sprite.
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	df::Sprite *p_temp_sprite = resource_manager.getSprite("level complete");
	if (!p_temp_sprite) {
		df::LogManager &log_manager = df::LogManager::getInstance();
		log_manager.writeLog("LevelComplete::LevelComplete(): Warning! Sprite 'level complete' not found");
	}

	else {
		setSprite(p_temp_sprite);
		setSpriteSlowdown(15);
		setTransparency('#');    // Transparent character.
		time_to_live = p_temp_sprite->getFrameCount() * 15;
	}

	// Put in center of window.
	setLocation(df::CENTER_CENTER);

	// Register for step event.
	registerInterest(df::STEP_EVENT);

	// Register for keyboard event.
	registerInterest(df::KEYBOARD_EVENT);

	// Deletes all objects from previous level.
	df::WorldManager &world_manager = df::WorldManager::getInstance();

	df::ObjectList updates = world_manager.getAllObjects();
	df::ObjectListIterator oli(&updates);

	while (!oli.isDone()) {
		if (oli.currentObject()->getType() == "Projectile" || oli.currentObject()->getType() == "Paddle" || oli.currentObject()->getType() == "Big Paddle" || oli.currentObject()->getType() == "Timer" || oli.currentObject()->getType() == "Bomb Notification" || oli.currentObject()->getType() == "Paddle Notification" || oli.currentObject()->getType() == "Power Notification") {
			delete oli.currentObject();
		}

		oli.next();
	}

	// Play "level complete" sound (Same as win sound).
	df::Sound *p_sound = df::ResourceManager::getInstance().getSound("win");
	p_sound->play();
}

LevelComplete::LevelComplete(int level) {

	// Sets object type.
	setType("Level Complete");

	// Link to "level complete" sprite.
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	df::Sprite *p_temp_sprite = resource_manager.getSprite("level complete");
	if (!p_temp_sprite) {
		df::LogManager &log_manager = df::LogManager::getInstance();
		log_manager.writeLog("LevelComplete::LevelComplete(): Warning! Sprite 'level complete' not found");
	}

	else {
		setSprite(p_temp_sprite);
		setSpriteSlowdown(15);
		setTransparency('#');    // Transparent character.
		time_to_live = p_temp_sprite->getFrameCount() * 15;
	}

	// Put in center of window.
	setLocation(df::CENTER_CENTER);

	// Register for step event.
	registerInterest(df::STEP_EVENT);

	// Register for keyboard event.
	registerInterest(df::KEYBOARD_EVENT);

	// Sets level to current level completed.
	lvl = level;

	// Deletes all objects from previous level.
	df::WorldManager &world_manager = df::WorldManager::getInstance();

	df::ObjectList updates = world_manager.getAllObjects();
	df::ObjectListIterator oli(&updates);

	while (!oli.isDone()) {
		if (oli.currentObject()->getType() == "Projectile" || oli.currentObject()->getType() == "Paddle" || oli.currentObject()->getType() == "Big Paddle" || oli.currentObject()->getType() == "Timer" || oli.currentObject()->getType() == "Bomb Notification" || oli.currentObject()->getType() == "Paddle Notification" || oli.currentObject()->getType() == "Power Notification" || oli.currentObject()->getType() == "Level 1 Notification" || oli.currentObject()->getType() == "Level 2 Notification" || oli.currentObject()->getType() == "Level 3 Notification") {
			delete oli.currentObject();
		}

		oli.next();
	}

	// Stop level music currently playing.
	df::Music *p_music;

	switch (level) {
	case 1:
		p_music = df::ResourceManager::getInstance().getMusic("level 1");
		break;

	case 2:
		p_music = df::ResourceManager::getInstance().getMusic("level 2");
		break;

	case 3:
		p_music = df::ResourceManager::getInstance().getMusic("level 3");
		break;

	default:
		p_music = df::ResourceManager::getInstance().getMusic("level 1");
		break;

	}

	p_music->stop();

	// Play "level complete" sound (Same as win sound).
	df::Sound *p_sound = df::ResourceManager::getInstance().getSound("win");
	p_sound->play();
}

// When level complete message done, start next level.
LevelComplete::~LevelComplete() {    
	lvl++;
	GameStart *gamestart = new GameStart(lvl);
	gamestart->startLevel(lvl);
}

// Handle event.
// Return 0 if ignored, else 1.
int LevelComplete::eventHandler(const df::Event *p_e) {

	if (p_e->getType() == df::STEP_EVENT) {
		step();
		return 1;
	}

	// If get here, have ignored this event.
	return 0;
}

// Count down to end of message.
void LevelComplete::step() {
	time_to_live--;
	if (time_to_live <= 0) {
		df::WorldManager &world_manager = df::WorldManager::getInstance();
		world_manager.markForDelete(this);
	}
}

// Override default draw so as not to display "value".
void LevelComplete::draw() {
	df::Object::draw();
}
