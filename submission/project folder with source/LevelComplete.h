// LevelComplete.h

#ifndef __LEVEL_COMPLETE_H__
#define __LEVEL_COMPLETE_H__

// Includes
#include "ViewObject.h"
#include "Music.h"
#include "Sound.h"

class LevelComplete : public df::ViewObject {

private:
	int time_to_live;
	df::Music *p_music;
	df::Sound *p_sound;
	void step();
	int lvl;

public:
	LevelComplete();
	LevelComplete(int level);
	~LevelComplete();
	int eventHandler(const df::Event *p_e);
	void draw();
};

#endif