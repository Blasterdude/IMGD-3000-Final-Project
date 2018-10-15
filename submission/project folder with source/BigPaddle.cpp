// BigPaddle.cpp

// Includes
#include "BigPaddle.h"
#include "Position.h"
#include "Projectile.h"
#include "GraphicsManager.h"
#include "ResourceManager.h"
#include "EventCollision.h"
#include "EventKeyboard.h"

// Initial constructor.
BigPaddle::BigPaddle() {
	// Sets object type.
	setType("Big Paddle");

	// Set altitude of Paddle.
	setAltitude(1);

	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	df::GraphicsManager &graphics_manager = df::GraphicsManager::getInstance();
	
	// Link "paddle" sprite to paddle.
	df::Sprite *temp_sprite = resource_manager.getSprite("big paddle");
	setSprite(temp_sprite);

	setPosition(df::Position(graphics_manager.getHorizontal() / 2, graphics_manager.getVertical() - 1));

	// Register for "keyboard" event.
	registerInterest(df::KEYBOARD_EVENT);
}

// Destructor.
BigPaddle::~BigPaddle() {

}

// Event handler.
int BigPaddle::eventHandler(const df::Event *p_e) {
	if (p_e->getType() == df::COLLISION_EVENT) {
		const df::EventCollision *p_collision_event = static_cast <const df::EventCollision *> (p_e);
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

// Moves the paddle based on player input.
void BigPaddle::move(const df::EventKeyboard *p_keyboard_event) {
	df::GraphicsManager &graphics_manager = df::GraphicsManager::getInstance();

	if (p_keyboard_event->getKey() == df::Keyboard::A || p_keyboard_event->getKey() == df::Keyboard::LEFTARROW) {		// Move left.
		if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN)
			if (getPosition().getX() <= 5) {
				setPosition(df::Position(5, graphics_manager.getVertical() - 1));
			}
			else {
				setPosition(df::Position(getPosition().getX() - 1, graphics_manager.getVertical() - 1));
			}
	}

	if (p_keyboard_event->getKey() == df::Keyboard::D || p_keyboard_event->getKey() == df::Keyboard::RIGHTARROW) {		// Move right.
		if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN)
			if (getPosition().getX() >= graphics_manager.getHorizontal() - 6) {
				setPosition(df::Position(graphics_manager.getHorizontal() - 6, graphics_manager.getVertical() - 1));
			}
			else {
				setPosition(df::Position(getPosition().getX() + 1, graphics_manager.getVertical() - 1));
			}
	}
}