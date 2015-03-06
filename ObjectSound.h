#pragma once

#include "Component.h"
#include "GameObject.h"
#include <SDL/SDL_mixer.h>
#include <SDL/SDL.h>

class ObjectSound : public Component {
public:  
	ObjectSound(std::weak_ptr<GameObject> go);
	virtual ~ObjectSound();
	virtual void update(const UpdateInfo& info) override;
	virtual void didCollide(PhysicsCollider& collider) override;
	bool loadMedia();
	void collision(int val);
	void closeMedia();

protected:	
	int x = 0;

private:
	//Loading success flag
	bool success = true;

	//The sound effects that will be used
	Mix_Chunk *bounce = NULL;
	Mix_Chunk *scratch = NULL;
	Mix_Chunk *score = NULL;
};
