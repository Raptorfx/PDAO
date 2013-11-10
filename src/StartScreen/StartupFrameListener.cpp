#include "StartupFrameListener.h"

StartupFrameListener::StartupFrameListener(Ogre::RenderWindow* win, Ogre::Root* ogre_root) :
		mWindow_(win),
		mOgreRoot_(ogre_root),
		frameDelay_(0.0),
		exitRequested_(false),
		mMainFrameListenerLoaded_(false)
{
	renderManager_ = new RenderManager();
	renderManager_->createCamera();
	renderManager_->createViewports();
	renderManager_->initCEGUI();

	inputManager_ = new InputManager;
	
	//Set initial mouse clipping size
    windowResized(mWindow_);

	mWindow_->setDeactivateOnFocusChange(false);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow_, this);

	mLoginWindow_.reset(new LoginWindow(this));

	mLoginUsername_ = "";

	LogManager::getSingletonPtr()->logMessage("*** Startup FrameListener initialized ***");
}


StartupFrameListener::~StartupFrameListener(void)
{
	delete mDAOFrameListener_;
	delete inputManager_;
	delete renderManager_;

}

void StartupFrameListener::changeToWelcomeWindow()
{
	mWelcomeWindow_.reset(new WelcomeWindow(this));
	mLoginWindow_.reset();
}


void StartupFrameListener::changeToMainFrameListener()
{
	mLoadingThread_.reset(new boost::thread(boost::bind(&StartupFrameListener::asyncLoadMainFramelistener, this)));
}

void StartupFrameListener::asyncLoadMainFramelistener()
{
	mDAOFrameListener_ = new DAO_FrameListener(mWindow_, inputManager_, renderManager_);
}

/*! \brief Adjust mouse clipping area
 *
 */
void StartupFrameListener::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = inputManager_->getMouse()->getMouseState();
    ms.width = width;
    ms.height = height;

    //Notify CEGUI that the display size has changed.
    CEGUI::System::getSingletonPtr()->notifyDisplaySizeChanged(CEGUI::Size(static_cast<float> (width), static_cast<float> (height)));
}

/*! \brief Unattach OIS before window shutdown (very important under Linux)
 *
 */
void StartupFrameListener::windowClosed(Ogre::RenderWindow* rw)
{
    if(rw == mWindow_)
    {
        if(inputManager_)
        {
            delete inputManager_;
        }
    }
}

void StartupFrameListener::requestExit()
{
	boost::mutex::scoped_lock l(lock_);
    exitRequested_ = true;
}

void StartupFrameListener::exitApplication()
{
	//Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow_, this);
    windowClosed(mWindow_);
}


/*! \brief The main rendering function for the OGRE 3d environment.
 *
 * This function is the one which actually carries out all of the rendering in
 * the OGRE 3d system.  Since all the rendering must happen here, one of the
 * operations performed by this function is the processing of a request queue
 * full of RenderRequest structures.
 */
bool StartupFrameListener::frameStarted(const Ogre::FrameEvent& evt)
{
	if (mWindow_->isClosed())
		return false;

	if(mMainFrameListenerLoaded_)
	{
		mWelcomeWindow_.reset();
		mOgreRoot_->removeFrameListener(this);
		mOgreRoot_->addFrameListener(mDAOFrameListener_);
		return true;
	}

	std::cout << evt.timeSinceLastFrame << std::endl;

	CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

	//Need to capture/update each device
    inputManager_->getKeyboard()->capture();
    inputManager_->getMouse()->capture();


    // Sleep to limit the framerate to the max value
    frameDelay_ -= evt.timeSinceLastFrame;
    if (frameDelay_ > 0.0)
    {
        usleep(10.0f * 100.0f * frameDelay_);
    }
    else
    {
        //FIXME: I think this 2.0 should be a 1.0 but this gives the
        // correct result.  This probably indicates a bug.
        frameDelay_ += 1.0 / DAO_Application::MAX_FRAMES_PER_SECOND;
    }

	//If an exit has been requested, start cleaning up.
	if (exitRequested_) 
	{
		exitApplication();
		return false;
	}

	inputManager_->setMouseExtraData(Ogre::Vector4(0.0f, 0.0f, 0.0f, 0.0f)); //Reset the angle for the next frame

	return true;
}


bool StartupFrameListener::frameRenderingQueued(const Ogre::FrameEvent& evt)
{

	return true;
}


bool StartupFrameListener::frameEnded()
{
	return true;
}