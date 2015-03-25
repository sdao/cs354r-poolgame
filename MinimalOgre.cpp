/*
-----------------------------------------------------------------------------
Filename:    MinimalOgre.cpp
-----------------------------------------------------------------------------
 
This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#include "MinimalOgre.h"
#include "PhysicsSphereCollider.h"
#include "PhysicsBoxCollider.h"
#include "ConstantVelocity.h"
#include "Ball.h"
#include "UpdateInfo.h"
#include <iostream>
#include <random>
#include <cassert>
#include "BGMusic.h"
#include "CueStickController.h"
#include "ObjectSound.h"
#include <string>
#include "GoalController.h"
 
//-------------------------------------------------------------------------------------
MinimalOgre::MinimalOgre(void)
    : mRoot(0),
    mCamera(0),
    mSceneMgr(0),
    mWindow(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mTrayMgr(0),
    mCameraMan(0),
    scoreboard(0),
    controls(0),
    pauseLabel(0),
    mCursorWasVisible(false),
    mShutDown(false),
    mInputManager(0),
    mMouse(0),
    mKeyboard(0),
	mOverlaySystem(0),
	menuTray(0),
    sp(0),
    mp(0),
    ex(0),
    back(0),
    host(0),
    connect(0),
	state(GameState::Main),
	physics(),
	sceneObjects(),
	player(),
	gameinfo(),
	multiplayer(false),
	client(false),
        serverManager(),
        clientManager()
{
}
//-------------------------------------------------------------------------------------
MinimalOgre::~MinimalOgre(void)
{
    if (mTrayMgr) delete mTrayMgr;
	if (menuTray) delete menuTray;
    if (mCameraMan) delete mCameraMan;
	if (mOverlaySystem) delete mOverlaySystem;
 
    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;
}
 
bool MinimalOgre::go(void)
{
#ifdef _DEBUG
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
#else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
#endif
 
    // construct Ogre::Root
    mRoot = new Ogre::Root(mPluginsCfg);
 
//-------------------------------------------------------------------------------------
    // setup resources
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);
 
    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
 
    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
//-------------------------------------------------------------------------------------
    // configure
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(mRoot->restoreConfig() || mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        mWindow = mRoot->initialise(true, "MinimalOgre Render Window");
    }
    else
    {
        return false;
    }


//-------------------------------------------------------------------------------------
    // choose scenemanager
    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
//-------------------------------------------------------------------------------------
	// initialize the OverlaySystem (changed for 1.9)
	mOverlaySystem = new Ogre::OverlaySystem();
    mSceneMgr->addRenderQueueListener(mOverlaySystem);
//-------------------------------------------------------------------------------------
    // create camera
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");
 
    // Position it at 500 in Z direction
    //mCamera->setPosition(Ogre::Vector3(0,0,80));
    // Look back along -Z
    mCamera->lookAt(Ogre::Vector3(0,0,-300));
    mCamera->setNearClipDistance(5);
 
    mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller

//-------------------------------------------------------------------------------------
    // create viewports
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
 
    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
//-------------------------------------------------------------------------------------
    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
//-------------------------------------------------------------------------------------
    // Create any resource listeners (for loading screens)
    //createResourceListener();
//-------------------------------------------------------------------------------------
    // load resources
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
//-------------------------------------------------------------------------------------

    //create FrameListener
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
 
    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
 
    mInputManager = OIS::InputManager::createInputSystem( pl );
 
    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
 
    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);
 
    //Set initial mouse clipping size
    windowResized(mWindow);
 
    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
 
    mInputContext.mKeyboard = mKeyboard;
    mInputContext.mMouse = mMouse;

    mRoot->addFrameListener(this);

    // Setup GUI
    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mInputContext, this);
    sp = mTrayMgr->createButton(OgreBites::TL_CENTER, "Single Player", "Solo");
    mp = mTrayMgr->createButton(OgreBites::TL_CENTER, "Multi Player", "Multiplayer");
    ex = mTrayMgr->createButton(OgreBites::TL_CENTER, "Exit", "Quit");

//-------------------------------------------------------------------------------------
    mRoot->startRendering();

    return true;
}
 
bool MinimalOgre::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;
 
    if(mShutDown)
        return false;
 
    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();
 
    mTrayMgr->frameRenderingQueued(evt);
 
    if (!mTrayMgr->isDialogVisible())
    {
        mCameraMan->frameRenderingQueued(evt);   // if dialog isn't up, then update the camera
    }

	if(state == GameState::Play){

		
    	// Run physics.
    	physics.stepSimulation(evt.timeSinceLastFrame);


    	// Update components.
		UpdateInfo info;
	    info.physics = &physics;
	    info.deltaTime = evt.timeSinceLastFrame;
    	for (auto go : sceneObjects) {
	      go->update(info);
    	}
	

		//TODO come back for multiplayer
		if( !client && !setPositions(gameinfo, sceneObjects) && player->isInState(PlayerState::Wait)){
			player->setState(PlayerState::Hit);
		}

        //update player
        player->update();
        
        //set param values on GUI
        std::string buffer = std::to_string(gameinfo.get()->scoreP1);
        scoreboard->setParamValue(0, buffer);
        buffer = std::to_string(gameinfo.get()->ballPositions.size());
        scoreboard->setParamValue(1, buffer);
	}

    else if (state == GameState::Main)
    {
        // Control Main Menu Stuff
    }

    return true;
}
//-------------------------------------------------------------------------------------
bool MinimalOgre::keyPressed( const OIS::KeyEvent &arg )
{
    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up
 
    // if (arg.key == OIS::KC_M)   // Toggle Background Music
    // {
    //     auto music = player->getComponent<BGMusic>();
    //     music->playOrPause();
    //     if (scoreboard->getParamValue(4) == "On")
    //         scoreboard->setParamValue(4, "Off");
    //     else
    //         scoreboard->setParamValue(4, "On");
    // }
    else if (arg.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
    }

 
    //mCameraMan->injectKeyDown(arg);

	switch(state){
		case Main:
			break;
		case Play:
			//pass keypress to player Object
			player->getKeyPress(arg);

		    if (arg.key == OIS::KC_SPACE)
		    {
	    	    if (player && player->isInState(PlayerState::Hit) &&
					gameinfo.get()->playerturn == 0) {
					
                    std::string strength = scoreboard->getParamValue(2);
		            auto cueController = player->getComponent<CueStickController>();
		            cueController->hit(strength);
	  	    		player->setState(PlayerState::Wait);
					if(multiplayer){
						gameinfo.get()->playerturn = (gameinfo.get()->playerturn+1)%2;
					}
        		}
    		}
            else if (arg.key == OIS::KC_O)   // toggle visibility of scoreboard
            {
                if (scoreboard->getTrayLocation() == OgreBites::TL_NONE)
                {
                    mTrayMgr->moveWidgetToTray(scoreboard, OgreBites::TL_TOPLEFT, 0);
                    scoreboard->show();
                }
                else
                {
                    mTrayMgr->removeWidgetFromTray(scoreboard);
                    scoreboard->hide();
                }
            }
            else if (arg.key == OIS::KC_C)   // toggle visibility of controls
            {
                if (controls->getTrayLocation() == OgreBites::TL_NONE)
                {
                    mTrayMgr->moveWidgetToTray(controls, OgreBites::TL_CENTER, 0);
                    controls->show();
                }
                else
                {
                    mTrayMgr->removeWidgetFromTray(controls);
                    controls->hide();
                }
            }
    		/*
    		if (arg.key == OIS::KC_G) // toggle gravity
    		{
    			if (player && player->isInState(PlayerState::Hit)) {
    				bool isGravitating = physics.isGravityEnabled();
    				if (isGravitating) {
           					physics.disableGravity();
    				} else {
    					physics.enableGravity();
            		}
    	                if (scoreboard->getParamValue(3) == "On")
            	            scoreboard->setParamValue(3, "Off");
    	                else
            	            scoreboard->setParamValue(3, "On");
    			}
        	}
    		*/
            else if (arg.key == OIS::KC_RIGHT)
            {
                if (player && player->isInState(PlayerState::Hit)) {
                    if (scoreboard->getParamValue(2) == "Low")
                    {
                        scoreboard->setParamValue(2, "Medium");
                    }
                    else if (scoreboard->getParamValue(2) == "Medium")
                    {
                        scoreboard->setParamValue(2, "High");
                    }
                    else if (scoreboard->getParamValue(2) == "High")
                    {
                        scoreboard->setParamValue(2, "Low");
                    }
                }
            }

            else if (arg.key == OIS::KC_LEFT)
            {
                if (player && player->isInState(PlayerState::Hit)) {
                    if (scoreboard->getParamValue(2) == "Low")
                    {
                        scoreboard->setParamValue(2, "High");
                    }
                    else if (scoreboard->getParamValue(2) == "Medium")
                    {
                        scoreboard->setParamValue(2, "Low");
                    }
                    else if (scoreboard->getParamValue(2) == "High")
                    {
                        scoreboard->setParamValue(2, "Medium");
                    }
                }
            }

            else if (arg.key == OIS::KC_R)   // cycle polygon rendering mode
            {
                Ogre::String newVal;
                Ogre::PolygonMode pm;
         
                switch (mCamera->getPolygonMode())
                {
                case Ogre::PM_SOLID:
                    newVal = "Wireframe";
                    pm = Ogre::PM_WIREFRAME;
                    break;
                case Ogre::PM_WIREFRAME:
                    newVal = "Points";
                    pm = Ogre::PM_POINTS;
                    break;
                default:
                    newVal = "Solid";
                    pm = Ogre::PM_SOLID;
                }
         
                mCamera->setPolygonMode(pm);
            }

            else if (arg.key == OIS::KC_P)
            {
                state = Pause;

                pauseLabel = mTrayMgr->createLabel(OgreBites::TL_CENTER, "Pause", "GAME PAUSED: Unpause by pressing 'P'", 600);
        		for(int i = 0; i < 6; i++){
        			player.get()->inputKeys[i] = false;
        		}
            }
			break;
		case Pause:
            if (arg.key == OIS::KC_P)
            {
                state = Play;
                mTrayMgr->destroyWidget(pauseLabel);
            }
			//show pause menu
			break;
		case End:
			//show "win/loss" menu
			break;
		default:
			break;
	}

    return true;
}
 
bool MinimalOgre::keyReleased( const OIS::KeyEvent &arg )
{
    mCameraMan->injectKeyUp(arg);

	switch(state){
		case Main:
			break;
		case Play:
			//pass keyrelease to player Object
			player->getKeyRelease(arg);
			break;
		case Pause:
			break;
		case End:
			break;
		default:
			break;
	}

    return true;
}
 
bool MinimalOgre::mouseMoved( const OIS::MouseEvent &arg )
{
    //if (mTrayMgr->injectMouseMove(arg)) return true;
    //mCameraMan->injectMouseMove(arg);
	//if(player.isInState(PlayerState::Play))
    if (state == Play)
	   player->getMouseEvent(arg);
    if (state == Main)
        mTrayMgr->injectMouseMove(arg);
    return true;
}
 
bool MinimalOgre::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectMouseDown(arg, id)) return true;
    mCameraMan->injectMouseDown(arg, id);
    return true;
}
 
bool MinimalOgre::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectMouseUp(arg, id)) return true;
    mCameraMan->injectMouseUp(arg, id);
    return true;
}

void MinimalOgre::setupField(bool singleplayer, float length, float width, float height){

	if(!singleplayer){
		GameInfo tempgameinfo = {0, 0, 0, Ogre::Vector3(length, height, width)};
		tempgameinfo.ballPositions = std::vector<Ogre::Vector3>();

		gameinfo = std::make_shared<GameInfo>(GameInfo(tempgameinfo));
	}

    //make the walls
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingletonPtr()->createPlane("wallMesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 100, 100, 5, 5, true, 1, 1.0 , 1.0, Ogre::Vector3::UNIT_X);

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
                        /* make 1st ball a cue ball */
			i == 0 ? BallType::BALL_CUE : BallType::BALL_POOL		);
		if (i == 0) {
			sph->setMaterial("Balls/White");
		} else {
			sph->setMaterial("Examples/Hilite/Yellow");
		}

		if(!singleplayer){
			auto sphCollider = std::make_shared<PhysicsSphereCollider>(
				sph,
				physics,
				i == 0 ? 50.0f : 40.0f, /* collider radius */
           	     	i == 0 ? 15.0f : 10.0f /* cue ball heavier */
			);
			sph->addComponent(sphCollider);
			auto ballSound = std::make_shared<ObjectSound>(sph);
			sph->addComponent(ballSound);
			if(sph->getTag() == 0x2)
				gameinfo.get()->ballPositions.push_back(sph->getWorldPosition());
		}
		else{
			//TODO network components instead of physics on balls
		}
		sceneObjects.push_back(sph);
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
		if(!singleplayer){
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
		}
		sceneObjects.push_back(goal);
	}

    //create player
    player = std::make_shared<Player>(mSceneMgr,
                      mCamera,
                      "column.mesh",
                      "Player");
    sceneObjects.push_back(player);
    //player->rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(Ogre::Degree(180)));
    player->setScale(Ogre::Vector3(0.03, 0.3, 0.03));
    
    // put bg music on player
        auto bgMusic = std::make_shared<BGMusic>();
        player->addComponent(bgMusic);
    
    // putting the cue controller on the player
    auto cueController = std::make_shared<CueStickController>(player);
    player->addComponent(cueController);

    // putting audio controller on player
    auto soundController = std::make_shared<ObjectSound>(player);
    player->addComponent(soundController);

    // Set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
 
    // Create a light
    Ogre::Light* l = mSceneMgr->createLight("MainLight");
    l->setPosition(20,80,50);

    Ogre::StringVector scores;
    scores.push_back("Score");
    scores.push_back("Balls Remaining");
    scores.push_back("Hit Strength");
    scores.push_back("Gravity");
    scores.push_back("Music");

    Ogre::StringVector cList;

    cList.push_back("Controls");
    cList.push_back("w");
    cList.push_back("a");
    cList.push_back("s");
    cList.push_back("d");
    cList.push_back("q");
    cList.push_back("e");
    cList.push_back("left-arrow");
    cList.push_back("right-arrow");
    cList.push_back("spacebar");
    cList.push_back("c");
    cList.push_back("o");
    cList.push_back("m");
    cList.push_back("g");
    cList.push_back("p");
    cList.push_back("r");

    scoreboard = mTrayMgr->createParamsPanel(OgreBites::TL_TOPLEFT, "Scoreboard", 250, scores);
    scoreboard->setParamValue(1, "0");
    scoreboard->setParamValue(2, "Low");
    scoreboard->setParamValue(3, "Off");
    scoreboard->setParamValue(4, "On");
    mTrayMgr->moveWidgetToTray(scoreboard, OgreBites::TL_TOPLEFT, 0);
    scoreboard->show();

    controls = mTrayMgr->createParamsPanel(OgreBites::TL_CENTER, "Controls", 350, cList);
    controls->setParamValue(1, "move forward");
    controls->setParamValue(2, "move left");
    controls->setParamValue(3, "move backward");
    controls->setParamValue(4, "move right");
    controls->setParamValue(5, "move down");
    controls->setParamValue(6, "move up");
    controls->setParamValue(7, "left-scroll hit strength");
    controls->setParamValue(8, "right-scroll hit strength");
    controls->setParamValue(9, "hit ball");
    controls->setParamValue(10, "toggle controls menu");
    controls->setParamValue(11, "toggle scoreboard");
    controls->setParamValue(12, "toggle music");
    controls->setParamValue(13, "toggle gravity");
    controls->setParamValue(14, "pause/unpause game");
    controls->setParamValue(15, "cycles polygon rendering");
    mTrayMgr->moveWidgetToTray(controls, OgreBites::TL_CENTER, 0);
    mTrayMgr->removeWidgetFromTray(controls);
    controls->hide();
} 

//Adjust mouse clipping area
void MinimalOgre::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);
 
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}
 
//Unattach OIS before window shutdown (very important under Linux)
void MinimalOgre::windowClosed(Ogre::RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if( rw == mWindow )
    {
        if( mInputManager )
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );
 
            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}

void MinimalOgre::buttonHit (OgreBites::Button *button)
{
    if (button == sp)
    {
        state = Play;
        // Create the scene
        mTrayMgr->clearTray(OgreBites::TL_CENTER);
        mTrayMgr->destroyAllWidgets();
        setupField(false, 1000, 1000, 1000);
        mTrayMgr->hideCursor();
    }
    else if (button == mp)
    {
        mTrayMgr->clearTray(OgreBites::TL_CENTER);
        mTrayMgr->destroyAllWidgets();
        host = mTrayMgr->createButton(OgreBites::TL_CENTER, "Host", "Host");
        connect = mTrayMgr->createButton(OgreBites::TL_CENTER, "Connect", "Connect");
        back = mTrayMgr->createButton(OgreBites::TL_CENTER, "Back", "Back");
        // Setup Multiplayer
    }
    else if (button == ex)
    {
        mShutDown = true;
    }
    else if (button == host)
    {
        //Wait for Connection
        client = false;
        serverManager.accept(67309);
    }
    else if (button == connect)
    {
        // Pick Port and Host
        client = true;
        serverManager.connect("firefly", 67309);
    }
    else if (button == back)
    {
        mTrayMgr->clearTray(OgreBites::TL_CENTER);
        mTrayMgr->destroyAllWidgets();
        sp = mTrayMgr->createButton(OgreBites::TL_CENTER, "Single Player", "Solo");
        mp = mTrayMgr->createButton(OgreBites::TL_CENTER, "Multi Player", "Multiplayer");
        ex = mTrayMgr->createButton(OgreBites::TL_CENTER, "Exit", "Quit");
    }
}

 
 
 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif
 
#ifdef __cplusplus
extern "C" {
#endif
 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        MinimalOgre app;
 
        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }
 
        return 0;
    }
 
#ifdef __cplusplus
}
#endif
