#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

//#include "Ogre/Ogre.h"
#include "OgreSingleton.h"
#include "OgreSceneManager.h"
#include "OgreSceneNode.h"

#include "OGRE/Terrain/OgreTerrain.h"
#include "OGRE/Terrain/OgreTerrainGroup.h"
#include "OGRE/Terrain/OgreTerrainQuadTreeNode.h"
#include "OGRE/Terrain/OgreTerrainMaterialGeneratorA.h"
#include "OGRE/Terrain/OgreTerrainPaging.h"

#include "Ogre/RTShaderSystem/OgreShaderExPerPixelLighting.h"
#include "Ogre/RTShaderSystem/OgreShaderExNormalMapLighting.h"
#include "Ogre/RTShaderSystem/OgreShaderGenerator.h"

#include "CEGUI.h"
#include "RendererModules/Ogre/CEGUIOgreRenderer.h"

#include "DAO_Application.h"
#include "DAO_ResourceManager.h"

#include "LogManager.h"
#include "TerrainManager.h"

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
	void initCEGUI();

private:

	Ogre::Camera* mainCamera_;
    Ogre::SceneManager* sceneManager_;
	TerrainManager* mTerrainManager_;
    Ogre::Viewport* viewport_;
    Ogre::RTShader::ShaderGenerator* shaderGenerator_;
    bool initialized_;

	//TODO: gui will have an own class
	//GUI
	CEGUI::OgreRenderer* gui_renderer_;

	
};

#endif
