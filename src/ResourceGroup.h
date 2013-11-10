#ifndef RESOURCEGROUP_H
#define RESOURCEGROUP_H

#include "Ogre/OgreString.h"

#include "Resource.h"

class ResourceGroup
{
public:
	ResourceGroup();
	ResourceGroup(Ogre::String name);
	~ResourceGroup();

	Ogre::String getName();
	void setName(Ogre::String name);

	void addResource(Resource res);
	void addResource(Ogre::String type, Ogre::String path);

	std::list<Resource>::iterator getListBegin();
	std::list<Resource>::iterator getListEnd();

private:
	Ogre::String groupName_;
	std::list<Resource> resourceList_;

};

#endif
