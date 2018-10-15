//
// GameOver.cpp
//

// Engine includes.
#include "EventStep.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

// Game includes.
#include "GameOver.h"
#include "GameStart.h"

GameOver::GameOver() {

	// Sets object type.
	setType("GameOver");

	// Link to "message" sprite.
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	df::Sprite *p_temp_sprite = resource_manager.getSprite("gameover");
	if (!p_temp_sprite) {
		df::LogManager &log_manager = df::LogManager::getInstance();
		log_manager.writeLog("GameOver::GameOver(): Warning! Sprite 'gameover' not found");
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

	// Remove Saucers and ViewObjects, re-activate GameStart.
	df::WorldManager &world_manager = df::WorldManager::getInstance();
	
	df::ObjectList object_list = world_manager.getAllObjects(true);
	df::ObjectListIterator i(&object_list);
	for (i.first(); !i.isDone(); i.next()) {
		df::Object *p_o = i.currentObject();

		if (p_o->getType() == "Squill" || p_o->getType() == "Projectile" || p_o->getType() == "Paddle" || p_o->getType() == "Big Paddle" || p_o->getType() == "Timer" || p_o->getType() == "Level 1 Notification" || p_o->getType() == "Level 2 Notification" || p_o->getType() == "Level 3 Notification") {
			world_manager.markForDelete(p_o);
		}
	}

	// Play "game over" sound.
	p_sound = df::ResourceManager::getInstance().getSound("game over");
	p_sound->play();
}

GameOver::GameOver(int level) {

	// Sets object type.
	setType("GameOver");

	// Link to "message" sprite.
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	df::Sprite *p_temp_sprite = resource_manager.getSprite("gameover");
	if (!p_temp_sprite) {
		df::LogManager &log_manager = df::LogManager::getInstance();
		log_manager.writeLog("GameOver::GameOver(): Warning! Sprite 'gameover' not found");
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
	
	// Remove Saucers and ViewObjects, re-activate GameStart.
	df::WorldManager &world_manager = df::WorldManager::getInstance();
	
	df::ObjectList object_list = world_manager.getAllObjects(true);
	df::ObjectListIterator i(&object_list);
	for (i.first(); !i.isDone(); i.next()) {
		df::Object *p_o = i.currentObject();

		if (p_o->getType() == "Squill" || p_o->getType() == "Projectile" || p_o->getType() == "Paddle" || p_o->getType() == "Timer" || p_o->getType() == "Level 1 Notification" || p_o->getType() == "Level 2 Notification" || p_o->getType() == "Level 3 Notification") {
			world_manager.markForDelete(p_o);
		}
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

	// Play "game over" sound.
	p_sound = df::ResourceManager::getInstance().getSound("game over");
	p_sound->play();
}

// When done, game over so reset things for GameStart.
GameOver::~GameOver() {
	df::WorldManager &world_manager = df::WorldManager::getInstance();

	// Remove Saucers and ViewObjects, re-activate GameStart.
	df::ObjectList object_list = world_manager.getAllObjects(true);
	df::ObjectListIterator i(&object_list);
	for (i.first(); !i.isDone(); i.next()) {
		df::Object *p_o = i.currentObject();
		
		if (p_o->getType() == "ViewObject") {
			world_manager.markForDelete(p_o);
		}

		if (p_o -> getType() == "GameStart") {
			this->setActive(false);
			p_o -> setActive(true);
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
int GameOver::eventHandler(const df::Event *p_e) {

	if (p_e->getType() == df::STEP_EVENT) {
		step();
		return 1;
	}

	// If get here, have ignored this event.
	return 0;
}

// Count down to end of message.
void GameOver::step() {
	time_to_live--;
	if (time_to_live <= 0) { 
		df::WorldManager &world_manager = df::WorldManager::getInstance();
		world_manager.markForDelete(this);
	}
}

// Override default draw so as not to display "value".
void GameOver::draw() {
	df::Object::draw();
}
