#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include "Ogre\OgreLogManager.h"
#include "Ogre/OgreSingleton.h"

#include "boost\shared_ptr.hpp"
#include "boost\thread\mutex.hpp"

#include "DAO_ResourceManager.h"

class LogManager : public Ogre::Singleton<LogManager>
{
public:
	LogManager(void);
	~LogManager(void);

	void logMessage(const std::string& message, Ogre::LogMessageLevel lml = Ogre::LML_NORMAL, bool maskDebug = false);
    void setLogDetail(Ogre::LoggingLevel ll);
    Ogre::Log& getLog() { return *gameLog_;}
    Ogre::LoggingLevel getLogDetail();

	static const std::string GAMELOG_NAME;

private:

	Ogre::Log* gameLog_;

	boost::mutex lock_;

};

#endif
