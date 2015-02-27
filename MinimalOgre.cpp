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
#include <iostream>
#include <random>
#include "BGMusic.h"
 
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
    mDetailsPanel(0),
    scoreboard(0),
    mCursorWasVisible(false),
    mShutDown(false),
    mInputManager(0),
    mMouse(0),
    mKeyboard(0),
	mOverlaySystem(0),
	state(GameState::Play),
	physics(),
	sceneObjects()
{
}
//-------------------------------------------------------------------------------------
MinimalOgre::~MinimalOgre(void)
{
    if (mTrayMgr) delete mTrayMgr;
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
    // Create the scene
    //Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");
 
    //Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    //headNode->attachObject(ogreHead);


	//create walls

	//make the walls
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingletonPtr()->createPlane("wallMesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 300, 300, 5, 5, true, 1, 1.0 , 1.0, Ogre::Vector3::UNIT_X);


	for(int i = 0; i < 6; i++){
		Ogre::Entity* wall = mSceneMgr->createEntity("wallEntity" + i , "wallMesh");
		wall->setMaterialName("Examples/GrassFloor");
		wall->setCastShadows(false);

		const std::string wallEntityName = "wallEntity";
		auto go = std::make_shared<GameObject>(mSceneMgr, wall,
			wallEntityName + Ogre::StringConverter::toString(i));
		//Ogre::SceneNode* wallnode = mSceneMgr->getRootSceneNode()->createChildSceneNode("wallEntity" + i);
		//wallnode->attachObject(wall);
		
		// TODO: BAD! VERY BAD!
		std::shared_ptr<PhysicsBoxCollider> box;

		switch(i){
			case 0:
				go->translate(0, -150, -100);
				box = std::make_shared<PhysicsBoxCollider>(
					go,
					physics,
					Ogre::Vector3(300.0f, 1.0f, 300.0f),
					0.0f
				);

				go->addComponent(box);
				break;
			case 1:
				go->translate(0, 150, -100);
				go->rotate(Ogre::Vector3::UNIT_Z, Ogre::Radian(Ogre::Degree(180)));
				break;
			case 2:
				go->translate(150, 0, -100);
				go->rotate(Ogre::Vector3::UNIT_Z, Ogre::Radian(Ogre::Degree(90)));
				break;
			case 3:
				go->translate(-150, 0, -100);
				go->rotate(Ogre::Vector3::UNIT_Z, Ogre::Radian(Ogre::Degree(270)));
				break;
			case 4:
				go->translate(0, 0, -250);
				go->rotate(Ogre::Vector3::UNIT_X, Ogre::Radian(Ogre::Degree(90)));
				break;
			case 5:
				go->translate(0, 0, 50);
				go->rotate(Ogre::Vector3::UNIT_X, Ogre::Radian(Ogre::Degree(270)));
				break;
			default:
				break;
		}

		sceneObjects.push_back(go);
	}
	//TODO should we save the walls in the vector gameObjects?
	//create Pockets

	//create balls
	std::mt19937 rng;
	std::uniform_real_distribution<float> dist(-50.0f, 50.0f);
	for (int i = 0; i < 100; i++) {
		const std::string sphereEntityName = "sph";
		auto sph = std::make_shared<Ball>(mSceneMgr, sphereEntityName + Ogre::StringConverter::toString(i), 10.0f, Ogre::Vector3(0 + dist(rng), dist(rng), -100.0f + dist(rng)));
		sph->setMaterial("Examples/BumpyMetal");
		auto sphCollider = std::make_shared<PhysicsSphereCollider>(
			sph,
			physics,
			10.0f, /* collider radius */
                	1.0f
		);
		sph->addComponent(sphCollider);
		sceneObjects.push_back(sph);
	}

	auto sph = std::make_shared<Ball>(mSceneMgr, "kinematic", 20.0f, Ogre::Vector3(0, -130.0f, -100.0f));
	sph->setMaterial("Examples/GrassFloor");
	auto constantVel = std::make_shared<ConstantVelocity>(
		sph,
		Ogre::Vector3(10.0f, 0.0f, 0.0f));
	auto kinCollider = std::make_shared<PhysicsSphereCollider>(
		sph,
		physics,
		20.0f,
		0.0f /* static/kinematic object */
	);
    auto bgMusic = std::make_shared<BGMusic>();
	sph->addComponent(constantVel);
	sph->addComponent(kinCollider);
    sph->addComponent(bgMusic);
	sceneObjects.push_back(sph);

	//create player
	player = new Player(mSceneMgr, mCamera, "ogrehead.mesh", "Player");
	//player->rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(Ogre::Degree(180)));
	player->setScale(Ogre::Vector3(0.5, 0.5, 0.5));
	//attack camera to player or vice versa

    // Set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
 
    // Create a light
    Ogre::Light* l = mSceneMgr->createLight("MainLight");
    l->setPosition(20,80,50);
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
    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mInputContext, this);
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    mTrayMgr->hideCursor();
 
    // create a params panel for displaying sample details
    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("");
    items.push_back("Filtering");
    items.push_back("Poly Mode");
 
    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
    mDetailsPanel->setParamValue(9, "Bilinear");
    mDetailsPanel->setParamValue(10, "Solid");
    mDetailsPanel->hide();

    Ogre::StringVector scores;
    scores.push_back("Score:");
    scores.push_back("Balls Remaining:");
    scores.push_back("Time Left:");

    scoreboard = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "Scoreboard", 400, scores);
    scoreboard->setParamValue(0, "10000 You're Awesome!");
    scoreboard->setParamValue(1, "0 Good Job!");
    scoreboard->setParamValue(2, "Infinite!");
    scoreboard->hide();

 
    mRoot->addFrameListener(this);
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
        if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
        {
            mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(mCamera->getDerivedPosition().x));
            mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(mCamera->getDerivedPosition().y));
            mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mCamera->getDerivedPosition().z));
            mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().w));
            mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().x));
            mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().y));
            mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().z));
        }
    }

    // Run physics.
    physics.stepSimulation(evt.timeSinceLastFrame);

	//update player
	player->update();

    // Update components.
    // TODO: is this the right place?
    //std::cout << "going to update components\n";
    for (auto go : sceneObjects) {
      go->update(evt.timeSinceLastFrame);
    }
 
    return true;
}
//-------------------------------------------------------------------------------------
bool MinimalOgre::keyPressed( const OIS::KeyEvent &arg )
{
    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up
 
    if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats
    {
        mTrayMgr->toggleAdvancedFrameStats();
    }
    else if (arg.key == OIS::KC_G)   // toggle visibility of even rarer debugging details
    {
        if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
        {
            mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
            mDetailsPanel->show();
        }
        else
        {
            mTrayMgr->removeWidgetFromTray(mDetailsPanel);
            mDetailsPanel->hide();
        }
    }
    else if (arg.key == OIS::KC_O)   // toggle visibility of even rarer debugging details
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
    else if (arg.key == OIS::KC_T)   // cycle texture filtering mode
    {
        Ogre::String newVal;
        Ogre::TextureFilterOptions tfo;
        unsigned int aniso;
 
        switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
        {
        case 'B':
            newVal = "Trilinear";
            tfo = Ogre::TFO_TRILINEAR;
            aniso = 1;
            break;
        case 'T':
            newVal = "Anisotropic";
            tfo = Ogre::TFO_ANISOTROPIC;
            aniso = 8;
            break;
        case 'A':
            newVal = "None";
            tfo = Ogre::TFO_NONE;
            aniso = 1;
            break;
        default:
            newVal = "Bilinear";
            tfo = Ogre::TFO_BILINEAR;
            aniso = 1;
        }
 
        Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
        Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
        mDetailsPanel->setParamValue(9, newVal);
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
        mDetailsPanel->setParamValue(10, newVal);
    }
    else if(arg.key == OIS::KC_F5)   // refresh all textures
    {
        Ogre::TextureManager::getSingleton().reloadAll();
    }
    else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
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
	player->getMouseEvent(arg);
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
