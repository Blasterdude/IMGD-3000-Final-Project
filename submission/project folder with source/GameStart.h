// GameStart.h

#ifndef __GAME_START_H__
#define __GAME_START_H__

// Includes
#include "Music.h"
#include "ViewObject.h"

class GameStart : public df::ViewObject {
private:
	df::Music *p_music;
	void start();

public:
	GameStart();
	GameStart(int level);
	void startLevel(int level);
	int eventHandler(const df::Event *p_e);
	void draw();
	void playMusic();
};

#endif
