#ifndef PLAYERCHARACTER_H
#define PLAYERCHARACTER_H

#include "RenderManager.h"
#include "Character.h"
#include "InputManager.h"
#include "GameContext.h"

#define NUM_ANIMS 2
#define ANIM_FADE_SPEED 7.5f

class InputManager;
class GameContext;

class PlayerCharacter: public Character
{
public:
	PlayerCharacter(Ogre::String name, Ogre::SceneManager *sceneMgr, GameContext* gameContext);
	~PlayerCharacter();

	void update (Ogre::Real elapsedTime, OIS::Keyboard *input);

	void setVisible (bool visible);

protected:

	void processKeyboardInput(Ogre::Real elapsedTime);
	void processMouseInput(Ogre::Real elapsedTime);

	enum AnimID
	{
		ANIM_IDLE_BASE,
		ANIM_WALK,
		ANIM_NONE
	};

	void setupAnimations();
	void setBaseAnimation(AnimID id, bool reset = false);
	void fadeAnimations(Ogre::Real deltaTime);
	void updateAnimations(Ogre::Real deltaTime);


    Ogre::String mName_;
	GameContext* mGameContext;

	//Predefines
	Ogre::Real mDropVelocityIncrement;

	Ogre::Entity* mSword1;
	Ogre::Entity* mSword2;

	Ogre::AnimationState* mAnims[NUM_ANIMS];    // master animation list
	AnimID mBaseAnimID;
	bool mFadingIn[NUM_ANIMS];            // which animations are fading in
	bool mFadingOut[NUM_ANIMS];           // which animations are fading out

	Ogre::Real mTimer;                // general timer to see how long animations have been playing

	bool isJumping_;
	bool isRunning_;
	Ogre::Real mVerticalVelocity;     // for jumping
	Ogre::Real mCurrentGroundPosition;
	Ogre::Real mCurrentDropVelocity;
	Ogre::Real mCharacterMoveSpeed;
	Ogre::Radian mRotationSpeedModificator;

	Ogre::Radian mTargetYaw;
	Ogre::Radian mCurrentYawSpeed;
	Ogre::Radian mMaxYawSpeed;
	Ogre::Radian mYawIncreasePerSecond;

	
};

#endif
