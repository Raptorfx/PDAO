#ifndef RESOURCE_H
#define RESOURCE_H

#include "Ogre/OgreString.h"

class Resource
{
public:
	Resource();
	Resource(Ogre::String t,Ogre::String p);
	~Resource();

	Ogre::String getType();
	void setType(Ogre::String t);

	Ogre::String getPath();
	void setPath(Ogre::String p);

private:
	Ogre::String type_;
	Ogre::String path_;

};

#endif
