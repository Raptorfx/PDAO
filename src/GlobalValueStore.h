#ifndef GLOBALVALUESTORE_H
#define GLOBALVALUESTORE_H

#include "OgreSingleton.h"

//#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <String>
#include <Map>

class GlobalValueStore : public Ogre::Singleton<GlobalValueStore>
{
public:
	GlobalValueStore(void);
	~GlobalValueStore(void);

	void setStdString(std::string key, std::string value);
	std::string getStdString(std::string key);


private:

	std::map<std::string, std::string> mStdStringMap_;
	boost::mutex mStdStringLock_;

};



#endif
