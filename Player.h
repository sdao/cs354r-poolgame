#ifndef _player_h_
#define _player_h_

#include "GameObject.h"
#include "GameEnums.h"

//#include <OgreSceneManager.h>

class Player : GameObject {
	protected:
		PlayerState pState;	

	public:
		Player(Ogre::SceneManager*, const std::string&, const std::string&);
		//void getKeyPress();
		//void getKeyRelease();
};

#endif
