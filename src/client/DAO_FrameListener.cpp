#include "DAO_FrameListener.h"

template<> DAO_FrameListener* Ogre::Singleton<DAO_FrameListener>::msSingleton = 0;

DAO_FrameListener::DAO_FrameListener(Ogre::RenderWindow* win) :
		mWindow_(win),
		frameDelay_(0.0),
		exitRequested_(false)
{
    renderManager_ = new RenderManager();
	renderManager_->createCamera();
	renderManager_->createViewports();

	gameContext_.reset(new GameContext(win, renderManager_));

	player_ = gameContext_->getPlayer();

	cameraControl_ = gameContext_->getCameraControl();

	//Initially set the camera to the player
	cameraControl_->setTargetNode(player_->getCameraNode());
	mCamMode_ = 1;

	m_gui_main_ = new GUIMain();
	m_gui_main_->init();

	inputManager_ = new InputManager;

	//Set initial mouse clipping size
    windowResized(mWindow_);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow_, this);
	

	LogManager::getSingletonPtr()->logMessage("*** FrameListener initialized ***");
}


DAO_FrameListener::~DAO_FrameListener(void)
{
	delete m_gui_main_;
}

/*! \brief Adjust mouse clipping area
 *
 */
void DAO_FrameListener::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = inputManager_->getMouse()->getMouseState();
    ms.width = width;
    ms.height = height;
}

/*! \brief Unattach OIS before window shutdown (very important under Linux)
 *
 */
void DAO_FrameListener::windowClosed(Ogre::RenderWindow* rw)
{
    if(rw == mWindow_)
    {
        if(inputManager_)
        {
            delete inputManager_;
        }
    }
}

void DAO_FrameListener::requestExit()
{
	//boost::mutex::scoped_lock l(lock_);
    exitRequested_ = true;
}

void DAO_FrameListener::exitApplication()
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
bool DAO_FrameListener::frameStarted(const Ogre::FrameEvent& evt)
{
	if (mWindow_->isClosed())
		return false;

	std::cout << "Last Frame: " << evt.timeSinceLastFrame << std::endl;

	//Need to capture/update each device
    inputManager_->getKeyboard()->capture();
    inputManager_->getMouse()->capture();

	//Update the terrain
	renderManager_->frameStarted(evt);

	updateCharacterControl(evt);

	mRaySceneQuery_ = renderManager_->getSceneManager()->createRayQuery(Ogre::Ray());


	// Camera  handling, if Right mouse button is pressed
	//CameraManager& camMgr = CameraManager::getSingleton();
	/*if(inputManager_->getRMouseDown())
	{
		camMgr.getCamera()->yaw(Ogre::Radian(inputManager_->getRotX() * evt.timeSinceLastFrame * -1));
		camMgr.getCamera()->pitch(Ogre::Radian(inputManager_->getRotY() * evt.timeSinceLastFrame * -1));
		inputManager_->setRotX(0.0f);
		inputManager_->setRotY(0.0f);
	}*/

	/*if(mouse_listener_->is_lmb_clicked())
	{
		//find the current mouse position
		CEGUI::Point mousePos = CEGUI::MouseCursor::getSingleton().getPosition();

		//get the event scale;
		std::pair<int,int> scale = mouse_listener_->getMoveScale();

		Ogre::Ray mouseRay = cam_->getCameraToViewportRay(mouse_->getMouseState().X.abs / float(scale.first), mouse_->getMouseState().Y.abs / float(scale.second));


		std::cout << "MouseRay: " << mouseRay.getDirection() << mouseRay.getOrigin() << std::endl;
		ray_scene_query_->setRay(mouseRay);
		ray_scene_query_->setSortByDistance(true);

		Ogre::TerrainGroup::RayResult result = app_->getTerrainGroup()->rayIntersects( mouseRay );

	}*/

	//CameraManager::getSingleton().moveCamera(evt.timeSinceLastFrame);

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

void DAO_FrameListener::updateCharacterControl(const Ogre::FrameEvent& evt)
{
	if (player_)
	{
        player_->update (evt.timeSinceLastFrame, inputManager_->getKeyboard());

        if (cameraControl_)
		{
			//float delta_angle = 0.0f;
			Ogre::Vector4 mouse_data(0.0f, 0.0f, 0.0f, 0.0f);
            switch (mCamMode_)
			{
				// 3rd person chase
				case 1: //CameraControlSystem::CameraMode::ThirdPersonChase:
					mouse_data = inputManager_->getMouseExtraData();
					if(inputManager_->isMouseButtonPressed(OIS::MB_Left) || inputManager_->isMouseButtonPressed(OIS::MB_Right))
					{
						
					}
					else
					{
						mouse_data.x = 0.0f;
						mouse_data.y = 0.0f;
					}

					//If Right MB reset the yaw value
					if(inputManager_->isMouseButtonPressed(OIS::MB_Right))
					{
						mouse_data.x = 0.0f;
					}

				    cameraControl_->update(evt.timeSinceLastFrame,
                                        player_->getCameraNode()->_getDerivedPosition(),
                                        player_->getSightNode()->_getDerivedPosition(),
										mouse_data);
                    break;

				// 3rd person fixed
				case 2: //CameraControlSystem::CameraMode::ThirdPersonFixed:
                    cameraControl_->update(evt.timeSinceLastFrame,
                                        Ogre::Vector3 (0, 200, 0),
                                        player_->getSightNode()->_getDerivedPosition(),
										mouse_data);
                    break;

				// 1st person
				case 3: //CameraControlSystem::CameraMode::FirstPerson:
                    cameraControl_->update(evt.timeSinceLastFrame,
                                        player_->getWorldPosition(),
                                        player_->getSightNode()->_getDerivedPosition(),
										mouse_data);
                    break;
            }
        }
    }
}


bool DAO_FrameListener::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	/*Ogre::Vector3 playerPos = player_->getWorldPosition();
	Ogre::Ray cameraRay(Ogre::Vector3(playerPos.x, 5000.0f, playerPos.z), Ogre::Vector3::NEGATIVE_UNIT_Y);

	mRaySceneQuery_->setRay(cameraRay);

	// Perform the scene query
	mRaySceneQuery_->setSortByDistance(false);
	Ogre::RaySceneQueryResult &result = mRaySceneQuery_->execute();
	Ogre::RaySceneQueryResult::iterator iter = result.begin();

	// Get the results, set the camera height
	for (iter; iter != result.end(); iter++)
	{
		if (iter->worldFragment)
		{
			Ogre::Real terrainHeight = iter->worldFragment->singleIntersection.y;
			LogManager::getSingletonPtr()->logMessage("terrainHeight");
			if ((terrainHeight + 10.0f) > playerPos.y)
			{
				LogManager::getSingletonPtr()->logMessage("set position....");
				player_->getMainNode()->setPosition(playerPos.x, terrainHeight + 10.0f, playerPos.z);
			}
			break;
		}
	}*/

	return true;
}


bool DAO_FrameListener::frameEnded()
{
	return true;
}


/*! \brief Exit the game.
 *
 */
/*bool ODFrameListener::quit(const CEGUI::EventArgs &e)
{
    requestExit();
    return true;
}*/

/*
Ogre::RaySceneQueryResult& DAO_FrameListener::doRaySceneQuery(const OIS::MouseEvent &arg)
{
    // Setup the ray scene query, use CEGUI's mouse position
    CEGUI::Point mousePos = CEGUI::MouseCursor::getSingleton().getPosition();// * mMouseScale;
    Ogre::Ray mouseRay = CameraManager::getSingleton().getCamera()->getCameraToViewportRay(mousePos.d_x / float(
            arg.state.width), mousePos.d_y / float(arg.state.height));
    mRaySceneQuery->setRay(mouseRay);
    mRaySceneQuery->setSortByDistance(true);

    // Execute query
    return mRaySceneQuery->execute();
}*/

