#include "GameInfo.h"

/*
GameInfo SetupField(float length, float width, float height, Ogre::SceneManager* mSceneMgr, Physics& physics, std::vector<std::shared_ptr<GameObject> >& sceneObjects){
	GameInfo gameinfo ={0, 0, 1, Ogre::Vector3(length, height, width)};
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

	//create Pockets
	for (int i = 0; i < 8; i++) {
		Ogre::Vector3 location;
		if (i % 4 == 0) {
			location = Ogre::Vector3(-500.0f, -500.0f, -500.0f);
		} else if (i % 4 == 1) {
			location = Ogre::Vector3(-500.0f, -500.0f, 500.0f);
		} else if (i % 4 == 2) {
			location = Ogre::Vector3(500.0f, -500.0f, 500.0f);
		} else {
			location = Ogre::Vector3(500.0f, -500.0f, -500.0f);
		}
		if (i >= 4) {
			location.y = -location.y;
		}

		auto goal = std::make_shared<Ball>(mSceneMgr,
			std::string("g") + Ogre::StringConverter::toString(i),
			100.0f,
			location,
			BallType::BALL_GOAL);
		goal->setMaterial("Balls/Blue");
		auto triggerCollider = std::make_shared<PhysicsSphereCollider>(
			goal,
			physics,
			90.0f,
			0.0f,
   	             	true
		);
		auto goalController = std::make_shared<GoalController>(gameinfo);
		goal->addComponent(triggerCollider);
		goal->addComponent(goalController);
		sceneObjects.push_back(goal);
	}

	//create balls
	std::mt19937 rng(time(0));
	std::uniform_real_distribution<float> xzDist(-300.0f, 300.0f); 
	std::uniform_real_distribution<float> yDist(-300.0f, 300.0f);
	for (int i = 0; i < 16; i++) {
		const std::string sphereEntityName = "sph";
		auto sph = std::make_shared<Ball>(
			mSceneMgr,
			sphereEntityName + Ogre::StringConverter::toString(i),
			i == 0 ? 50.0f : 40.0f,
			Ogre::Vector3(0 + xzDist(rng),
				      yDist(rng),
				      -100.0f + xzDist(rng)),
                        /* make 1st ball a cue ball 
			i == 0 ? BallType::BALL_CUE : BallType::BALL_POOL		);
		if (i == 0) {
			sph->setMaterial("Balls/White");
		} else {
			sph->setMaterial("Examples/Hilite/Yellow");
		}
		auto sphCollider = std::make_shared<PhysicsSphereCollider>(
			sph,
			physics,
			i == 0 ? 50.0f : 40.0f, /* collider radius 
                	i == 0 ? 15.0f : 10.0f /* cue ball heavier 
		);
		sph->addComponent(sphCollider);
		auto ballSound = std::make_shared<ObjectSound>(sph);
		sph->addComponent(ballSound);
		if(sph->getTag() == 0x2)
			gameinfo.ballPositions.push_back(sph->getWorldPosition());
		sceneObjects.push_back(sph);
	}

	return gameinfo;
}
*/
void setPositions(std::shared_ptr<GameInfo>& gameinfo, const std::vector<std::shared_ptr<GameObject> >& sceneObjects){
	int startscore = gameinfo.get()->ballPositions.size();
	gameinfo.get()->ballPositions.clear();
	for( auto go : sceneObjects ){
		if(go.get()->getTag() == 0x2){
			gameinfo.get()->ballPositions.push_back(go.get()->getWorldPosition());
			
			//get ball Physics comonent, get velocity etc
		}
	}
	gameinfo.get()->scoreP1 += startscore - gameinfo.get()->ballPositions.size();
}
