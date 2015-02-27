#pragma once

#include "Component.h"
#include "GameObject.h"
#include <SDL/SDL_mixer.h>
#include <SDL/SDL.h>

class ObjectSound : public Component {
public:  
	ObjectSound();
	virtual ~ObjectSound();
	virtual void update(const UpdateInfo& info) override;
	bool loadMedia();
	void collision();
	void closeMedia();

protected:	
	int x = 0;

private:
	//Loading success flag
	bool success = true;

	//The sound effects that will be used
	Mix_Chunk *bounce = NULL;
};
