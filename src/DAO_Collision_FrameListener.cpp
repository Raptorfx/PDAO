#include "DAO_Collision_FrameListener.h"


DAO_Collision_FrameListener::DAO_Collision_FrameListener(Ogre::SceneManager *sceneMgr,
                                           Ogre::RenderWindow* win,
                                           Ogre::Camera* cam,
                                           Ogre::Vector3 &gravityVector,
                                           Ogre::AxisAlignedBox &bounds)
{

	mMoveSpeed = 50;   // defined in ExampleFrameListener
    mNumEntitiesInstanced = 0; // how many shapes are created
    mSceneMgr = sceneMgr;
	mCam = cam;
	mWin = win;
    // Start Bullet
    mWorld = new OgreBulletDynamics::DynamicsWorld(mSceneMgr, bounds, gravityVector);
    // add Debug info display tool
    debugDrawer = new OgreBulletCollisions::DebugDrawer();
    debugDrawer->setDrawWireframe(true);   // we want to see the Bullet containers
    mWorld->setDebugDrawer(debugDrawer);
    mWorld->setShowDebugShapes(true);      // enable it if you want to see the Bullet containers
    Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("debugDrawer", Ogre::Vector3::ZERO);
    node->attachObject(static_cast <Ogre::SimpleRenderable *> (debugDrawer));

}


DAO_Collision_FrameListener::~DAO_Collision_FrameListener()
{
	delete mWorld->getDebugDrawer();
    mWorld->setDebugDrawer(0);
    delete mWorld;
}
