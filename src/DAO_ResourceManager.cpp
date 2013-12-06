#include "DAO_ResourceManager.h"

template<> DAO_ResourceManager* Ogre::Singleton<DAO_ResourceManager>::msSingleton = 0;


//#if defined(OD_DEBUG) && OGRE_PLATFORM == OGRE_PLATFORM_WIN32
//On windows, if the application is compiled in debug mode, use the plugins with debug prefix.
const std::string DAO_ResourceManager::PLUGINSCFG = "plugins_d.cfg";
const std::string DAO_ResourceManager::RESOURCECFG = "resources_d.cfg";
//#else
//const std::string DAO_ResourceManager::PLUGINSCFG = "plugins.cfg";
//const std::string DAO_ResourceManager::RESOURCECFG = "resources.cfg";
//#endif

const std::string DAO_ResourceManager::CONFIGFILENAME = "ogre.cfg";
const std::string DAO_ResourceManager::LOGFILENAME = "darkages.log";

DAO_ResourceManager::DAO_ResourceManager(void) :
		screenshotCounter_(0),
        resourcePath_("config/"),
        homePath_("config/"),
        pluginsPath_(""),
		ogreCfgFile_(""),
        ogreLogFile_("")
{
	
	#ifndef OGRE_STATIC_LIB
    pluginsPath_ = resourcePath_ + PLUGINSCFG;
#endif

    ogreCfgFile_ = homePath_ + CONFIGFILENAME;
    ogreLogFile_ = homePath_ + LOGFILENAME;

	initResources();
}

void DAO_ResourceManager::initResources()
{
	ResourceGroup groupBase("Base");
	groupBase.addResource("FileSystem","../media/nvidia");
	groupBase.addResource("FileSystem","../media/heightmaps");
	groupBase.addResource("FileSystem","../media/materials/scripts");
	groupBase.addResource("FileSystem","../media/materials/textures");

	resources_[groupBase.getName()] = groupBase;

	//Meshs
	ResourceGroup groupMesh("Mesh");
	groupMesh.addResource("FileSystem","../media/meshs");
	resources_[groupMesh.getName()] = groupMesh;

	ResourceGroup groupSinbad("Sinbad");
	groupSinbad.addResource("Zip","../media/packs/Sinbad.zip");
	resources_[groupSinbad.getName()] = groupSinbad;
}


DAO_ResourceManager::~DAO_ResourceManager(void)
{
	unloadAllResourceGroups();
}

void DAO_ResourceManager::loadResourceGroup(Ogre::String group)
{
	for(std::list<Resource>::iterator iter = resources_[group].getListBegin(); iter != resources_[group].getListEnd(); iter++)
	{
		if(!Ogre::ResourceGroupManager::getSingleton().resourceLocationExists(iter->getPath(),group))
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(iter->getPath(), iter->getType(), group);
	}

	if(Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(group) && !Ogre::ResourceGroupManager::getSingleton().isResourceGroupInitialised(group))
	{
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(group);
	}

}

void DAO_ResourceManager::unloadResourceGroup(Ogre::String group)
{
	if(Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(group) && Ogre::ResourceGroupManager::getSingleton().isResourceGroupInitialised(group))
	{
		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(group);
	}
}

void DAO_ResourceManager::unloadAllResourceGroups()
{
	for(std::map<std::string, ResourceGroup>::iterator iter = resources_.begin(); iter != resources_.end(); iter++)
	{
		unloadResourceGroup(iter->first);
	}
}

void DAO_ResourceManager::setupResources()
{
	Ogre::ConfigFile cf;
	cf.load(resourcePath_ + RESOURCECFG);

	Ogre::ConfigFile::SectionIterator sectionIter =	cf.getSectionIterator();
	Ogre::String sectionName, typeName, dataname;
	while (sectionIter.hasMoreElements())
	{
		sectionName = sectionIter.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = sectionIter.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			dataname = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(dataname, typeName, sectionName);
		}
	}
	//Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void DAO_ResourceManager::takeScreenshot()
{
    //FIXME: the counter is reset after each start, this overwrites existing pictures
    std::ostringstream ss;
    ss << "DAOscreenshot_" << ++screenshotCounter_ << ".png";
	std::cout << getHomePath() << ss.str() << std::endl;
    DAO_Application::getSingleton().getWindow()->writeContentsToFile(getHomePath() + ss.str());
}