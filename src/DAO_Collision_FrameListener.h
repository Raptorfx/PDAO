#ifndef DAO_COLLISION_FRAMELISTENER
#define DAO_COLLISION_FRAMELISTENER

#include "Ogre\Ogre.h"
#include "OgreBulletDynamicsRigidBody.h"

class DAO_Collision_FrameListener : public Ogre::FrameListener
{
private:
	Ogre::RenderWindow* mWin;
	Ogre::Camera* mCam;
	Ogre::SceneManager* mSceneMgr;
    OgreBulletDynamics::DynamicsWorld *mWorld;   // OgreBullet World
    OgreBulletCollisions::DebugDrawer *debugDrawer;
    int mNumEntitiesInstanced;
	int mMoveSpeed;
    std::deque<OgreBulletDynamics::RigidBody *>         mBodies;
    std::deque<OgreBulletCollisions::CollisionShape *>  mShapes;

public:
	DAO_Collision_FrameListener(Ogre::SceneManager *sceneMgr,
                                           Ogre::RenderWindow* win,
                                           Ogre::Camera* cam,
                                           Ogre::Vector3 &gravityVector,
                                           Ogre::AxisAlignedBox &bounds);
	~DAO_Collision_FrameListener();
};

#endif