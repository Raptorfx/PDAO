#ifndef TERRAINPAGEPROVIDER_H
#define TERRAINPAGEPROVIDER_H

#include "OGRE/Paging/OgrePageManager.h"
#include "OGRE/Terrain/OgreTerrain.h"
#include "OGRE/Terrain/OgreTerrainGroup.h"
#include "OGRE/Terrain/OgreTerrainQuadTreeNode.h"
#include "OGRE/Terrain/OgreTerrainMaterialGeneratorA.h"
#include "OGRE/Terrain/OgreTerrainPagedWorldSection.h"
#include "OGRE/Terrain/OgreTerrainPaging.h"

class TerrainPageProvider : public Ogre::PageProvider
{
public:
	TerrainPageProvider();
	~TerrainPageProvider();

	bool prepareProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) { return true; }
	bool loadProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) { return true; }
	bool unloadProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) { return true; }
	bool unprepareProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) { return true; }
};

#endif //TERRAINPAGEPROVIDER_H
