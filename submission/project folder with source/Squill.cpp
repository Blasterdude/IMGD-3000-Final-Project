// Squill.cpp

// Includes 
#include "Squill.h"
#include "EventView.h"
#include "GameManager.h"
#include "GameOver.h"
#include "GraphicsManager.h"
#include "LogManager.h"
#include "LevelComplete.h"
#include "Points.h"
#include "Projectile.h"
#include "ResourceManager.h"
#include "SquillDeath.h"
#include "GameStart.h"
#include "WorldManager.h"
#include "Win.h"

int Squill::squill_count = 0;
int Squill::level = 1;

// Initial constructor.
Squill::Squill() {
	// Sets object's type.
	setType("Squill");

	// Sets altitude of Squill.
	setAltitude(2);

	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();

	// Links "squill" sprite to Squill.
	df::Sprite *temp_sprite = resource_manager.getSprite("basic");
	setSprite(temp_sprite);

	// Sets initial values.
	type = "basic";
	hitpoints = 1;
	setPosition(df::Position(0, 10));

	// Sets value of number of squill on screen.
	squill_count++;
	df::LogManager &log_manager = df::LogManager::getInstance();
	log_manager.writeLog("Squill count = %d.\n", getSquillCount());

	levelcomplete = false;
	win = false;
	gameover = true;
}

// Destructor.
Squill::~Squill() {
	// Send "view" event with points to interested ViewObjects.
	if (getSquillType() == "basic") {
		// Add 10 points.
		df::EventView ev(POINTS_STRING, 10, true);
		df::WorldManager &world_manager = df::WorldManager::getInstance();
		world_manager.onEvent(&ev);
	}

	if (getSquillType() == "average") {
		// Add 10 points.
		df::EventView ev(POINTS_STRING, 20, true);
		df::WorldManager &world_manager = df::WorldManager::getInstance();
		world_manager.onEvent(&ev);
	}

	if (getSquillType() == "super") {
		// Add 10 points.
		df::EventView ev(POINTS_STRING, 30, true);
		df::WorldManager &world_manager = df::WorldManager::getInstance();
		world_manager.onEvent(&ev);
	}

	if (getSquillType() == "powerup") {
		// Add 10 points.
		df::EventView ev(POINTS_STRING, 50, true);
		df::WorldManager &world_manager = df::WorldManager::getInstance();
		world_manager.onEvent(&ev);
	}

	// Decrease number of Squills left.
	squill_count--;
	df::LogManager &log_manager = df::LogManager::getInstance();
	log_manager.writeLog("Squill count = %d.\n", getSquillCount());
	
	// Set "squill death" animation.
	SquillDeath *death = new SquillDeath();
	death->setPosition(df::Position(getPosition().getX(), getPosition().getY()));

	// Play "squill death" sound.
	p_sound = df::ResourceManager::getInstance().getSound("death");
	p_sound->play();

	// Set levelcomplete to true.
	df::GraphicsManager &graphics_manager = df::GraphicsManager::getInstance();
	if (gameover == false && squill_count == 0 && level < 3) {
		levelcomplete = true;
	}

	// Set win to true.
	if (gameover == false && squill_count == 0 && level == 3) {
		levelcomplete = false;
		win = true;
	}

	// Display level complete message and increase level by 1.
	if (levelcomplete == true) {
		df::LogManager &log_manager = df::LogManager::getInstance();
		log_manager.writeLog("Level complete.\n");
		new LevelComplete(level);
		level++;
	}

	// Display win message.
	if (win == true) {
		df::LogManager &log_manager = df::LogManager::getInstance();
		log_manager.writeLog("You won!\n");
		new Win();
		level = 1;
	}
}

// Handles events.
int Squill::eventHandler(const df::Event *p_e) {
	if (p_e->getType() == df::COLLISION_EVENT) {
		const df::EventCollision *p_collision_event = static_cast <const df::EventCollision *> (p_e);
		if (p_collision_event->getObject1()->getType() == "Projectile") {
			// Play "hit enemy" sound.
			p_sound = df::ResourceManager::getInstance().getSound("hit enemy");
			p_sound->play();

			Projectile* projectile = static_cast <Projectile*> (p_collision_event->getObject1());
			hit(p_collision_event, projectile->getPower());

			df::GraphicsManager &graphics_manager = df::GraphicsManager::getInstance();
			if (projectile->getPosition().getY() < graphics_manager.getHorizontal() - 1 && squill_count == 1 && (getSquillType() == "basic" || getSquillType() == "average" || getSquillType() == "super" || getSquillType() == "powerup")) {
				gameover = false;
			}
		}

		return 1;
	}

	// If get here, have ignored this event.
	return 0;
}

// Sets the type of Squill.
void Squill::setSquillType(std::string squill_type) {
	type = squill_type;
}

// Gets the type of Squill.
std::string Squill::getSquillType() const {
	return type;
}

// Sets the hitpoints of the Squill.
void Squill::setHitpoints(int hp) {
	hitpoints = hp;
}

// Gets the hitpoints of the Squill.
int Squill::getHitpoints() const {
	return hitpoints;
}

// Sets the value of the number of squill on screen.
void Squill::setSquillCount(int count) {
	squill_count = count;
}

// Gets the value of the number of squill on screen.
int Squill::getSquillCount() const {
	return squill_count;
}

// If a Squill is hit by the projectile, and it has 1 hitpoint, mark it for delete. Otherwise, decrease its hitpoints by a value equal to the projectile's power.
void Squill::hit(const df::EventCollision *p_collision_event, int power) {
	df::WorldManager &world_manager = df::WorldManager::getInstance();
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();

	if (getHitpoints() == 1) {

		if (p_collision_event->getObject1()->getType() == "Projectile") {
			world_manager.markForDelete(p_collision_event->getObject2());
		}

		else {
			return;
		}
	}

	if (getHitpoints() > 1) {

		if (p_collision_event->getObject2()->getType() == "Squill") {
			hitpoints -= power;
			if (hitpoints == 1) {
				df::Sprite *temp_sprite = resource_manager.getSprite("basic");
				setSprite(temp_sprite);
			}

			if (hitpoints == 2) {
				df::Sprite *temp_sprite = resource_manager.getSprite("average");
				setSprite(temp_sprite);
			}

			if (hitpoints <= 0) {
				world_manager.markForDelete(p_collision_event->getObject2());
			}
		}

		else {
			return;
		}
	}

	else {
		return;
	}
}