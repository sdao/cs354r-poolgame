#include "Player.h"

Player::Player(Ogre::SceneManager* mSceneMgr, const std::string& meshName, const std::string& name) : 
	GameObject(mSceneMgr, meshName, name),
	pState(PlayerState::Hit)
{
	
}
