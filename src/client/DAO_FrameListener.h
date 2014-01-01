#ifndef DAO_FRAMELISTENER
#define DAO_FRAMELISTENER

//#include <iostream>

#include "boost/shared_ptr.hpp"
#include "boost\thread\mutex.hpp"

#include "Ogre\Ogre.h"
#include "Ogre\OgreFrameListener.h"
#include "Ogre\OgreWindowEventUtilities.h"
#include "OIS\OIS.h"

#include "Sleep.h"

#include "InputManager.h"
#include "GameContext.h"
#include "RenderManager.h"
#include "PlayerCharacter.h"
#include "CameraControlSystem.h"
#include "GUI/GUIMain.h"

//#include "CollisionTools.h"

class InputManager;
class GameContext;
class RenderManager;
//class CameraControlSystem;
class GUIMain;

class DAO_FrameListener :
        public Ogre::Singleton<DAO_FrameListener>,
        public Ogre::FrameListener,
        public Ogre::WindowEventListener
{
public:
	DAO_FrameListener(Ogre::RenderWindow* win);
	~DAO_FrameListener();

	void requestExit();

	//Adjust mouse clipping area
    virtual void windowResized(Ogre::RenderWindow* rw);

    //Unattach OIS before window shutdown (very important under Linux)
    virtual void windowClosed(Ogre::RenderWindow* rw);

	// Override frameStarted event to process that (don't care about frameEnded)
	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	bool frameEnded();

private:

	void updateCharacterControl(const Ogre::FrameEvent& evt);

	void exitApplication();

	Ogre::RenderWindow*     mWindow_;

	boost::shared_ptr<GameContext>    gameContext_;

	RenderManager*          renderManager_;
    InputManager*           inputManager_;
	GUIMain* m_gui_main_;

	PlayerCharacter*		player_;
	CameraControlSystem*    cameraControl_;
	//CameraControlSystem::CameraMode				mCamMode_;
	int mCamMode_;

	Ogre::RaySceneQuery*    mRaySceneQuery_;

	float	                frameDelay_;
	bool					exitRequested_;

	boost::mutex lock_;
};


#endif /* DAO_FRAMELISTENER */
