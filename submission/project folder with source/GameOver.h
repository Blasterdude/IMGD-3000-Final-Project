// GameOver.h
 
#ifndef __GAME_OVER_H__
#define __GAME_OVER_H__

// Includes
#include "ViewObject.h"
#include "Music.h"
#include "Sound.h"

class GameOver : public df::ViewObject {

private:
	int time_to_live;
	df::Music *p_music;
	df::Sound *p_sound;
	void step();

public:
	GameOver();
	GameOver(int level);
	~GameOver();
	int eventHandler(const df::Event *p_e);
	void draw();
};

#endif