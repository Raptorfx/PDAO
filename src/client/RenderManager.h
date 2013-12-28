#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "OgreSingleton.h"
#include "OgreSceneManager.h"
#include "OgreSceneNode.h"

#include "Ogre/RTShaderSystem/OgreShaderExPerPixelLighting.h"
#include "Ogre/RTShaderSystem/OgreShaderExNormalMapLighting.h"
#include "Ogre/RTShaderSystem/OgreShaderGenerator.h"

#include "Ogre\OgreFrameListener.h"
#include "Ogre\OgreWindowEventUtilities.h"

#include "DAO_Application.h"
#include "DAO_ResourceManager.h"

#include "LogManager.h"
#include "Terrain/TerrainManager.h"

class TerrainManager;

class RenderManager : public Ogre::Singleton<RenderManager>
{
public:
	RenderManager();
	~RenderManager();

	inline Ogre::Camera* getCamera()const {return mainCamera_;}
    inline Ogre::SceneManager* getSceneManager()const {return sceneManager_;}
	inline TerrainManager* getTerrainManager()const {return mTerrainManager_;}

	void createCamera();
    void createViewports();
    void createScene();

	void frameStarted(const Ogre::FrameEvent& evt);

private:

	Ogre::Camera* mainCamera_;
    Ogre::SceneManager* sceneManager_;
	TerrainManager* mTerrainManager_;
    Ogre::Viewport* viewport_;
    Ogre::RTShader::ShaderGenerator* shaderGenerator_;
    bool initialized_;
};

#endif
