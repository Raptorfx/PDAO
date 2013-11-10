#include "DAO_Application.h"


const double DAO_Application::DEFAULT_FRAMES_PER_SECOND = 50.0;
double DAO_Application::MAX_FRAMES_PER_SECOND = DEFAULT_FRAMES_PER_SECOND;

template<> DAO_Application* Ogre::Singleton<DAO_Application>::msSingleton = 0;

DAO_Application::DAO_Application(void) :
        root_(0),
        window_(0)
{
	new GlobalValueStore();
}


DAO_Application::~DAO_Application(void)
{
	if(root_)
		delete root_;
}



int DAO_Application::startup()
{
	//Random::initialize();

	DAO_ResourceManager* resMgr = new DAO_ResourceManager;
	root_ = new Ogre::Root(
            resMgr->getPluginsPath(),
            resMgr->getCfgFile(),
            resMgr->getLogFile());

    /* TODO: Skip this and use root.restoreConfig()
     * to load configuration settings if we are sure there are valid ones
     * saved in ogre.cfg
     * We should use this later (when we have an own setup options screen)
     * to avoid having the setup dialog started on every run
     */
    /* TODO: create our own options menu and define good default values
     *       (drop smaller than 800x600, AA, shadow quality, mipmaps, etc)
     */

	if(!root_->restoreConfig())
	{
		if (!root_->showConfigDialog())
		{
			return -1;
		}
	}


	window_ = root_->initialise(true,"Dark Ages Online");

	LogManager* logManager = new LogManager();
    logManager->setLogDetail(Ogre::LL_BOREME);

	resMgr->loadResourceGroup("Base");

	//for CEGui
	/*resMgr->loadResourceGroup("Imagesets");
	resMgr->loadResourceGroup("Fonts");
	resMgr->loadResourceGroup("Schemes");
	resMgr->loadResourceGroup("LookNFeel");
	resMgr->loadResourceGroup("Layouts");*/

	resMgr->loadResourceGroup("Mesh");
	resMgr->loadResourceGroup("Sinbad");

	//inputManager_ = new InputManager;


	logManager->logMessage("*** Creating frame listener *** ", Ogre::LML_NORMAL);
    root_->addFrameListener(new DAO_FrameListener(window_));
	//StartupFrameListener* sfl = new StartupFrameListener(window_, root_);
	//root_->addFrameListener(sfl);
    logManager->logMessage("*** Frame listener created *** ", Ogre::LML_NORMAL);

	//mainLoop();


	try
    {
        //root_->startRendering();
		mainLoop();
    }
    catch(Ogre::Exception& e)
    {
        displayErrorMessage("Ogre exception:\n" + e.getFullDescription());
        cleanUp();
        return -1;
    }
    catch(std::exception& e)
    {
        displayErrorMessage("Exception:\n" + std::string(e.what()));
        cleanUp();
        return -1;
    }


	//Moved out from cleanup, as we only want to remove it if it exists.
    root_->removeFrameListener(DAO_FrameListener::getSingletonPtr());
	cleanUp();

	return 0;
}

/*! \brief Delete the various singleton objects and clean up other stuff
 *
 */
void DAO_Application::cleanUp()
{
	delete DAO_ResourceManager::getSingletonPtr();
    //delete DAO_FrameListener::getSingletonPtr();
    delete RenderManager::getSingletonPtr();
    delete LogManager::getSingletonPtr();
}

/*! \brief Display a GUI error message
 *
 */
void DAO_Application::displayErrorMessage(const std::string& message, bool log)
{
    if(log)
    {
        LogManager::getSingleton().logMessage(message, Ogre::LML_CRITICAL);
    }
    Ogre::ErrorDialog e;
    e.display(message, LogManager::GAMELOG_NAME);
}

void DAO_Application::mainLoop()
{
	while(_keepRunning)
	{
		Ogre::WindowEventUtilities::messagePump();
		_keepRunning = root_->renderOneFrame();
	}
}
