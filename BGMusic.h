#pragma once

#include "Component.h"
#include "GameObject.h"
// #include "Sound.h"
#include <SDL/SDL_mixer.h>
#include <SDL/SDL.h>

class BGMusic : public Component {
public:  
	BGMusic();
	virtual ~BGMusic();
	virtual void update(const UpdateInfo& info) override;
	bool loadMedia();
	void closeMedia();
	void playOrPause();

protected:	
	int x = 0;

private:
	//Loading success flag
	bool success = true;

	//The music that will be played
	Mix_Music *bgmusic = NULL;
};
