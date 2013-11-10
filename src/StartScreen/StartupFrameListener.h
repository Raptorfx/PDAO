#ifndef STARTUPFRAMELISTENER_H
#define STARTUPFRAMELISTENER_H

//#include <iostream>

#include "boost/shared_ptr.hpp"
#include "boost/thread.hpp"

#include "Ogre/Ogre.h"
#include "Ogre/OgreFrameListener.h"
#include "Ogre/OgreWindowEventUtilities.h"
#include "OIS/OIS.h"

#include "../Sleep.h"

#include "../DAO_FrameListener.h"

#include "../InputManager.h"
#include "../LogManager.h"
#include "../RenderManager.h"

#include "LoginWindow.h"
#include "WelcomeWindow.h"

class WelcomeWindow;
class LoginWindow;
class DAO_FrameListener;

class InputManager;
class LogManager;
class RenderManager;

class StartupFrameListener :
        public Ogre::FrameListener,
        public Ogre::WindowEventListener
{
public:
	StartupFrameListener(Ogre::RenderWindow* win, Ogre::Root* ogre_root);
	~StartupFrameListener();

	void requestExit();

	//Adjust mouse clipping area
    virtual void windowResized(Ogre::RenderWindow* rw);

    //Unattach OIS before window shutdown (very important under Linux)
    virtual void windowClosed(Ogre::RenderWindow* rw);

	// Override frameStarted event to process that (don't care about frameEnded)
	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	bool frameEnded();

	void changeToWelcomeWindow();
	void changeToMainFrameListener();

	//CEGUI Functions
    //bool quit(const CEGUI::EventArgs &e);
    //Ogre::RaySceneQueryResult& doRaySceneQuery(const OIS::MouseEvent &arg);

private:

	void exitApplication();
	void asyncLoadMainFramelistener();

	CEGUI::String mLoginUsername_;

	Ogre::RenderWindow*     mWindow_;
	DAO_FrameListener*		mDAOFrameListener_;
	Ogre::Root*				mOgreRoot_;

	//boost::shared_ptr<GameContext>    gameContext_;

    InputManager*           inputManager_;
	RenderManager*          renderManager_;
	LogManager*             logManager;
	//CameraControlSystem::CameraMode				mCamMode_;
	int mCamMode_;

	boost::shared_ptr<LoginWindow>		mLoginWindow_;
	boost::shared_ptr<WelcomeWindow>	mWelcomeWindow_;

	//Store the mouse position for hide/show
	CEGUI::Point m_mouse_cursor_pos_;

	Ogre::RaySceneQuery*    mRaySceneQuery_;

	float	                frameDelay_;
	bool					exitRequested_;

	boost::shared_ptr<boost::thread>	mLoadingThread_;
	bool								mMainFrameListenerLoaded_;

	boost::mutex lock_;
};


#endif /* DAO_FRAMELISTENER */
