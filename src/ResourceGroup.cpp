#include "ResourceGroup.h"


ResourceGroup::ResourceGroup()
{
	groupName_ = "Default";
}

ResourceGroup::ResourceGroup(Ogre::String name)
{
	groupName_ = name;
}

ResourceGroup::~ResourceGroup()
{
}

Ogre::String ResourceGroup::getName()
{
	return groupName_;
}

void ResourceGroup::setName(Ogre::String name)
{
	groupName_ = name;
}

void ResourceGroup::addResource(Resource res)
{
	resourceList_.push_back(res);
}

void ResourceGroup::addResource(Ogre::String type, Ogre::String path)
{
	resourceList_.push_back(Resource(type,path));
}

std::list<Resource>::iterator ResourceGroup::getListBegin()
{
	return resourceList_.begin();
}

std::list<Resource>::iterator ResourceGroup::getListEnd()
{
	return resourceList_.end();
}