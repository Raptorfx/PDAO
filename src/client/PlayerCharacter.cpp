#include "PlayerCharacter.h"


PlayerCharacter::PlayerCharacter(Ogre::String name, Ogre::SceneManager *sceneMgr, GameContext* gameContext)
{
	// Setup basic member references
    mName_ = name;
    mSceneMgr_ = sceneMgr;
	mGameContext = gameContext;
 
    // Setup basic node structure to handle 3rd person cameras
    mMainNode_ = mSceneMgr_->getRootSceneNode ()->createChildSceneNode (mName_, Ogre::Vector3 (0, 1000, 0));
	mCharacterNode_ = mMainNode_->createChildSceneNode (mName_ + "_character", Ogre::Vector3 (0, 0, 0));
    mSightNode_ = mMainNode_->createChildSceneNode (mName_ + "_sight", Ogre::Vector3 (0, 8, 0));
    mCameraNode_ = mMainNode_->createChildSceneNode (mName_ + "_camera", Ogre::Vector3 (0, 0, 0));

	//Set the orientation of the camera system
	mGameContext->getCameraControl()->updateOrientation(mMainNode_->getOrientation());
 
    // Give this character a shape :)
    mEntity_ = mSceneMgr_->createEntity (mName_, "Stickman.mesh");
    mCharacterNode_->attachObject (mEntity_);
	mCharacterNode_->scale(1.0f, 1.0f, 1.0f);
	mEntity_->setMaterialName("Examples/Robot");

	mSword1 = sceneMgr->createEntity("SinbadSword1", "Sword.mesh");
	mSword2 = sceneMgr->createEntity("SinbadSword2", "Sword.mesh");
	mEntity_->attachObjectToBone("Sheat.L", mSword1);
	mEntity_->attachObjectToBone("Sheat.R", mSword2);
	mEntity_->setCastShadows(true);

	mVerticalVelocity = 0;
	isJumping_ = false;
	isRunning_ = false;

	mDropVelocityIncrement = 80.0f;
	mCurrentDropVelocity = 0.0f;
	mCharacterMoveSpeed = 30.0f;
	mRotationSpeedModificator = Ogre::Radian(1.0f); 

	mTargetYaw = Ogre::Radian(0.0f);

	setupAnimations();
}

PlayerCharacter::~PlayerCharacter()
{
	mMainNode_->detachAllObjects();
    delete mEntity_;
    mMainNode_->removeAndDestroyAllChildren();
    mSceneMgr_->destroySceneNode(mName_);
}

void PlayerCharacter::setupAnimations()
{
	// this is very important due to the nature of the exported animations
	mEntity_->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

	Ogre::String animNames[] =
	{"BaseIdle", "walk"};

	// populate our animation list
	for (int i = 0; i < NUM_ANIMS; i++)
	{
		mAnims[i] = mEntity_->getAnimationState(animNames[i]);
		mAnims[i]->setLoop(true);
		mFadingIn[i] = false;
		mFadingOut[i] = false;
	}

	// start off in the idle state (top and bottom together)
	setBaseAnimation(ANIM_IDLE_BASE);
}

void PlayerCharacter::setBaseAnimation(AnimID id, bool reset)
{
	if (mBaseAnimID >= 0 && mBaseAnimID < NUM_ANIMS)
	{
		// if we have an old animation, fade it out
		mFadingIn[mBaseAnimID] = false;
		mFadingOut[mBaseAnimID] = true;
	}

	mBaseAnimID = id;

	if (id != ANIM_NONE)
	{
		// if we have a new animation, enable it and fade it in
		mAnims[id]->setEnabled(true);
		mAnims[id]->setWeight(0);
		mFadingOut[id] = false;
		mFadingIn[id] = true;
		if (reset) mAnims[id]->setTimePosition(0);
	}
}

void PlayerCharacter::updateAnimations(Ogre::Real deltaTime)
{
	Ogre::Real baseAnimSpeed = 2.5f;

	mTimer += deltaTime;

	// increment the current base and top animation times
	if (mBaseAnimID != ANIM_NONE) mAnims[mBaseAnimID]->addTime(deltaTime * baseAnimSpeed);

	// apply smooth transitioning between our animations
	fadeAnimations(deltaTime);
}

void PlayerCharacter::fadeAnimations(Ogre::Real deltaTime)
{
	for (int i = 0; i < 2; i++)
	{
		if (mFadingIn[i])
		{
			// slowly fade this animation in until it has full weight
			Ogre::Real newWeight = mAnims[i]->getWeight() + deltaTime * ANIM_FADE_SPEED;
			mAnims[i]->setWeight(Ogre::Math::Clamp<Ogre::Real>(newWeight, 0, 1));
			if (newWeight >= 1) mFadingIn[i] = false;
		}
		else if (mFadingOut[i])
		{
			// slowly fade this animation out until it has no weight, and then disable it
			Ogre::Real newWeight = mAnims[i]->getWeight() - deltaTime * ANIM_FADE_SPEED;
			mAnims[i]->setWeight(Ogre::Math::Clamp<Ogre::Real>(newWeight, 0, 1));
			if (newWeight <= 0)
			{
				mAnims[i]->setEnabled(false);
				mFadingOut[i] = false;
			}
		}
	}
}

void PlayerCharacter::update(Ogre::Real elapsedTime, OIS::Keyboard *input) 
{

	processKeyboardInput(elapsedTime);
	processMouseInput(elapsedTime);

	//Get the position of the ground below the player
	Ogre::TerrainGroup* terrain = RenderManager::getSingletonPtr()->getTerrainManager()->getTerrainGroup();
		
	//Store the position to increase it, to guarantuee a hit on the terrain
	Ogre::Vector3 main_position = mMainNode_->getPosition();
	Ogre::Vector3 ray_cast_position = main_position;
	ray_cast_position.y += 1000.0f;

	// Setup the query ray
	Ogre::Ray queryRay(ray_cast_position, Ogre::Vector3::NEGATIVE_UNIT_Y);

	// Perform the scene query
	Ogre::TerrainGroup::RayResult result = terrain->rayIntersects(queryRay);

	if(result.hit) {
		mCurrentGroundPosition = result.position.y;
	}


	//Process jumping
	if(isJumping_)
	{
		// if we're jumping, add a vertical offset too, and apply gravity
		mMainNode_->translate(0, mCurrentDropVelocity * elapsedTime, 0, Ogre::Node::TS_LOCAL);
		mCurrentDropVelocity -= mDropVelocityIncrement * elapsedTime;
			
		Ogre::Vector3 pos = mMainNode_->getPosition();
		if (pos.y <= mCurrentGroundPosition)
		{
			// if we've hit the ground, change to landing state
			pos.y = mCurrentGroundPosition;
			mMainNode_->setPosition(pos);
			isJumping_ = false;
			mCurrentDropVelocity = 0.0f;
		}
	}

	//Upate the player height to the world if not jumping
	if(!isJumping_)
	{
		if(main_position.y <= mCurrentGroundPosition)
		{
			main_position.y = mCurrentGroundPosition;
			mMainNode_->setPosition(main_position);
			mCurrentDropVelocity = 0.0f;
		}
		else
		{
			mCurrentDropVelocity += mDropVelocityIncrement * elapsedTime;
			main_position.y = main_position.y - mCurrentDropVelocity;

			//Check if we are already below the ground
			if(main_position.y < mCurrentGroundPosition)
				main_position.y = mCurrentGroundPosition;
			mMainNode_->setPosition(main_position);
		}
	}

	if (isRunning_ && mBaseAnimID == ANIM_IDLE_BASE)
	{
		// start running if not already moving and the player wants to move
		setBaseAnimation(ANIM_WALK, true);
	}

	if(!isRunning_ && mBaseAnimID == ANIM_WALK)
	{
		setBaseAnimation(ANIM_IDLE_BASE);
	}

	updateAnimations(elapsedTime);
}

void PlayerCharacter::processKeyboardInput(Ogre::Real elapsedTime)
{
	if (InputManager::getSingleton().isKeyPressed(OIS::KC_W)) 
	{
        mMainNode_->translate (mMainNode_->getOrientation () * Ogre::Vector3 (0, 0, mCharacterMoveSpeed * elapsedTime));
		isRunning_ = true;
    }
    if (InputManager::getSingleton().isKeyPressed(OIS::KC_S)) 
	{
        mMainNode_->translate (mMainNode_->getOrientation () * Ogre::Vector3 (0, 0, -mCharacterMoveSpeed * elapsedTime));
		isRunning_ = true;
    }
    if (InputManager::getSingleton().isKeyPressed(OIS::KC_A)) 
	{
		if(InputManager::getSingleton().isMouseButtonPressed(OIS::MB_Right))
			mMainNode_->translate (mMainNode_->getOrientation () * Ogre::Vector3 (mCharacterMoveSpeed * elapsedTime, 0, 0));
		else
			mMainNode_->yaw(Ogre::Radian(elapsedTime * (mRotationSpeedModificator / 2)  ));
		isRunning_ = true;
    }
    if (InputManager::getSingleton().isKeyPressed(OIS::KC_D)) 
	{
		if(InputManager::getSingleton().isMouseButtonPressed(OIS::MB_Right))
			mMainNode_->translate (mMainNode_->getOrientation () * Ogre::Vector3 (-mCharacterMoveSpeed * elapsedTime, 0, 0));
		else
			mMainNode_->yaw(Ogre::Radian(-elapsedTime * (mRotationSpeedModificator / 2) ));
		isRunning_ = true;
    }

	if (InputManager::getSingleton().isKeyPressed(OIS::KC_SPACE)) {
		if(!isJumping_)
		{
			mCurrentDropVelocity = 30.0f;
			isJumping_ = true;
		}
    }

	if(!InputManager::getSingleton().isKeyPressed(OIS::KC_W) &&
		!InputManager::getSingleton().isKeyPressed(OIS::KC_A) &&
		!InputManager::getSingleton().isKeyPressed(OIS::KC_S) &&
		!InputManager::getSingleton().isKeyPressed(OIS::KC_D))
		isRunning_ = false;
}
 
void PlayerCharacter::processMouseInput(Ogre::Real elapsedTime)
{
	if(InputManager::getSingleton().isMouseButtonPressed(OIS::MB_Right))
	{
		Ogre::Radian newYaw(InputManager::getSingleton().getMouseExtraData().x * elapsedTime * -300.0f);

		//Also update the camera if the right mouse button is pressed
		mGameContext->getCameraControl()->updateYaw(newYaw);

		Ogre::Radian cameraYaw = mGameContext->getCameraControl()->getTargetNodeOrientation().getYaw();
		Ogre::Radian yawDifference = cameraYaw - mMainNode_->getOrientation().getYaw();

		//Prevent a 359,99..° turn
		if(yawDifference < Ogre::Radian(-3.0f))
		{
			cameraYaw += Ogre::Radian(6.0f);
			yawDifference = cameraYaw - mMainNode_->getOrientation().getYaw();
		}
		else if(yawDifference > Ogre::Radian(3.0f))
		{
			cameraYaw -= Ogre::Radian(6.0f);
			yawDifference = cameraYaw - mMainNode_->getOrientation().getYaw();
		}

		//Slow down the roatation
		if(yawDifference > Ogre::Radian(0.01f) || yawDifference < Ogre::Radian(-0.01f))
			yawDifference = yawDifference * elapsedTime;

		//Limit the rotation to a maximum
		if( yawDifference > mRotationSpeedModificator)
			yawDifference = mRotationSpeedModificator;
		else if( yawDifference < -mRotationSpeedModificator)
			yawDifference = -mRotationSpeedModificator;

		//Position the player slowly to the camera direction
		mMainNode_->yaw(yawDifference);
	}
}

// Change visibility - Useful for 1st person view ;)
void PlayerCharacter::setVisible (bool visible) 
{
    mMainNode_->setVisible (visible);
}
