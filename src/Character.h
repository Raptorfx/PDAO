#ifndef CHARACTER_H
#define CHARACTER_H

#include "Ogre/Ogre.h"
#include "OIS/OIS.h"

class Character
{
public:
	// Updates the character (movement...)
    virtual void update (Ogre::Real elapsedTime, OIS::Keyboard *input) = 0;
    // The three methods below returns the two camera-related nodes, 
    // and the current position of the character (for the 1st person camera)
    inline Ogre::SceneNode *getSightNode() {return mSightNode_;}
    inline Ogre::SceneNode *getCameraNode() {return mCameraNode_;}
	inline Ogre::SceneNode *getMainNode() {return mMainNode_;}

    Ogre::Vector3 getWorldPosition() {return mMainNode_->_getDerivedPosition();}

protected:
	// Main character node
    Ogre::SceneNode *mMainNode_;
	// "Sight" node - The character is supposed to be looking here
    Ogre::SceneNode *mSightNode_;
	// Node for the chase camera
    Ogre::SceneNode *mCameraNode_;
	// Node for attaching the character
	Ogre::SceneNode *mCharacterNode_;
	// Character entity
    Ogre::Entity *mEntity_;
    Ogre::SceneManager *mSceneMgr_;


};

#endif
