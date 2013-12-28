#include "LogManager.h"

template<> LogManager* Ogre::Singleton<LogManager>::msSingleton = 0;

const std::string LogManager::GAMELOG_NAME = "dao_game.log";

LogManager::LogManager(void)
{
	boost::mutex::scoped_lock l(lock_);

	gameLog_ = Ogre::LogManager::getSingleton().createLog(
        DAO_ResourceManager::getSingleton().getHomePath() + GAMELOG_NAME);
}


LogManager::~LogManager(void)
{
}



void LogManager::logMessage(const std::string& message, Ogre::LogMessageLevel lml,
                    bool maskDebug)
{
	boost::mutex::scoped_lock l(lock_);

	gameLog_->logMessage(message, lml, maskDebug);
}

/*! \brief Set the log detail level.
 *
 */
void LogManager::setLogDetail(Ogre::LoggingLevel ll)
{
	boost::mutex::scoped_lock l(lock_);

	gameLog_->setLogDetail(ll);
}

Ogre::LoggingLevel LogManager::getLogDetail()
{
	boost::mutex::scoped_lock l(lock_);

    Ogre::LoggingLevel ret;
	ret = gameLog_->getLogDetail();
    return ret;
}