#include "GlobalValueStore.h"

template<> GlobalValueStore* Ogre::Singleton<GlobalValueStore>::msSingleton = 0;

GlobalValueStore::GlobalValueStore(void)
{
}


GlobalValueStore::~GlobalValueStore(void)
{
}

void GlobalValueStore::setStdString(std::string key, std::string value)
{
	boost::mutex::scoped_lock l(mStdStringLock_);
	mStdStringMap_[key] = value;
}

std::string GlobalValueStore::getStdString(std::string key)
{
	boost::mutex::scoped_lock l(mStdStringLock_);
	return mStdStringMap_[key];
}
