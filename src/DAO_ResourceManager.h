#ifndef DAO_RESOURCEMANAGER_H
#define DAO_RESOURCEMANAGER_H

#include "Ogre/OgreSingleton.h"
#include "Ogre/OgreConfig.h"
#include "Ogre/OgreString.h"

#include <map>

#include "DAO_Application.h"
#include "ResourceGroup.h"

class DAO_ResourceManager :	public Ogre::Singleton<DAO_ResourceManager>
{
public:
	DAO_ResourceManager(void);
	~DAO_ResourceManager(void);

	void loadResourceGroup(Ogre::String group);
	void unloadResourceGroup(Ogre::String group);
	void unloadAllResourceGroups();
	void setupResources();
	void takeScreenshot();

	inline const std::string& getResourcePath() const{return resourcePath_;}
    inline const std::string& getHomePath() const{return homePath_;}
    inline const std::string& getPluginsPath() const{return pluginsPath_;}
	inline const std::string& getCfgFile() const{return ogreCfgFile_;}
    inline const std::string& getLogFile() const{return ogreLogFile_;}

private:
	void initResources();

	unsigned int screenshotCounter_;
	std::string resourcePath_;
	std::string homePath_;
	std::string pluginsPath_;
	std::string ogreCfgFile_;
    std::string ogreLogFile_;

	

	std::map<std::string, ResourceGroup> resources_;


	static const std::string PLUGINSCFG;
    static const std::string RESOURCECFG;
	static const std::string CONFIGFILENAME;
    static const std::string LOGFILENAME;
};

#endif
