#include "TerrainDefiner.h"

void TerrainDefiner::define(Ogre::TerrainGroup* terrainGroup, long x, long y)
{
	//TODO: Sometime it tries to load weird huge terrain slots, compensate this for now
	if(x < -10 || x > 10) x = 0;
	if(y < -10 || y > 10) y = 0;

	Ogre::String filename = generateFileName(x, y);
    if(Ogre::ResourceGroupManager::getSingleton().resourceExists(terrainGroup->getResourceGroup(), filename))
    {
        terrainGroup->defineTerrain(x, y);
    }
    else
    {
        Ogre::Image img;
        getTerrainImage(x, y , img);
        terrainGroup->defineTerrain(x, y, &img);
    }
}

void TerrainDefiner::getTerrainImage(long x, long y, Ogre::Image& img)
{
	std::string filename = generateFileName(x, y);
	std::cout << "Looking for heightmap " << filename << std::endl;
    img.load(filename, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
}

std::string TerrainDefiner::generateFileName(long x, long y)
{
	std::stringstream retval;
	retval << "height_map_" << x << "_" << y << ".bmp";
	return retval.str();
}