#ifndef _gameinfo_h_
#define _gameinfo_h_

#include <OgreSceneManager.h>
#include <OgreVector3.h>
#include <vector>
#include "GameObject.h"
//#include "Physics.h"
//#include "PhysicsBoxCollider.h"
//#include "PhysicsSphereCollider.h"
//#include "GoalController.h"
//#include "ObjectSound.h"
//#include "Ball.h"

struct GameInfo{

	int scoreP1;
	int scoreP2;
	int playerturn;
	Ogre::Vector3 dimensions;
	std::vector<Ogre::Vector3> ballPositions;
};

//GameInfo SetupField(float, float, float, Ogre::SceneManager*, Physics&, std::vector<std::shared_ptr<GameObject> >&);
bool setPositions(std::shared_ptr<GameInfo>&, const std::vector<std::shared_ptr<GameObject> >&);
#endif
