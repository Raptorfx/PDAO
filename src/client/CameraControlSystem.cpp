#include "CameraControlSystem.h"

CameraControlSystem::CameraControlSystem(Ogre::String name, Ogre::SceneManager *sceneMgr, Ogre::Camera *camera) :
        mName_(name),
        mSceneMgr_(sceneMgr)
{
	mCurrentCamMode_ = ThirdPerson;

	// Basic member references setup
    // Create the camera's node structure
	mTargetNode_ = mSceneMgr_->getRootSceneNode()->createChildSceneNode(mName_ + "_target");
	mTargetNode_->setInheritOrientation(false);
	//Yaw the node so it is aligned with the camera node befor attaching
	mCameraNode_ = mTargetNode_->createChildSceneNode(mName_);
	mTargetNode_->yaw(Ogre::Radian(3.0f), Ogre::Node::TS_WORLD);

	mCameraNode_->translate(0.0f, 0.0f, -20.0f);


	mMaximumCameraDistance_ = -200.0f;
	mMinimumCameraDistance_ = -20.0f;
	mCurrentCameraDistance_ = -20.0f;
	mDesiredCameraDistance_ = -20.0f;
	mMinimumCameraGroundDistance_ = 0.3f;
	
 
    // Create our camera if it wasn't passed as a parameter
    if (camera == 0) 
	{
        mCamera_ = mSceneMgr_->createCamera(mName_);
        mOwnCamera_ = true;
    }
    else
	{
        mCamera_ = camera;
        // just to make sure that mCamera is set to 'origin' (same position as the mCameraNode)
        mCamera_->setPosition(0.0,0.0,0.0);
        mOwnCamera_ = false;
    }

    // ... and attach the Ogre camera to the camera node
	mCameraNode_->setDirection(mTargetNode_->getPosition());
    mCameraNode_->attachObject(mCamera_);
	mCamera_->lookAt(mTargetNode_->getPosition());
 
    // Default tightness
    mTightness_ = 1.0f;//0.01f;
}


CameraControlSystem::~CameraControlSystem(void)
{
	mCameraNode_->detachAllObjects ();
    if (mOwnCamera_)
        delete mCamera_;
    mSceneMgr_->destroySceneNode (mName_);
    mSceneMgr_->destroySceneNode (mName_ + "_target");
}

Ogre::Vector3 CameraControlSystem::getCameraPosition () 
{
    return mTargetNode_->getPosition ();
}

void CameraControlSystem::updateOrientation(Ogre::Quaternion orient)
{
	mTargetNode_->setOrientation(orient);
}
 
void CameraControlSystem::instantUpdate (Ogre::Vector3 cameraPosition, Ogre::Vector3 targetPosition) 
{
    mTargetNode_->setPosition (targetPosition);
}
 
void CameraControlSystem::update(Ogre::Real elapsedTime, Ogre::Vector3 cameraPosition, Ogre::Vector3 targetPosition, Ogre::Vector4 mouse_data)
{
    // Handle movement
    Ogre::Vector3 displacement;
 
	displacement = (targetPosition - mTargetNode_->getPosition ());// * elapsedTime;
    mTargetNode_->translate (displacement);

	switch(mCurrentCamMode_)
	{
		case(ThirdPerson):
			update3rdPerson(elapsedTime, cameraPosition, targetPosition, mouse_data);
			break;

		case(FirstPerson):
			update1stPerson(elapsedTime, cameraPosition, targetPosition, mouse_data);
			break;

		case(Orthagonal):
			updateOrthagonal(elapsedTime, cameraPosition, targetPosition, mouse_data);
			break;
	}
}

void CameraControlSystem::update3rdPerson(Ogre::Real elapsedTime, Ogre::Vector3 cameraPosition, Ogre::Vector3 targetPosition, Ogre::Vector4 mouse_data) 
{
	mCurrentCameraDistance_ = mCameraNode_->getPosition().z;

	if(mouse_data.w != 0.0f)
	{
		mDesiredCameraDistance_ += mouse_data.w;

		if(mDesiredCameraDistance_ < mMaximumCameraDistance_)
			mDesiredCameraDistance_ = mMaximumCameraDistance_;
		else if(mDesiredCameraDistance_ > mMinimumCameraDistance_)
			mDesiredCameraDistance_ = mMinimumCameraDistance_;
	}

	if(mCurrentCameraDistance_ != mDesiredCameraDistance_)
	{
		Ogre::Real diffDistance = mDesiredCameraDistance_ - mCurrentCameraDistance_;
		Ogre::Real moveDistance = diffDistance * (elapsedTime*2);

		if( ( diffDistance > 0.0f && diffDistance < 0.01f ) || (diffDistance < 0.0f && diffDistance > -0.01f))
			moveDistance = mDesiredCameraDistance_ - mCurrentCameraDistance_;

		Ogre::Vector3 newPosition = mCameraNode_->getPosition();
		newPosition.z += moveDistance;
		mCameraNode_->setPosition(newPosition);

		mCameraNode_->setDirection(mTargetNode_->getPosition());
	}

	if(mCurrentCameraDistance_ == mMinimumCameraDistance_ && mouse_data.w > 0.0f)
	{
		mCurrentCamMode_ = FirstPerson;
		mCameraNode_->setPosition(mTargetNode_->getPosition());
		mCameraNode_->setOrientation(mTargetNode_->getOrientation());

		return;
	}

	Ogre::Vector3 cameraWorldPosition = mCameraNode_->convertLocalToWorldPosition(mCamera_->getPosition());

	//Get the position of the ground below the camera
	Ogre::TerrainGroup* terrain = RenderManager::getSingletonPtr()->getTerrainManager()->getTerrainGroup();
	Ogre::Vector3 ray_cast_position = cameraWorldPosition;
	ray_cast_position.y += 1000.0f;
	Ogre::Ray queryRay(ray_cast_position, Ogre::Vector3::NEGATIVE_UNIT_Y);
	Ogre::TerrainGroup::RayResult result = terrain->rayIntersects(queryRay);
	if(result.hit) {
		mCurrentGroundPosition = result.position.y;
	}

	if(cameraWorldPosition.y >= mCurrentGroundPosition + mMinimumCameraGroundDistance_ )
	{
		if(mouse_data.y != 0.0f)
		{
			Ogre::Radian newPitch = mTargetNode_->getOrientation().getPitch() + Ogre::Radian(mouse_data.y);

			//Check if we can rotate
			if(newPitch < Ogre::Radian(-1.40f))
			{
				mTargetNode_->pitch(Ogre::Radian(1.40f)+mTargetNode_->getOrientation().getPitch());
			}
			else if(newPitch > Ogre::Radian(1.40f))
			{
				mTargetNode_->pitch(Ogre::Radian(1.40f)-mTargetNode_->getOrientation().getPitch());
			}
			else
			{
				mTargetNode_->pitch(Ogre::Radian(mouse_data.y));
			}
		}
		

		//Restore Y cam node translation
		if(mCameraNode_->getPosition().y > mMinimumCameraGroundDistance_)
		{
			Ogre::Real groundDiff = mCurrentGroundPosition + mMinimumCameraGroundDistance_ - cameraWorldPosition.y;
			mCameraNode_->translate(0.0f, groundDiff, 0.0f, Ogre::Node::TS_WORLD);
		}

	}
	else if(cameraWorldPosition.y == mCurrentGroundPosition + mMinimumCameraGroundDistance_)
	{
		if(mouse_data.y > 0.0f)
		{
			Ogre::Radian newPitch = mTargetNode_->getOrientation().getPitch() + Ogre::Radian(mouse_data.y);

			//Check if we can rotate
			if(newPitch < Ogre::Radian(-1.40f))
			{
				mTargetNode_->pitch(Ogre::Radian(1.40f)+mTargetNode_->getOrientation().getPitch());
			}
			else if(newPitch > Ogre::Radian(1.40f))
			{
				mTargetNode_->pitch(Ogre::Radian(1.40f)-mTargetNode_->getOrientation().getPitch());
			}
			else
			{
				mTargetNode_->pitch(Ogre::Radian(mouse_data.y));
			}
		}
	}
	else
	{
		Ogre::Real groundDiff = mCurrentGroundPosition + mMinimumCameraGroundDistance_ - cameraWorldPosition.y;
		mCameraNode_->translate(0.0f, groundDiff, 0.0f, Ogre::Node::TS_WORLD);
	}

	//Fix the pitch if needed
	if(mTargetNode_->getOrientation().getPitch() > Ogre::Radian(1.40f))
		mTargetNode_->pitch(Ogre::Radian(-0.01f));

	if(mouse_data.x != 0.0f)
		mTargetNode_->yaw(Ogre::Radian(mouse_data.x * -2), Ogre::Node::TS_WORLD);

	mCamera_->lookAt(mTargetNode_->getPosition());
}

void CameraControlSystem::update1stPerson(Ogre::Real elapsedTime, Ogre::Vector3 cameraPosition, Ogre::Vector3 targetPosition, Ogre::Vector4 mouse_data)
{
	if(mouse_data.w < 0.0f)
	{
		mCurrentCamMode_ = ThirdPerson;
		mCameraNode_->setPosition(0.0f, 10.0f, -20.0f);
		return;
	}

	if(mouse_data.y != 0.0f)
			mTargetNode_->pitch(Ogre::Radian(mouse_data.y));
}

void CameraControlSystem::updateOrthagonal(Ogre::Real elapsedTime, Ogre::Vector3 cameraPosition, Ogre::Vector3 targetPosition, Ogre::Vector4 mouse_data)
{

}

void CameraControlSystem::updateYaw(Ogre::Radian yaw)
{
	mTargetNode_->yaw(yaw, Ogre::Node::TS_WORLD);
}

void CameraControlSystem::setTargetNode(Ogre::SceneNode * target)
{
	mInitialTargetParent_ = mTargetNode_->getParentSceneNode();
	mTargettingNode_ = target;
}

void CameraControlSystem::removeTargetNode(Ogre::SceneNode * target)
{
	mTargettingNode_ = mInitialTargetParent_;
}
