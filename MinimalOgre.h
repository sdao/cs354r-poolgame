/*
-----------------------------------------------------------------------------
Filename:    MinimalOgre.h
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
#ifndef __MinimalOgre_h_
#define __MinimalOgre_h_
 
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
 
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
 
#include <SdkTrays.h>
#include <SdkCameraMan.h>

#include "GameEnums.h"
#include "GameObject.h"
#include "Player.h"
//#include "Balls.h"
#include "GameInfo.h"
#include "Physics.h"
#include "Server.h"
#include "Client.h"
#include <vector>
#include <memory>
#include <atomic>
#include <mutex>

class MinimalOgre : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener, OgreBites::SdkTrayListener
{
public:
    MinimalOgre(void);
    virtual ~MinimalOgre(void);
    bool go(void);
protected:
    Ogre::Root *mRoot;
    Ogre::Camera* mCamera;
    Ogre::SceneManager* mSceneMgr;
    Ogre::RenderWindow* mWindow;
    Ogre::String mResourcesCfg;
    Ogre::String mPluginsCfg;
 
	Ogre::OverlaySystem *mOverlaySystem;
 
    // OgreBites
	OgreBites::InputContext mInputContext;
    OgreBites::SdkTrayManager* mTrayMgr;
    OgreBites::SdkCameraMan* mCameraMan;      // basic camera controller
    OgreBites::ParamsPanel* scoreboard;    // displays scores
    OgreBites::ParamsPanel* controls;    // displays controls
    OgreBites::Label* pauseLabel;          // Label saying Pause
    OgreBites::Label* endLabel;          // Label saying Win/Lose
    OgreBites::Label* waitLabel;          // Label saying waiting for connection
    bool mCursorWasVisible;                   // was cursor visible before dialog appeared
    bool mShutDown;
 
    // OIS Input devices
    OIS::InputManager* mInputManager;
    OIS::Mouse*    mMouse;
    OIS::Keyboard* mKeyboard;
 
    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
 
    // OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	//Assignment3 methods
	void setupField(bool, float, float, float);
    void destroyField(); 

    // Ogre::WindowEventListener
    virtual void windowResized(Ogre::RenderWindow* rw);
    virtual void windowClosed(Ogre::RenderWindow* rw);

	// Assignment2 Data Starts
    OgreBites::SdkTrayManager* menuTray;
    OgreBites::Button* sp;
    OgreBites::Button* mp;
    OgreBites::Button* ex;
    OgreBites::Button* mm;
    OgreBites::Button* resume;
    OgreBites::Button* ff;
    OgreBites::Button* back;
    OgreBites::Button* host;
    OgreBites::Button* connect;
    OgreBites::TextBox* connectText;
	GameState state;
	Physics physics;
	std::vector<std::shared_ptr<GameObject>> sceneObjects;
	std::shared_ptr<Player> player;
	std::shared_ptr<GameInfo> gameinfo;
	std::atomic_bool multiplayer;
	bool client;
	Server serverManager;
	Client clientManager;
    std::atomic_bool isConnectedHost;
    std::atomic_bool clientHit;
    std::atomic_bool clientRecieveBallPos;


    // SDKTrayListener Functions

    virtual void buttonHit (OgreBites::Button *button) override;
    // virtual void itemSelected (SelectMenu *menu)
    // virtual void labelHit (Label *label)
    // virtual void sliderMoved (Slider *slider)
    // virtual void checkBoxToggled (CheckBox *box)
    // virtual void okDialogClosed (const Ogre::DisplayString &message)
    // virtual void yesNoDialogClosed (const Ogre::DisplayString &question, bool yesHit)
};

#endif // #ifndef __MinimalOgre_h_
