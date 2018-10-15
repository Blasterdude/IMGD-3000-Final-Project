// Win.h

#ifndef __WIN_H__
#define __WIN_H__

// Includes
#include "ViewObject.h"
#include "Music.h"
#include "Sound.h"

class Win : public df::ViewObject {

private:
	int time_to_live;
	df::Music *p_music;
	df::Sound *p_sound;
	void step();

public:
	Win();
	~Win();
	int eventHandler(const df::Event *p_e);
	void draw();
};

#endif