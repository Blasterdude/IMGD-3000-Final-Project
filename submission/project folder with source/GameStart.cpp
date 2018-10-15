//
// GameStart.cpp
//

// Engine includes.
#include "Color.h"
#include "EventKeyboard.h"
#include "GameManager.h"
#include "GraphicsManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include <stdlib.h>
#include "WorldManager.h"

// Game includes.
#include "GameStart.h"
#include "Music.h"
#include "Points.h"
#include "Paddle.h"
#include "Projectile.h"
#include "Squill.h"
#include "Level1Notification.h"
#include "Level2Notification.h"
#include "Level3Notification.h"

GameStart::GameStart() {
	setType("GameStart");

	// Link to "message" sprite.
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	df::Sprite *p_temp_sprite = resource_manager.getSprite("gamestart");

	if (!p_temp_sprite) {
		df::LogManager &log_manager = df::LogManager::getInstance();
		log_manager.writeLog("GameStart::GameStart(): Warning! Sprite 'gamestart' not found");
	}

	else {
		setSprite(p_temp_sprite);
		setSpriteSlowdown(15);
	}

	// Put in center of screen.
	setLocation(df::CENTER_CENTER);

	// Register for "keyboard" event.
	registerInterest(df::KEYBOARD_EVENT);

	// Play start music.
	p_music = df::ResourceManager::getInstance().getMusic("main");
	playMusic();
}

GameStart::GameStart(int level) {
	setType("GameStart");

	// Link to "message" sprite.
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	df::Sprite *p_temp_sprite = resource_manager.getSprite("gamestart");

	if (!p_temp_sprite) {
		df::LogManager &log_manager = df::LogManager::getInstance();
		log_manager.writeLog("GameStart::GameStart(): Warning! Sprite 'gamestart' not found");
	}

	else {
		setSprite(p_temp_sprite);
		setSpriteSlowdown(15);
	}

	// Put in center of screen.
	setLocation(df::CENTER_CENTER);

	// Register for "keyboard" event.
	registerInterest(df::KEYBOARD_EVENT);
}

// Play start music.
void GameStart::playMusic() {
	p_music->play();
}

// Handle event.
// Return 0 if ignored, else 1.
int GameStart::eventHandler(const df::Event *p_e) {
	df::GameManager &game_manager = df::GameManager::getInstance();

	if (p_e->getType() == df::KEYBOARD_EVENT) {
		df::EventKeyboard *p_keyboard_event = (df::EventKeyboard *) p_e;
		switch (p_keyboard_event->getKey()) {
		case df::Keyboard::P: 			// play
			start();
			break;
		case df::Keyboard::Q:			// quit
			game_manager.setGameOver();
			break;
		default:
			break;
		}
		return 1;
	}

	// If get here, have ignored this event.
	return 0;
}

void GameStart::start() {

	df::GraphicsManager &graphics_manager = df::GraphicsManager::getInstance();
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	df::WorldManager &world_manager = df::WorldManager::getInstance();

	df::ObjectList updates = world_manager.getAllObjects();
	df::ObjectListIterator oli(&updates);

	// Deletes all objects from previous level.
	while (!oli.isDone()) {
		if (oli.currentObject()->getType() == "Squill" || oli.currentObject()->getType() == "Projectile" || oli.currentObject()->getType() == "Paddle" || oli.currentObject()->getType() == "Big Paddle" || oli.currentObject()->getType() == "Timer" || oli.currentObject()->getType() == "Bomb Notification" || oli.currentObject()->getType() == "Paddle Notification" || oli.currentObject()->getType() == "Power Notification" || oli.currentObject()->getType() == "Level 1 Notification" || oli.currentObject()->getType() == "Level 2 Notification" || oli.currentObject()->getType() == "Level 3 Notification") {
			delete oli.currentObject();
		}

		oli.next();
	}

	p_music = df::ResourceManager::getInstance().getMusic("main");
	p_music->pause();

	// Setup heads-up display.
	new Points;		                     // Points display.

	df::ViewObject *p_vo = new df::ViewObject; // Time left for powerup (initially set to 0 since no powerups activated).
	p_vo->setLocation(df::TOP_LEFT);
	p_vo->setViewString("Powerup Time Remaining:");
	p_vo->setValue(0);
	p_vo->setColor(df::GREEN);

	df::ViewObject *p_vo2 = new df::ViewObject; // Count of lives left.
	p_vo2->setLocation(df::TOP_CENTER);
	p_vo2->setViewString("Lives Left:");
	p_vo2->setValue(10);
	p_vo2->setColor(df::GREEN);

	// Create Paddle.
	new Paddle();

	// Create Projectile.
	new Projectile();

	// Spawn Squills to shoot.
	int x = 2;
	int y = 5;

	while (y >= 5 && y < 7) {
		if (x <= graphics_manager.getHorizontal() - 2) {
			int s = rand() % 5;

			if (s >= 0 && s < 5) {
				Squill *squill = new Squill();
				df::Sprite *temp_sprite = resource_manager.getSprite("basic");
				squill->setSprite(temp_sprite);
				squill->setSquillType("basic");
				squill->setHitpoints(1);
				squill->setSpriteSlowdown(4);
				squill->setPosition(df::Position(x, y));
			}

			if (s >= 5 && s < 8) {
				Squill *squill = new Squill();
				df::Sprite *temp_sprite = resource_manager.getSprite("average");
				squill->setSprite(temp_sprite);
				squill->setSquillType("average");
				squill->setHitpoints(2);
				squill->setSpriteSlowdown(4);
				squill->setPosition(df::Position(x, y));
			}

			if (s >= 8 && s < 9) {
				Squill *squill = new Squill();
				df::Sprite *temp_sprite = resource_manager.getSprite("super");
				squill->setSprite(temp_sprite);
				squill->setSquillType("super");
				squill->setHitpoints(3);
				squill->setSpriteSlowdown(4);
				squill->setPosition(df::Position(x, y));
			}

			if (s == 9) {
				Squill *squill = new Squill();
				df::Sprite *temp_sprite = resource_manager.getSprite("powerup");
				squill->setSprite(temp_sprite);
				squill->setSquillType("powerup");
				squill->setHitpoints(1);
				squill->setSpriteSlowdown(4);
				squill->setPosition(df::Position(x, y));
			}

			x += 4;
		}

		else {
			x = 2;
			y++;
		}
	}

	// Sets up notification for which level is currently being played.
	Level1Notification *notification = new Level1Notification();
	notification->setPosition(df::Position(graphics_manager.getHorizontal() - 5, graphics_manager.getVertical() - 1));

	// Plays level 1 music.
	p_music = df::ResourceManager::getInstance().getMusic("level 1");
	playMusic();

	// When game starts, become inactive.
	setActive(false);
}

void GameStart::startLevel(int level) {

	df::GraphicsManager &graphics_manager = df::GraphicsManager::getInstance();
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	df::WorldManager &world_manager = df::WorldManager::getInstance();

	df::ObjectList updates = world_manager.getAllObjects();
	df::ObjectListIterator oli(&updates);

	// Deletes all objects from previous level.
	while (!oli.isDone()) {
		if (oli.currentObject()->getType() == "Projectile" || oli.currentObject()->getType() == "Paddle" || oli.currentObject()->getType() == "Big Paddle" || oli.currentObject()->getType() == "Timer" || oli.currentObject()->getType() == "Bomb Notification" || oli.currentObject()->getType() == "Paddle Notification" || oli.currentObject()->getType() == "Power Notification" || oli.currentObject()->getType() == "Level 1 Notification" || oli.currentObject()->getType() == "Level 2 Notification" || oli.currentObject()->getType() == "Level 3 Notification") {
			delete oli.currentObject();
		}

		oli.next();
	}

	p_music = df::ResourceManager::getInstance().getMusic("main");
	p_music->stop();

	// Setup heads-up display.

	df::ViewObject *p_vo = new df::ViewObject; // Time left for powerup (initially set to 0 since no powerups activated).
	p_vo->setLocation(df::TOP_LEFT);
	p_vo->setViewString("Powerup Time Remaining:");
	p_vo->setValue(0);
	p_vo->setColor(df::GREEN);

	// Create Paddle.
	new Paddle();

	// Create Projectile.
	new Projectile(level);

	// Spawn Squills to shoot.

	int x, y;
	int mx, my;
	int r;

	switch (level) {
		case 1:
			 x = 2;
			 y = 5;
			 my = 7;
			 mx = graphics_manager.getHorizontal() - 2;
			 r = 5;
			break;
		case 2:
			x = 2;
			y = 5;
			my = 8;
			mx = graphics_manager.getHorizontal() - 2;
			r = 10;
			break;

		case 3:
			x = 2;
			y = 5;
			my = 9;
			mx = graphics_manager.getHorizontal() - 2;
			r = 10;
			break;

		default:
			x = 2;
			y = 5;
			my = 7;
			mx = graphics_manager.getHorizontal() - 2;
			r = 5;
			break;

	}

	while (y >= 5 && y < my) {
		if (x <= mx) {
			int s = rand() % r;

			if (s >= 0 && s < 5) {
				Squill *squill = new Squill();
				df::Sprite *temp_sprite = resource_manager.getSprite("basic");
				squill->setSprite(temp_sprite);
				squill->setSquillType("basic");
				squill->setHitpoints(1);
				squill->setSpriteSlowdown(4);
				squill->setPosition(df::Position(x, y));
			}

			if (s >= 5 && s < 8) {
				Squill *squill = new Squill();
				df::Sprite *temp_sprite = resource_manager.getSprite("average");
				squill->setSprite(temp_sprite);
				squill->setSquillType("average");
				squill->setHitpoints(2);
				squill->setSpriteSlowdown(4);
				squill->setPosition(df::Position(x, y));
			}

			if (s >= 8 && s < 9) {
				Squill *squill = new Squill();
				df::Sprite *temp_sprite = resource_manager.getSprite("super");
				squill->setSprite(temp_sprite);
				squill->setSquillType("super");
				squill->setHitpoints(3);
				squill->setSpriteSlowdown(4);
				squill->setPosition(df::Position(x, y));
			}

			if (s == 9) {
				Squill *squill = new Squill();
				df::Sprite *temp_sprite = resource_manager.getSprite("powerup");
				squill->setSprite(temp_sprite);
				squill->setSquillType("powerup");
				squill->setHitpoints(1);
				squill->setSpriteSlowdown(4);
				squill->setPosition(df::Position(x, y));
			}

			x += 4;
		}

		else {
			x = 2;
			y++;
		}
	}
	
	// Sets up notification for which level is currently being played.
	Level1Notification *notification1 = new Level1Notification();
	Level2Notification *notification2 = new Level2Notification();
	Level3Notification *notification3 = new Level3Notification();

	switch (level) {
	case 1:
		notification1->setPosition(df::Position(graphics_manager.getHorizontal() - 5, graphics_manager.getVertical() - 1));
		break;
	case 2:
		notification2->setPosition(df::Position(graphics_manager.getHorizontal() - 5, graphics_manager.getVertical() - 1));
		break;
	case 3:
		notification3->setPosition(df::Position(graphics_manager.getHorizontal() - 5, graphics_manager.getVertical() - 1));
		break;
	default:
		notification1->setPosition(df::Position(graphics_manager.getHorizontal() - 5, graphics_manager.getVertical() - 1));
		break;
	}

	switch (level) {
	case 1:
		p_music = df::ResourceManager::getInstance().getMusic("level 1");
		break;

	case 2:
		p_music = df::ResourceManager::getInstance().getMusic("level 1");
		break;

	case 3:
		p_music = df::ResourceManager::getInstance().getMusic("level 2");
		break;

	case 4:
		p_music = df::ResourceManager::getInstance().getMusic("level 3");
		break;

	default:
		p_music = df::ResourceManager::getInstance().getMusic("level 1");
		break;
	}

	// Pause start music.
	p_music->pause();

	switch (level) {
		case 1:
			p_music = df::ResourceManager::getInstance().getMusic("level 1");
			p_music->play();
			break;

		case 2:
			p_music = df::ResourceManager::getInstance().getMusic("level 2");
			p_music->play();
			break;

		case 3:
			p_music = df::ResourceManager::getInstance().getMusic("level 3");
			p_music->play();
			break;

		default:
			break;

	}

	

	// When game starts, become inactive.
	setActive(false);
}

// Override default draw so as not to display "value".
void GameStart::draw() {
	df::Object::draw();
}
