#ifndef _gameinfo_h_
#define _gameinfo_h_

#include <OgreSceneManager.h>
#include <OgreVector3.h>
#include <vector>
#include "Physics.h"
#include "PhysicsBoxCollider.h"
#include "Ball.h"

struct GameInfo{

	int scoreP1;
	int scoreP2;
	Ogre::Vector3 dimensions;
	std::vector<Ogre::Vector3> ballPositions;
};

GameInfo SetupField(float length, float width, float height, Ogre::SceneManager* mSceneMgr, Physics& physics, std::vector<std::shared_ptr<GameObject> >& sceneObjects){
	GameInfo gameinfo ={0, 0, Ogre::Vector3(length, height, width)};
	gameinfo.ballPositions = std::vector<Ogre::Vector3>();
	for(int i = 0; i < 6; i++){
        Ogre::Entity* wall = mSceneMgr->createEntity("wallEntity" + i , "wallMesh");
        wall->setMaterialName("Examples/GrassFloor");
        wall->setCastShadows(false);

        const std::string wallEntityName = "wallEntity";
        auto go = std::make_shared<GameObject>(mSceneMgr, wall,
            wallEntityName + Ogre::StringConverter::toString(i));

        std::shared_ptr<PhysicsBoxCollider> box;

        switch(i){
            case 0://bottom
                go->translate(0, -height/2, 0);
				go->scale(Ogre::Vector3(width/100.0f, 0, length/100.0f));
                box = std::make_shared<PhysicsBoxCollider>(
                    go,
                    physics,
                    Ogre::Vector3(length, 1.0f, width),
                    0.0f
                );

                go->addComponent(box);
                break;
            case 1://top
                go->translate(0, height/2, 0);
				go->scale(Ogre::Vector3(width/100.0f, 0, length/100.0f));
                go->rotate(Ogre::Vector3::UNIT_Z, Ogre::Radian(Ogre::Degree(180)));

                box = std::make_shared<PhysicsBoxCollider>(
                    go,
                    physics,
                    Ogre::Vector3(length, 1.0f, width),
                    0.0f
                );

                go->addComponent(box);
                break;
            case 2://left
                go->translate(length/2, 0, 0);
				go->scale(Ogre::Vector3(width/100.0f, 0, length/100.0f));
                go->rotate(Ogre::Vector3::UNIT_Z, Ogre::Radian(Ogre::Degree(90)));

                box = std::make_shared<PhysicsBoxCollider>(
                    go,
                    physics,
                    Ogre::Vector3(1.0f, height, width),
                    0.0f
                );

                go->addComponent(box);
                break;
            case 3:
                go->translate(-length/2, 0, 0);
				go->scale(Ogre::Vector3(width/100.0f, 0, length/100.0f));
                go->rotate(Ogre::Vector3::UNIT_Z, Ogre::Radian(Ogre::Degree(270)));

                box = std::make_shared<PhysicsBoxCollider>(
                    go,
                    physics,
                    Ogre::Vector3(1.0f, height, width),
                    0.0f
                );

                go->addComponent(box);
                break;
            case 4://Right
                go->translate(0, 0, -width/2);
				go->scale(Ogre::Vector3(width/100.0f, 0, length/100.0f));
                go->rotate(Ogre::Vector3::UNIT_X, Ogre::Radian(Ogre::Degree(90)));

                box = std::make_shared<PhysicsBoxCollider>(
                    go,
                    physics,
                    Ogre::Vector3(length, height, 1.0f),
                    0.0f
                );

                go->addComponent(box);
                break;
            case 5:
                go->translate(0, 0, width/2);
				go->scale(Ogre::Vector3(width/100.0f, 0, length/100.0f));
                go->rotate(Ogre::Vector3::UNIT_X, Ogre::Radian(Ogre::Degree(270)));

                box = std::make_shared<PhysicsBoxCollider>(
                    go,
                    physics,
                    Ogre::Vector3(length, height, 1.0f),
                    0.0f
                );

                go->addComponent(box);
                break;
            default:
                break;
        }

        sceneObjects.push_back(go);
    }

	return gameinfo;
}

void setPositions(std::shared_ptr<GameInfo>& gameinfo, const std::vector<std::shared_ptr<GameObject> >& sceneObjects){
	int startscore = gameinfo.get()->ballPositions.size();
	gameinfo.get()->ballPositions.clear();
	for( auto go : sceneObjects ){
		if(go.get()->getTag() == 0x2){
			gameinfo.get()->ballPositions.push_back(go.get()->getWorldPosition());
		}
	}
	gameinfo.get()->scoreP1 += startscore - gameinfo.get()->ballPositions.size();
}

//void getPositions

#endif
