#ifndef DAO_APPLICATION
#define DAO_APPLICATION

#include "Ogre/Ogre.h"
#include "Ogre/OgreSingleton.h"
#include "Ogre/OgreErrorDialog.h"
#include "OIS/OIS.h"

#include "DAO_ResourceManager.h"
#include "LogManager.h"

#include "DAO_FrameListener.h"
#include "GlobalValueStore.h"

class DAO_FrameListener;

class DAO_Application : public Ogre::Singleton<DAO_Application>
{
public:
	DAO_Application();
	~DAO_Application();

	int startup();

	//Getters
	inline Ogre::RenderWindow* getWindow()const {return window_;}
	inline Ogre::Root* getRoot()const {return root_;}

	static void displayErrorMessage(const std::string& message, bool log = true);

	static const double DEFAULT_FRAMES_PER_SECOND;
    static double MAX_FRAMES_PER_SECOND;

private:
	void mainLoop();

	void cleanUp();

	Ogre::Root* root_;
	DAO_FrameListener* frame_listener_;
	Ogre::RenderWindow* window_;

	bool _keepRunning;

};

#endif /* DAO_APPLICATION */
