#ifndef CAMERACONTROLSYSTEM_H
#define CAMERACONTROLSYSTEM_H

#include "Ogre/Ogre.h"

#include "InputManager.h"
#include "Terrain/TerrainManager.h"

class CameraControlSystem
{
public:

	enum CameraMode {
		FirstPerson = 0,
		ThirdPerson,
		Orthagonal
	};

	CameraControlSystem(Ogre::String name, Ogre::SceneManager *sceneMgr, Ogre::Camera *camera = 0);
	~CameraControlSystem();

	inline void setTightness(Ogre::Real tightness){mTightness_ = tightness;}
	inline Ogre::Real getTightness(){return mTightness_;}

	Ogre::Vector3 getCameraPosition();
	void instantUpdate(Ogre::Vector3 cameraPosition, Ogre::Vector3 targetPosition);
	void update(Ogre::Real elapsedTime, Ogre::Vector3 cameraPosition, Ogre::Vector3 targetPosition, Ogre::Vector4 mouse_data);
	void updateYaw(Ogre::Radian yaw);
	Ogre::Quaternion getTargetNodeOrientation(){return mTargetNode_->getOrientation();}
	void updateOrientation(Ogre::Quaternion orient);

	void setTargetNode(Ogre::SceneNode * target);
	void removeTargetNode(Ogre::SceneNode * target);

private:

	void update3rdPerson(Ogre::Real elapsedTime, Ogre::Vector3 cameraPosition, Ogre::Vector3 targetPosition, Ogre::Vector4 mouse_data);
	void update1stPerson(Ogre::Real elapsedTime, Ogre::Vector3 cameraPosition, Ogre::Vector3 targetPosition, Ogre::Vector4 mouse_data);
	void updateOrthagonal(Ogre::Real elapsedTime, Ogre::Vector3 cameraPosition, Ogre::Vector3 targetPosition, Ogre::Vector4 mouse_data);

	// The camera target
    Ogre::SceneNode *mTargetNode_;
	// The camera itself
    Ogre::SceneNode *mCameraNode_;
	//Store the original parent node of the target
	Ogre::SceneNode *mTargettingNode_;
	Ogre::SceneNode *mInitialTargetParent_;


	// Ogre camera
    Ogre::Camera *mCamera_;

	Ogre::SceneManager *mSceneMgr_;
    Ogre::String mName_;

 
	// To know if the ogre camera binded has been created outside or inside of this class
    bool mOwnCamera_;
 
	// Determines the movement of the camera - 1 means tight movement, while 0 means no movement
    Ogre::Real mTightness_;
	Ogre::Real mCameraGroundDistance_;
	Ogre::Real mCurrentGroundPosition;
	Ogre::Real mMaximumCameraDistance_;
	Ogre::Real mMinimumCameraDistance_;
	Ogre::Real mCurrentCameraDistance_;
	Ogre::Real mDesiredCameraDistance_;
	Ogre::Real mMinimumCameraGroundDistance_;

	CameraMode mCurrentCamMode_;
};

#endif
