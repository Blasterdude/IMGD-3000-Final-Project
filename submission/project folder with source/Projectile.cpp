// Projectile.cpp

// Includes
#include "EventOut.h"
#include "EventCollision.h"
#include "Projectile.h"
#include "Position.h"
#include "Bomb.h"
#include "BombNotification.h"
#include "PaddleNotification.h"
#include "PowerNotification.h"
#include "EventKeyboard.h"
#include "EventStep.h"
#include "EventView.h"
#include "GameOver.h"
#include "GameManager.h"
#include "GraphicsManager.h"
#include "ObjectList.h"
#include "ObjectListIterator.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "BigPaddle.h"
#include "Paddle.h"
#include "Squill.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Timer.h"
#include "WorldManager.h"

// Counter for if ball is launched - 0 = unlaunched, 1 = launched.
static int counter;

// Lives the player has left.
static int lives = 10;

// Initial constructor.
Projectile::Projectile() {

	// Sets object's type.
	setType("Projectile");

	// Sets altitude of projectile.
	setAltitude(4);

	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	df::GraphicsManager &graphics_manager = df::GraphicsManager::getInstance();

	// Link "projectile" sprite to projectile.
	df::Sprite *temp_sprite = resource_manager.getSprite("projectile");
	setSprite(temp_sprite);

	// Sets initial values.
	setXVelocity(0);
	setYVelocity(0);
	power = 1;
	baseSpeed = 0.53;
	direction = 27; // Straight down.
	setPosition(df::Position(graphics_manager.getHorizontal() / 2, graphics_manager.getVertical() - 2));

	// Sets gameover to false.
	gameover = false;

	// Sets counter to 0 -> projectile not launched.
	counter = 0;

	// Makes a generic timer for powerups, setting time to 0 (set to 30 when a powerup is obtained).
	timer = new Timer();
	timer->getTime();

	// Sets HUD for timer to 0 since initially no powerups collected.
	df::WorldManager &world_manager = df::WorldManager::getInstance();
	df::EventView ev("Powerup Time Remaining:", -30, true);
	world_manager.onEvent(&ev);
	timer->setTime(0);
	timer->getTime();

	// Outputs number of lives left to logfile.
	df::LogManager &log_manager = df::LogManager::getInstance();
	log_manager.writeLog("Lives = %d.\n", lives);

	// Register for "step" event.
	registerInterest(df::STEP_EVENT);

	// Register for "keyboard" event.
	registerInterest(df::KEYBOARD_EVENT);
}


// level-based constructor.
Projectile::Projectile(int level) {

	// Sets object's type.
	setType("Projectile");

	// Sets altitude of projectile.
	setAltitude(4);

	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	df::GraphicsManager &graphics_manager = df::GraphicsManager::getInstance();

	// Link "projectile" sprite to projectile.
	df::Sprite *temp_sprite = resource_manager.getSprite("projectile");
	setSprite(temp_sprite);

	// Sets initial values.
	setXVelocity(0);
	setYVelocity(0);
	power = 1;
	direction = 27; // Straight down.
	setPosition(df::Position(graphics_manager.getHorizontal() / 2, graphics_manager.getVertical() - 2));

	lvl = level;

	switch (level){
	case 1:
		baseSpeed = 0.53;
		break;
	case 2:
		baseSpeed = 0.75;
		break;

	case 3:
		baseSpeed = 1;
		break;

	default:
		baseSpeed = 0.53;
		break;
	}


	// Sets gameover to false.
	gameover = false;

	// Sets counter to 0 -> projectile not launched.
	counter = 0;

	// Makes a generic timer for powerups, setting time to 0 (set to 30 when a powerup is obtained).
	timer = new Timer();
	timer->getTime();

	// Sets HUD for timer to 0 since initially no powerups collected.
	df::WorldManager &world_manager = df::WorldManager::getInstance();
	df::EventView ev("Powerup Time Remaining:", -30, true);
	world_manager.onEvent(&ev);
	timer->setTime(0);
	timer->getTime();

	// Outputs number of lives left to logfile.
	df::LogManager &log_manager = df::LogManager::getInstance();
	log_manager.writeLog("Lives = %d.\n", lives);

	// Register for "step" event.
	registerInterest(df::STEP_EVENT);

	// Register for "keyboard" event.
	registerInterest(df::KEYBOARD_EVENT);
}

// Destructor.
Projectile::~Projectile() {
	// Resets time to 0.
	for (int i = 0; i < timer->getTime(); i++) {
		df::WorldManager &world_manager = df::WorldManager::getInstance();
		df::EventView ev("Powerup Time Remaining:", -1, true);
		world_manager.onEvent(&ev);
	}

	timer->setTime(0);
}

// Handles events.
int Projectile::eventHandler(const df::Event *p_e) {
	df::GraphicsManager &graphics_manager = df::GraphicsManager::getInstance();

	if (p_e->getType() == df::STEP_EVENT) {

		// If timer for powerup time left is 0, reset projectile's power back to 1 and/or start using normal paddle again.
		if (timer->getTime() == 0) {
			setPower(1);

			df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
			df::Sprite *p_temp_sprite = resource_manager.getSprite("projectile");
			setSprite(p_temp_sprite);

			df::WorldManager &world_manager = df::WorldManager::getInstance();
			df::GraphicsManager &graphics_manager = df::GraphicsManager::getInstance();

			df::ObjectList updates = world_manager.getAllObjects();
			df::ObjectListIterator oli(&updates);

			while (!oli.isDone()) {
				if (oli.currentObject()->getType() == "Big Paddle") {
					int x = oli.currentObject()->getPosition().getX();
					int y = oli.currentObject()->getPosition().getY();

					Paddle *paddle = new Paddle();
					paddle->setPosition(df::Position(x, y));

					world_manager.markForDelete(oli.currentObject());
				}

				if (oli.currentObject()->getType() == "Bomb Notification" || oli.currentObject()->getType() == "Paddle Notification" || oli.currentObject()->getType() == "Power Notification") {
					world_manager.markForDelete(oli.currentObject());
				}

				oli.next();
			}
		}

		if (counter == 0 && getPosition().getX() >= graphics_manager.getHorizontal() - 4) {
			setPosition(df::Position(graphics_manager.getHorizontal() - 4, graphics_manager.getVertical() - 2));
			return 1;
		}

		if (counter == 0 && getPosition().getX() <= 3) {
			setPosition(df::Position(3, graphics_manager.getVertical() - 2));
			return 1;
		}

		if (getPosition().getX() <= 0) {
			setXVelocity(0);
			setYVelocity(0);
			setPosition(df::Position(1, getPosition().getY()));
			direction = hitRight(direction);
			newVelocity(direction);

			// Play "hit wall" sound.
			p_sound = df::ResourceManager::getInstance().getSound("hit wall");
			p_sound->play();

			return 1;
		}

		if (getPosition().getX() >= graphics_manager.getHorizontal() - 1) {
			setXVelocity(0);
			setYVelocity(0);
			setPosition(df::Position(graphics_manager.getHorizontal() - 2, getPosition().getY()));
			direction = hitLeft(direction);
			newVelocity(direction);

			// Play "hit wall" sound.
			p_sound = df::ResourceManager::getInstance().getSound("hit wall");
			p_sound->play();

			return 1;
		}

		if (getPosition().getY() <= 0) {
			setXVelocity(0);
			setYVelocity(0);
			setPosition(df::Position(getPosition().getX(), 1));
			direction = hitBottom(direction);
			newVelocity(direction);

			// Play "hit wall" sound.
			p_sound = df::ResourceManager::getInstance().getSound("hit wall");
			p_sound->play();

			return 1;
		}

		if (getPosition().getY() >= graphics_manager.getVertical() - 1) {
			df::WorldManager &world_manager = df::WorldManager::getInstance();

			if (lives > 0) {
				// Subtract 1 life.
				df::EventView ev("Lives Left:", -1, true);
				world_manager.onEvent(&ev);

				df::ObjectList updates = world_manager.getAllObjects();

				df::ObjectListIterator oli(&updates);

				while (!oli.isDone()) {
					if (oli.currentObject()->getType() == "Projectile") {
						world_manager.markForDelete(oli.currentObject());
					}

					if (oli.currentObject()->getType() == "Paddle") {
						world_manager.markForDelete(oli.currentObject());
					}

					if (oli.currentObject()->getType() == "Big Paddle") {
						world_manager.markForDelete(oli.currentObject());
					}

					oli.next();
				}

				// Play "out of bounds" sound.
				p_sound = df::ResourceManager::getInstance().getSound("out");
				p_sound->play();

				lives--;
				new Projectile(lvl);
				new Paddle;
			}

			if (lives == 0 && gameover == false) {
				setXVelocity(0);
				setYVelocity(0);
				df::LogManager &log_manager = df::LogManager::getInstance();
				log_manager.writeLog("Game ended.\n");
				gameover = true;
				new GameOver(lvl);
				lives = 10;
				return 1;
			}

			if (gameover == true) {
				return 0;
			}
		}
	}

	if (p_e->getType() == df::COLLISION_EVENT) {
		const df::EventCollision *p_collision_event = static_cast <const df::EventCollision *> (p_e);

		if (p_collision_event->getObject2()->getType() == "Squill") {
			Squill* squill = static_cast <Squill*> (p_collision_event->getObject2());

			// if projectile is to the left of the Squill and within the Squill's height, run hitLeft() and set the projectile's velocity with newVelocity().
			if (getPosition().getX() == squill->getPosition().getX() - 3 && getPosition().getY() <= squill->getPosition().getY() + 1 && getPosition().getY() >= squill->getPosition().getY()) {
				df::LogManager &log_manager = df::LogManager::getInstance();
				log_manager.writeLog("Direction = Left.\n");
				log_manager.writeLog("Projectile's Position = %d, %d.\n", getPosition().getX(), getPosition().getY());
				log_manager.writeLog("Squill's Position = %d, %d.\n", squill->getPosition().getX(), squill->getPosition().getY());
				log_manager.writeLog("Projectile's Velocity = %f, %f.\n", getXVelocity(), getYVelocity());
				direction = hitLeft(direction);
				newVelocity(direction);
				log_manager.writeLog("Projectile's Velocity = %f, %f.\n", getXVelocity(), getYVelocity());
			}

			// if projectile is to the right of the Squill and within the Squill's height, run hitRight() and set the projectile's velocity with newVelocity().
			if (getPosition().getX() == squill->getPosition().getX() + 2 && getPosition().getY() <= squill->getPosition().getY() + 1 && getPosition().getY() >= squill->getPosition().getY()) {
				df::LogManager &log_manager = df::LogManager::getInstance();
				log_manager.writeLog("Direction = Right.\n");
				log_manager.writeLog("Projectile's Position = %d, %d.\n", getPosition().getX(), getPosition().getY());
				log_manager.writeLog("Squill's Position = %d, %d.\n", squill->getPosition().getX(), squill->getPosition().getY());
				log_manager.writeLog("Projectile's Velocity = %f, %f.\n", getXVelocity(), getYVelocity());
				direction = hitRight(direction);
				newVelocity(direction);
				log_manager.writeLog("Projectile's Velocity = %f, %f.\n", getXVelocity(), getYVelocity());
			}

			// if projectile is below the Squill and within the Squill's width, run hitBottom() and set the projectile's velocity with newVelocity().
			if (getPosition().getY() > squill->getPosition().getY() && getPosition().getX() >= squill->getPosition().getX() - 2 && getPosition().getX() <= squill->getPosition().getX() + 1) {
				df::LogManager &log_manager = df::LogManager::getInstance();
				log_manager.writeLog("Direction = Down.\n");
				log_manager.writeLog("Projectile's Position = %d, %d.\n", getPosition().getX(), getPosition().getY());
				log_manager.writeLog("Squill's Position = %d, %d.\n", squill->getPosition().getX(), squill->getPosition().getY());
				log_manager.writeLog("Projectile's Velocity = %f, %f.\n", getXVelocity(), getYVelocity());
				direction = hitBottom(direction);
				newVelocity(direction);
				log_manager.writeLog("Projectile's Velocity = %f, %f.\n", getXVelocity(), getYVelocity());
			}

			// if projectile is above the Squill and within the Squill's width, run hitTop() and set the projectile's velocity with newVelocity().
			if (getPosition().getY() < squill->getPosition().getY() && getPosition().getX() >= squill->getPosition().getX() - 2 && getPosition().getX() <= squill->getPosition().getX() + 1) {
				df::LogManager &log_manager = df::LogManager::getInstance();
				log_manager.writeLog("Direction = Up.\n");
				log_manager.writeLog("Projectile's Position = %d, %d.\n", getPosition().getX(), getPosition().getY());
				log_manager.writeLog("Squill's Position = %d, %d.\n", squill->getPosition().getX(), squill->getPosition().getY());
				log_manager.writeLog("Projectile's Velocity = %f, %f.\n", getXVelocity(), getYVelocity());
				direction = hitTop(direction);
				newVelocity(direction);
				log_manager.writeLog("Projectile's Velocity = %f, %f.\n", getXVelocity(), getYVelocity());
			}

			// Handles the powerups for the 'powerup' Squills.
			if (squill->getSquillType() == "powerup") {
				if (squill->getHitpoints() == 1) {
					// Play "powerup" sound.
					p_sound = df::ResourceManager::getInstance().getSound("powerup");
					p_sound->play();

					int s = rand() % 3;

					if (s == 0) {
						// Gets rid of notification for previous power up.
						df::WorldManager &world_manager = df::WorldManager::getInstance();

						df::ObjectList updates = world_manager.getAllObjects();
						df::ObjectListIterator oli(&updates);

						while (!oli.isDone()) {
							if (oli.currentObject()->getType() == "Bomb Notification" || oli.currentObject()->getType() == "Paddle Notification" || oli.currentObject()->getType() == "Power Notification") {
								world_manager.markForDelete(oli.currentObject());
							}

							oli.next();
						}

						// Resets time to 0.
						for (int i = 0; i < timer->getTime(); i++) {
							df::WorldManager &world_manager = df::WorldManager::getInstance();
							df::EventView ev("Powerup Time Remaining:", -1, true);
							world_manager.onEvent(&ev);
						}

						timer->setTime(0);

						// Sets projectile to "super projectile" sprite.
						df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
						df::Sprite *p_temp_sprite = resource_manager.getSprite("super projectile");
						setSprite(p_temp_sprite);

						// Reset projectile's power to 1.
						setPower(1);

						// Sets HUD for timer to 30.
						df::EventView ev("Powerup Time Remaining:", +30, true);
						world_manager.onEvent(&ev);
						timer->setTime(30);

						setPower(getPower() * 2);
						df::LogManager &log_manager = df::LogManager::getInstance();
						log_manager.writeLog("Projectile's Power = %d.\n", getPower());

						PowerNotification *notification = new PowerNotification();
						notification->setPosition(df::Position(9, graphics_manager.getVertical() - 1));
					}

					if (s == 1) {
						// Gets rid of notification for previous power up.
						df::WorldManager &world_manager = df::WorldManager::getInstance();

						df::ObjectList updates = world_manager.getAllObjects();
						df::ObjectListIterator oli(&updates);

						// Resets time to 0.
						for (int i = 0; i < timer->getTime(); i++) {
							df::WorldManager &world_manager = df::WorldManager::getInstance();
							df::EventView ev("Powerup Time Remaining:", -1, true);
							world_manager.onEvent(&ev);
						}

						timer->setTime(0);

						// Resets projectile to normal sprite.
						df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
						df::Sprite *p_temp_sprite = resource_manager.getSprite("projectile");
						setSprite(p_temp_sprite);

						// Reset projectile's power to 1.
						setPower(1);

						// Sets HUD for timer to 30.
						df::EventView ev("Powerup Time Remaining:", +30, true);
						world_manager.onEvent(&ev);
						timer->setTime(30);

						df::GraphicsManager &graphics_manager = df::GraphicsManager::getInstance();

						while (!oli.isDone()) {
							if (oli.currentObject()->getType() == "Bomb Notification" || oli.currentObject()->getType() == "Paddle Notification" || oli.currentObject()->getType() == "Power Notification") {
								world_manager.markForDelete(oli.currentObject());
							}

							if (oli.currentObject()->getType() == "Paddle") {
								int x = oli.currentObject()->getPosition().getX();
								int y = oli.currentObject()->getPosition().getY();

								BigPaddle *bigpaddle = new BigPaddle();
								bigpaddle->setPosition(df::Position(x, y));

								world_manager.markForDelete(oli.currentObject());
							}

							oli.next();
						}

						PaddleNotification *notification = new PaddleNotification();
						notification->setPosition(df::Position(10, graphics_manager.getVertical() - 1));
					}

					if (s == 2) {
						// If power was higher than 1, reset it to 1.
						setPower(1);

						// Resets projectile to normal sprite.
						df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
						df::Sprite *p_temp_sprite = resource_manager.getSprite("projectile");
						setSprite(p_temp_sprite);

						// Gets rid of notification for previous power up.
						df::WorldManager &world_manager = df::WorldManager::getInstance();

						df::ObjectList updates = world_manager.getAllObjects();
						df::ObjectListIterator oli(&updates);

						// Resets time to 0.
						for (int i = 0; i < timer->getTime(); i++) {
							df::WorldManager &world_manager = df::WorldManager::getInstance();
							df::EventView ev("Powerup Time Remaining:", -1, true);
							world_manager.onEvent(&ev);
						}

						timer->setTime(0);

						// Sets HUD for timer to 1.
						df::EventView ev("Powerup Time Remaining:", +1, true);
						world_manager.onEvent(&ev);
						timer->setTime(1);

						df::GraphicsManager &graphics_manager = df::GraphicsManager::getInstance();

						while (!oli.isDone()) {
							if (oli.currentObject()->getType() == "Bomb Notification" || oli.currentObject()->getType() == "Paddle Notification" || oli.currentObject()->getType() == "Power Notification") {
								world_manager.markForDelete(oli.currentObject());
							}

							if (oli.currentObject()->getType() == "Squill" && oli.currentObject()->getPosition().getX() > (graphics_manager.getHorizontal() / 2) - 32 && oli.currentObject()->getPosition().getX() < (graphics_manager.getHorizontal() / 2) + 32) {
								world_manager.markForDelete(oli.currentObject());
							}

							oli.next();
						}

						Bomb *bomb = new Bomb();
						bomb->setPosition(df::Position(graphics_manager.getHorizontal() / 2, 6));

						BombNotification *notification = new BombNotification();
						notification->setPosition(df::Position(7, graphics_manager.getVertical() - 1));

						// Play "bomb" sound.
						p_sound = df::ResourceManager::getInstance().getSound("bomb");
						p_sound->play();
					}
				}
			}
		}

		if (p_collision_event->getObject2()->getType() == "Paddle") {
			Paddle* paddle = static_cast <Paddle*> (p_collision_event->getObject2());
			// if projectile is above the paddle and within the paddle's width, run hitTop().
			if (getPosition().getY() < paddle->getPosition().getY() && getPosition().getX() <= paddle->getPosition().getX() + 3 && getPosition().getX() >= paddle->getPosition().getX() - 3) {
				df::LogManager &log_manager = df::LogManager::getInstance();
				log_manager.writeLog("Direction = Up.\n");
				log_manager.writeLog("Projectile's Position = %d, %d.\n", getPosition().getX(), getPosition().getY());
				log_manager.writeLog("Paddle's Position = %d, %d.\n", paddle->getPosition().getX(), paddle->getPosition().getY());
				log_manager.writeLog("Projectile's Velocity = %f, %f.\n", getXVelocity(), getYVelocity());
				direction = hitTop(direction);
				newVelocity(direction);
				log_manager.writeLog("Projectile's Velocity = %f, %f.\n", getXVelocity(), getYVelocity());

				// Play "hit paddle" sound.
				p_sound = df::ResourceManager::getInstance().getSound("hit paddle");
				p_sound->play();
			}
		}

		if (p_collision_event->getObject2()->getType() == "Big Paddle") {
			BigPaddle* bigpaddle = static_cast <BigPaddle*> (p_collision_event->getObject2());
			// if projectile is above the paddle and within the paddle's width, run hitTop().
			if (getPosition().getY() < bigpaddle->getPosition().getY() && getPosition().getX() <= bigpaddle->getPosition().getX() + 5 && getPosition().getX() >= bigpaddle->getPosition().getX() - 5) {
				df::LogManager &log_manager = df::LogManager::getInstance();
				log_manager.writeLog("Direction = Up.\n");
				log_manager.writeLog("Projectile's Position = %d, %d.\n", getPosition().getX(), getPosition().getY());
				log_manager.writeLog("Big Paddle's Position = %d, %d.\n", bigpaddle->getPosition().getX(), bigpaddle->getPosition().getY());
				log_manager.writeLog("Projectile's Velocity = %f, %f.\n", getXVelocity(), getYVelocity());
				direction = hitTop(direction);
				newVelocity(direction);
				log_manager.writeLog("Projectile's Velocity = %f, %f.\n", getXVelocity(), getYVelocity());

				// Play "hit paddle" sound.
				p_sound = df::ResourceManager::getInstance().getSound("hit paddle");
				p_sound->play();
			}
		}

		return 1;
	}

	if (p_e->getType() == df::KEYBOARD_EVENT) {
		const df::EventKeyboard *p_keyboard_event = static_cast <const df::EventKeyboard *> (p_e);
		move(p_keyboard_event);
		return 1;
	}

	// If get here, have ignored this event.
	return 0;
}

// Sets the power of the projectile.
void Projectile::setPower(int pow) {
	power = pow;
}

// Gets the power of the projectile.
int Projectile::getPower() const {
	return power;
}

// Starts moving the projectile.
void Projectile::move(const df::EventKeyboard *p_keyboard_event) {
	df::GraphicsManager &graphics_manager = df::GraphicsManager::getInstance();

	if (p_keyboard_event->getKey() == df::Keyboard::A || p_keyboard_event->getKey() == df::Keyboard::LEFTARROW) {		// Move projectile left with paddle if not launched.
		if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN) {
			if (counter == 0) {
				setPosition(df::Position(getPosition().getX() - 1, graphics_manager.getVertical() - 2));
			}
		}
	}

	if (p_keyboard_event->getKey() == df::Keyboard::D || p_keyboard_event->getKey() == df::Keyboard::RIGHTARROW) {		// Move projectile right with paddle if not launched.
		if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN) {
			if (counter == 0) {
				setPosition(df::Position(getPosition().getX() + 1, graphics_manager.getVertical() - 2));
			}
		}
	}

	switch (p_keyboard_event->getKey()) {
	case df::Keyboard::SPACE:				// Launch projectile.
		if (p_keyboard_event->getKeyboardAction() == df::KEY_PRESSED)
			if (counter == 0) {
				int start = rand() % 3;

				// Move right (Y velocity set to go down as the collision with the paddle will negate the value).

				if (start == 0) {
					setXVelocity(0.375);
					setYVelocity(0.375);
					direction = 28;
					counter++;
				}

				// Move up (Y velocity set to go down as the collision with the paddle will negate the value).

				if (start == 1) {
					setXVelocity(0);
					setYVelocity(0.375);
					direction = 27;
					counter++;
				}
				
				// Move left (Y velocity set to go down as the collision with the paddle will negate the value).

				if (start == 2) {
					setXVelocity(-0.375);
					setYVelocity(0.375);
					direction = 26;
					counter++;
				}

				df::LogManager &log_manager = df::LogManager::getInstance();
				log_manager.writeLog("Projectile's Velocity = %f, %f.\n", getXVelocity(), getYVelocity());

			}

			else {
				break;
			}
			break;

	case df::Keyboard::Q:					// Quits game.
		if (p_keyboard_event->getKey() == df::Keyboard::Q) {
			setXVelocity(0);
			setYVelocity(0);
			new GameOver(lvl);
		}
		break;

	default:
		break;
	}
}

int Projectile::hitTop(int given) {

	df::LogManager &log_manager = df::LogManager::getInstance();
	log_manager.writeLog("HIT TOP Initial direction is %d.\n", given);
	int newDir = given;

	int r = 1;

	if ((given >= 18 && given <= 35) || given == 0) {
		if (given >= 19){
			newDir = (36 - given);
		}
		else if (given == 18 || given == 0) {
			return given;
		}

		log_manager.writeLog("HIT TOP new direction is %d.\n", newDir);

		// generate random number between -1 and 1.
		r = rand() % 3 - 1;

		if ((newDir == 0 && r == -1) || (newDir == 35 && r == 1)) {
			return newDir;
		}

		return newDir + r;
	}

	else {
		log_manager.writeLog("HIT TOP IMPOSSIBLE DIRECTION?.\n");

		return given;
	}
}


int Projectile::hitBottom(int given) {

	df::LogManager &log_manager = df::LogManager::getInstance();
	log_manager.writeLog("HIT BOTTOM Initial direction is %d.\n", given);

	int newDir = given;

	int r = 1;

	if (given >= 0 && given <= 18) {
		if (given <= 17 && given >= 1){
			newDir = 36 - given;
		}

		else if (given == 0 || given == 18) {
			return given;
		}

		log_manager.writeLog("HIT BOTTOM new direction is %d.\n", newDir);

		// generate random number between -1 and 1.
		r = rand() % 3 - 1;

		if ((newDir == 0 && r == -1) || (newDir == 35 && r == 1)) {
			return newDir;
		}

		return newDir + r;
	}

	else {
		log_manager.writeLog("HIT BOTTOM IMPOSSIBLE DIRECTION?.\n");
		return given;
	}
}

int Projectile::hitLeft(int given) {

	df::LogManager &log_manager = df::LogManager::getInstance();
	log_manager.writeLog("HIT LEFT Initial direction is %d.\n", given);

	int newDir = given;

	int r = 1;

	if (given >= 27 || given <= 9) {
		if (given == 0) {
			return 19;
		}

		if (given <= 8) {
			newDir = 18 - given;
		}

		else if (given >= 28) {
			newDir = (36 - given) + 18;
		}

		else if (given == 9 || given == 27) {
			return given;
		}

		log_manager.writeLog("HIT LEFT new direction is %d.\n", newDir);

		// generate random number between -1 and 1.
		r = rand() % 3 - 1;

		if ((newDir == 0 && r == -1) || (newDir == 35 && r == 1)){
			return newDir;
		}

		return newDir + r;
	}

	else {
		log_manager.writeLog("HIT LEFT IMPOSSIBLE DIRECTION?.\n");
		return given;
	}
}



int Projectile::hitRight(int given) {

	df::LogManager &log_manager = df::LogManager::getInstance();
	log_manager.writeLog("HIT RIGHT Initial direction is %d.\n", given);

	int newDir = given;
	int r = 1;

	if (given >= 9 && given <= 27) {
		if (given == 18) {
			return 35;
		}

		if (given >= 10 && given <= 18){
			newDir = 18 - given;
		}
		else if (given <= 26) {
			newDir = 36 - (given - 18);
		}
		else if (given == 9 || given == 27) {
			return given;
		}

		log_manager.writeLog("HIT RIGHT new direction is %d.\n", newDir);

		// generate random number between -1 and 1.
		r = rand() % 3 - 1;

		if ((newDir == 0 && r == -1) || (newDir == 35 && r == 1)){
			return newDir;
		}

		return newDir + r;
	}
	else{
		log_manager.writeLog("HIT RIGHT IMPOSSIBLE DIRECTION?.\n");
		return given;
	}
}

void Projectile::newVelocity(int given) {

	double xf = 0;
	double yf = 0;

	switch (given) {
	case 0:
		xf = 1;
		yf = 0;
		break;

	case 1:
		xf = .985;
		yf = .174;
		break;

	case 2:
		xf = .94;
		yf = .342;
		break;

	case 3:
		xf = .866;
		yf = .5;
		break;

	case 4:
		xf = .766;
		yf = .643;
		break;

	case 5:
		xf = .643;
		yf = .766;
		break;

	case 6:
		xf = .5;
		yf = .866;
		break;

	case 7:
		xf = .342;
		yf = .94;
		break;

	case 8:
		xf = .174;
		yf = .985;
		break;

	case 9:
		xf = 0;
		yf = 1;
		break;

	case 10:
		xf = -0.174;
		yf = .985;
		break;

	case 11:
		xf = -0.342;
		yf = .94;
		break;

	case 12:
		xf = -0.5;
		yf = .866;
		break;

	case 13:
		xf = -0.643;
		yf = .766;
		break;

	case 14:
		xf = -0.766;
		yf = .643;
		break;

	case 15:
		xf = -0.866;
		yf = .5;
		break;

	case 16:
		xf = -0.94;
		yf = .342;
		break;

	case 17:
		xf = -0.985;
		yf = .174;
		break;

	case 18:
		xf = -1;
		yf = 0;
		break;

	case 19:
		xf = -0.985;
		yf = -0.174;
		break;

	case 20:
		xf = -0.94;
		yf = -0.342;
		break;

	case 21:
		xf = -0.866;
		yf = -0.5;
		break;

	case 22:
		xf = -0.766;
		yf = -0.643;
		break;

	case 23:
		xf = -0.643;
		yf = -0.766;
		break;

	case 24:
		xf = -0.5;
		yf = -0.866;
		break;

	case 25:
		xf = -0.342;
		yf = -0.94;
		break;

	case 26:
		xf = -0.174;
		yf = -0.985;
		break;

	case 27:
		xf = 0;
		yf = -1;
		break;

	case 28:
		xf = 0.174;
		yf = -0.985;
		break;

	case 29:
		xf = 0.342;
		yf = -0.94;
		break;

	case 30:
		xf = 0.5;
		yf = -0.866;
		break;

	case 31:
		xf = 0.643;
		yf = -0.766;
		break;

	case 32:
		xf = 0.766;
		yf = -0.643;
		break;

	case 33:
		xf = 0.866;
		yf = -0.5;
		break;

	case 34:
		xf = 0.94;
		yf = -0.342;
		break;

	case 35:
		xf = 0.985;
		yf = -0.174;
		break;

	default:
		df::LogManager &log_manager = df::LogManager::getInstance();

		log_manager.writeLog("NEW_VELOCITY IMPOSSIBLE DIRECTION GIVEN.\n", given);
		break;
	}

	setXVelocity((float)(baseSpeed * xf));
	setYVelocity((float)(baseSpeed * -1 * yf));
	df::LogManager &log_manager = df::LogManager::getInstance();

	log_manager.writeLog("NEW_VELOCITY New vs are x = %f, y= %f.\n", getXVelocity(), getYVelocity());
}