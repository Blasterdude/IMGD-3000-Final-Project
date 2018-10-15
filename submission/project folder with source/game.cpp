// game.cpp

// Engine includes.
#include "GameManager.h"
#include "LogManager.h"
#include "Pause.h"
#include "ResourceManager.h"

// Game includes.
#include "GameStart.h"

// Function prototypes.
void loadResources(void);
 
int main(int argc, char *argv[]) {
	df::LogManager &log_manager = df::LogManager::getInstance();

	// Start up game manager.
	df::GameManager &game_manager = df::GameManager::getInstance();
	if (game_manager.startUp()) {
		log_manager.writeLog("Error starting game manager!");
		game_manager.shutDown();
		return 0;
	}

	// Set flush of logfile during development (when done, make false).
	log_manager.setFlush(true);

	// Load game resources.
	loadResources();
  
	// Create GameStart object.
	new GameStart();

	// Enable player to pause game.
	new df::Pause(df::Keyboard::F10);

	// Sets log level to 1.
	log_manager.setLogLevel(1);

	// Run game (this blocks until game loop is over).
	game_manager.run();
  
	// Shut everything down.
	game_manager.shutDown();
}

// Load resources (sprites, sound effects, music).
void loadResources(void) {
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	resource_manager.loadSprite("sprites/gamestart-spr.txt", "gamestart");
	resource_manager.loadSprite("sprites/gameover-spr.txt", "gameover");
	resource_manager.loadSprite("sprites/level-complete-spr.txt", "level complete");
	resource_manager.loadSprite("sprites/win-spr.txt", "win");
	resource_manager.loadSprite("sprites/paddle-spr.txt", "paddle");
	resource_manager.loadSprite("sprites/big-paddle-spr.txt", "big paddle");
	resource_manager.loadSprite("sprites/projectile-spr.txt", "projectile");
	resource_manager.loadSprite("sprites/super-projectile-spr.txt", "super projectile");
	resource_manager.loadSprite("sprites/basic-squill-spr.txt", "basic");
	resource_manager.loadSprite("sprites/average-squill-spr.txt", "average");
	resource_manager.loadSprite("sprites/super-squill-spr.txt", "super");
	resource_manager.loadSprite("sprites/powerup-squill-spr.txt", "powerup");
	resource_manager.loadSprite("sprites/bomb-spr.txt", "bomb");
	resource_manager.loadSprite("sprites/squill-death-spr.txt", "death");
	resource_manager.loadSprite("sprites/bomb-notification-spr.txt", "bomb notification");
	resource_manager.loadSprite("sprites/paddle-notification-spr.txt", "paddle notification");
	resource_manager.loadSprite("sprites/power-notification-spr.txt", "power notification");
	resource_manager.loadSprite("sprites/level-1-spr.txt", "level 1");
	resource_manager.loadSprite("sprites/level-2-spr.txt", "level 2");
	resource_manager.loadSprite("sprites/level-3-spr.txt", "level 3");
	resource_manager.loadMusic("sounds/main.wav", "main");
	resource_manager.loadMusic("sounds/lvl1.wav", "level 1");
	resource_manager.loadMusic("sounds/lvl2.wav", "level 2");
	resource_manager.loadMusic("sounds/lvl3.wav", "level 3");
	resource_manager.loadSound("sounds/gameover.wav", "game over");
	resource_manager.loadSound("sounds/bomb.wav", "bomb");
	resource_manager.loadSound("sounds/death.wav", "death");
	resource_manager.loadSound("sounds/hit-enemy.wav", "hit enemy");
	resource_manager.loadSound("sounds/hitpad.wav", "hit paddle");
	resource_manager.loadSound("sounds/hitwall.wav", "hit wall");
	resource_manager.loadSound("sounds/out.wav", "out");
	resource_manager.loadSound("sounds/powerup.wav", "powerup");
	resource_manager.loadSound("sounds/victory.wav", "win");
}
