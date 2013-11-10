#include "GameContext.h"


GameContext::GameContext(Ogre::RenderWindow* renderWindow, RenderManager* renderManager)
{
	logManager = LogManager::getSingletonPtr();
    renderManager = renderManager;
    
    //NOTE This is moved here temporarily.
    try
    {
        //logManager->logMessage("Creating camera...", Ogre::LML_NORMAL);
        //renderManager->createCamera();
        //logManager->logMessage("Creating viewports...", Ogre::LML_NORMAL);
        //renderManager->createViewports();
        logManager->logMessage("Creating scene...", Ogre::LML_NORMAL);
        renderManager->createScene();
		//renderManager->initCEGUI();
    }
    catch(Ogre::Exception& e)
    {
        //ODApplication::displayErrorMessage("Ogre exception when ininialising the render manager:\n" + e.getFullDescription(), false);
        exit(0);
        //cleanUp();
        //return;
    }
    catch (std::exception& e)
    {
        //ODApplication::displayErrorMessage("Exception when ininialising the render manager:\n" + std::string(e.what()), false);
        exit(0);
        //cleanUp();
        //return;
    }

	

    cameraControl = new CameraControlSystem("Camera_Control_System",renderManager->getSceneManager(),renderManager->getCamera());
	player = new PlayerCharacter("Player_1", renderManager->getSceneManager(), this);
    
    
    logManager->logMessage("Created camera manager");
}


GameContext::~GameContext(void)
{
	delete player;
	delete cameraControl;
	delete renderManager;
}


PlayerCharacter* GameContext::getPlayer()
{ 
	return player; 
}

CameraControlSystem* GameContext::getCameraControl()
{ 
	return cameraControl; 
}

