#ifndef _player_h_
#define _player_h_

#include "GameObject.h"
#include "GameEnums.h"
#include <OISKeyboard.h>
#include <OISMouse.h>
//#include <OgreSceneManager.h>

class Player : public GameObject {
	protected:
		PlayerState pState;
		bool* inputKeys;

	public:
		Player(Ogre::SceneManager*, Ogre::Camera*, const std::string&, const std::string&);
		void update();
		void getKeyPress(const OIS::KeyEvent&);
		void getKeyRelease(const OIS::KeyEvent&);
		void getMouseEvent(const OIS::MouseEvent&);
};

#endif
