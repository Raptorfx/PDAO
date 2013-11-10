#ifndef GAMECONTEXT_H
#define GAMECONTEXT_H

#include "Ogre/Ogre.h"

#include "DAO_Application.h"
#include "PlayerCharacter.h"
#include "CameraControlSystem.h"
#include "LogManager.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "TerrainManager.h"

class CameraControlSystem;
class InputManager;
class RenderManager;
class LogManager;
class PlayerCharacter;

class GameContext
{
public:
	GameContext(Ogre::RenderWindow* renderWindow, RenderManager* renderManager);
	~GameContext(void);

	PlayerCharacter* getPlayer();
	CameraControlSystem* getCameraControl();

private:
	RenderManager*          renderManager;
    InputManager*           inputManager;
    LogManager*             logManager;

	PlayerCharacter*		player;
	CameraControlSystem*    cameraControl;

    /*long int                previousTurn;
    Ogre::SceneNode*        creatureSceneNode;
    Ogre::SceneNode*        roomSceneNode;
    Ogre::SceneNode*        fieldSceneNode;
    Ogre::SceneNode*        lightSceneNode;
    Ogre::Timer             statsDisplayTimer;*/
};

#endif
