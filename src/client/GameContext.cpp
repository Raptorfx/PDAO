#include "GameContext.h"


GameContext::GameContext(Ogre::RenderWindow* renderWindow, RenderManager* renderManager)
{
	logManager = LogManager::getSingletonPtr();
    renderManager = renderManager;
    
    //NOTE This is moved here temporarily.
    try
    {
        logManager->logMessage("Creating scene...", Ogre::LML_NORMAL);
        renderManager->createScene();
    }
    catch(Ogre::Exception& e)
    {
        exit(0);
    }
    catch (std::exception& e)
    {
        exit(0);
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

