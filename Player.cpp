#include "Player.h"

Player::Player(Ogre::SceneManager* mSceneMgr, Ogre::Camera* mCamera, const std::string& meshName, const std::string& name) : 
	GameObject(mSceneMgr, meshName, name),
	pState(PlayerState::Hit)
{
	mCamera->setPosition(Ogre::Vector3(0, 0, -80));
	mCamera->rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(Ogre::Degree(180)));
	node->createChildSceneNode("ChildCamera")->attachObject(mCamera);
	inputKeys = new bool[4];
	for(int i = 0 ; i < 4 ; i++){
		inputKeys[i] =  false;
	}
}

void Player::update(){
	Ogre::Vector3 moveDirection = Ogre::Vector3::ZERO;
	std::cout<<"orientation: " << node->getOrientation().x << " "
				   << node->getOrientation().y << " "
				   << node->getOrientation().z << std::endl;
	//W
	if(inputKeys[0]){
		moveDirection += Ogre::Vector3::UNIT_Z;
	}
	//A
	if(inputKeys[1]){
		moveDirection += Ogre::Vector3::UNIT_X;
	}
	//S
	if(inputKeys[2]){
		moveDirection += Ogre::Vector3::NEGATIVE_UNIT_Z;
	}
	//D
	if(inputKeys[3]){
		moveDirection += Ogre::Vector3::NEGATIVE_UNIT_X;
	}

	translate(moveDirection, Ogre::Node::TransformSpace::TS_LOCAL);
}

void Player::getKeyPress(const OIS::KeyEvent& arg){
	if(arg.key == OIS::KC_W){
		inputKeys[0] = true;
	}
	else if(arg.key == OIS::KC_A){
		inputKeys[1] = true;
	}
	else if(arg.key == OIS::KC_S){
		inputKeys[2] = true;
	}
	else if(arg.key == OIS::KC_D){
		inputKeys[3] = true;
	}
}

void Player::getKeyRelease(const OIS::KeyEvent& arg){
	if(arg.key == OIS::KC_W){
		inputKeys[0] = false;
	}
	else if(arg.key == OIS::KC_A){
		inputKeys[1] = false;
	}
	else if(arg.key == OIS::KC_S){
		inputKeys[2] = false;
	}
	else if(arg.key == OIS::KC_D){
		inputKeys[3] = false;
	}
}

void Player::getMouseEvent(const OIS::MouseEvent& arg){
	//yaw(Ogre::Degree(arg.state.X.rel * 0.15f));
	Ogre::Quaternion rot = node->getOrientation();
	if(rot.x < 0.5f || rot.x > -0.5f){
		rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(Ogre::Degree(-arg.state.X.rel * 0.15f)),
		   Ogre::Node::TransformSpace::TS_WORLD);
	}
	//rotate(Ogre::Vector3::UNIT_X, Ogre::Radian(arg.state.Y.rel * 0.15f));
	pitch(Ogre::Degree(arg.state.Y.rel * 0.5f));
	
	//rot.z = 0.0f;
	//node->setOrientation(rot);
}
