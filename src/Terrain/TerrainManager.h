#ifndef TERRAIN_MANAGER_H
#define TERRAIN_MANAGER_H

#include "../RenderManager.h"
#include "TerrainPageProvider.h"
#include "TerrainDefiner.h"

#include "OGRE/Terrain/OgreTerrain.h"
#include "OGRE/Terrain/OgreTerrainGroup.h"
#include "OGRE/Terrain/OgreTerrainQuadTreeNode.h"
#include "OGRE/Terrain/OgreTerrainMaterialGeneratorA.h"
#include "OGRE/Terrain/OgreTerrainPagedWorldSection.h"
#include "OGRE/Terrain/OgreTerrainPaging.h"

#define TERRAIN_PAGE_MIN_X -1
#define TERRAIN_PAGE_MIN_Y -1
#define TERRAIN_PAGE_MAX_X 1
#define TERRAIN_PAGE_MAX_Y 1

#define TERRAIN_WORLD_SIZE 12000.0f
#define TERRAIN_SIZE 513
#define HOLD_LOD_DISTANCE 3000.0

//Forward Declarations
class TerrainDefiner;

class TerrainManager
{
public:
	TerrainManager();
	~TerrainManager();
	void createTerrain();

	inline Ogre::TerrainGlobalOptions* getTerrainGlobalOpton()const {return terrain_globals_;}
	inline Ogre::TerrainGroup* getTerrainGroup()const {return terrain_group_;}

	void frameStarted();

private:
	//Terrain
	Ogre::TerrainGlobalOptions* terrain_globals_;
    Ogre::TerrainGroup* terrain_group_;
	bool terrains_imported_;

	Ogre::TerrainPaging* mTerrainPaging;
	Ogre::PageManager* mPageManager;
	TerrainPageProvider* mTerrainPageProvider;
	TerrainDefiner* mTerrainDefiner;
	Ogre::PagedWorld* mPagedWorld;
	Ogre::TerrainPagedWorldSection* mTerrainPagedWorldSection;

	Ogre::SceneManager* sceneManager_;

    void configureTerrainDefaults(Ogre::Light* light);
};


#endif
