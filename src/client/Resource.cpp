#include "Resource.h"


Resource::Resource()
{
	type_ = "";
	path_ = "";
}

Resource::Resource(Ogre::String t, Ogre::String p)
{
	type_ = t;
	path_ = p;
}


Resource::~Resource()
{
}


Ogre::String Resource::getType()
{
	return type_;
}

void Resource::setType(Ogre::String t)
{
	type_ = t;
}

Ogre::String Resource::getPath()
{
	return path_;
}

void Resource::setPath(Ogre::String p)
{
	path_ = p;
}