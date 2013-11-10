#ifndef TERRAIN_MANAGER_H
#define TERRAIN_MANAGER_H

#include "RenderManager.h"

#define TERRAIN_PAGE_MIN_X 0
#define TERRAIN_PAGE_MIN_Y 0
#define TERRAIN_PAGE_MAX_X 0
#define TERRAIN_PAGE_MAX_Y 0

#define TERRAIN_WORLD_SIZE 12000.0f
#define TERRAIN_SIZE 513

class TerrainManager
{
public:
	TerrainManager();
	~TerrainManager();
	void createTerrain();

	inline Ogre::TerrainGlobalOptions* getTerrainGlobalOpton()const {return terrain_globals_;}
	inline Ogre::TerrainGroup* getTerrainGroup()const {return terrain_group_;}

private:
	//Terrain
	Ogre::TerrainGlobalOptions* terrain_globals_;
    Ogre::TerrainGroup* terrain_group_;
	bool terrains_imported_;

	Ogre::TerrainPaging* mTerrainPaging;
	Ogre::PageManager* mPageManager;

	class DummyPageProvider : public Ogre::PageProvider
	{
	public:
		bool prepareProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) { return true; }
		bool loadProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) { return true; }
		bool unloadProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) { return true; }
		bool unprepareProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) { return true; }
	};
	DummyPageProvider mDummyPageProvider;

	Ogre::SceneManager* sceneManager_;

	void defineTerrain(long x, long y);
    void initBlendMaps(Ogre::Terrain* terrain);
    void configureTerrainDefaults(Ogre::Light* light);
	void getTerrainImage(long x, long y, Ogre::Image& img);
};


#endif
