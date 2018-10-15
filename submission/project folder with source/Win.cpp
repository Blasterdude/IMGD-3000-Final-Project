// Win.cpp

// Engine includes.
#include "EventStep.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

// Game includes.
#include "Win.h"
#include "GameStart.h"

Win::Win() {

	// Sets object type.
	setType("Win");

	// Link to "win" sprite.
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	df::Sprite *p_temp_sprite = resource_manager.getSprite("win");
	if (!p_temp_sprite) {
		df::LogManager &log_manager = df::LogManager::getInstance();
		log_manager.writeLog("Win::Win(): Warning! Sprite 'win' not found");
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

	// Stop "level 3" music.
	p_music = df::ResourceManager::getInstance().getMusic("level 3");
	p_music->stop();

	// Play "win" sound.
	p_sound = df::ResourceManager::getInstance().getSound("win");
	p_sound->play();
}

// When done, win so reset things for GameStart.
Win::~Win() {
	df::WorldManager &world_manager = df::WorldManager::getInstance();
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();

	// Remove Squills and ViewObjects, re-activate GameStart.
	df::ObjectList object_list = world_manager.getAllObjects(true);
	df::ObjectListIterator i(&object_list);
	for (i.first(); !i.isDone(); i.next()) {
		df::Object *p_o = i.currentObject();

		if (p_o->getType() == "ViewObject") {
			world_manager.markForDelete(p_o);
		}

		if (p_o->getType() == "GameStart") {
			this->setActive(false);
			p_o->setActive(true);
			p_sound->stop();
			p_music = df::ResourceManager::getInstance().getMusic("main");
			p_music->play();
			p_music = df::ResourceManager::getInstance().getMusic("level 1");
			p_music->stop();
			p_music = df::ResourceManager::getInstance().getMusic("level 2");
			p_music->stop();
			p_music = df::ResourceManager::getInstance().getMusic("level 3");
			p_music->stop();
		}
	}
}

// Handle event.
// Return 0 if ignored, else 1.
int Win::eventHandler(const df::Event *p_e) {

	if (p_e->getType() == df::STEP_EVENT) {
		step();
		return 1;
	}

	// If get here, have ignored this event.
	return 0;
}

// Count down to end of message.
void Win::step() {
	time_to_live--;
	if (time_to_live <= 0) {
		df::WorldManager &world_manager = df::WorldManager::getInstance();
		world_manager.markForDelete(this);
	}
}

// Override default draw so as not to display "value".
void Win::draw() {
	df::Object::draw();
}
