#ifndef TERRAINDEFINER_H
#define TERRAINDEFINER_H

#include "../RenderManager.h"

#include "OGRE/Terrain/OgreTerrainGroup.h"
#include "OGRE/Paging/OgrePagedWorldSection.h"

class TerrainDefiner : public Ogre::TerrainPagedWorldSection::TerrainDefiner
{
public:
	void define(Ogre::TerrainGroup* terrainGroup, long x, long y);
	void getTerrainImage(long x, long y, Ogre::Image& img);
	std::string generateFileName(long x, long y);
};


#endif //TERRAINDEFINER_H