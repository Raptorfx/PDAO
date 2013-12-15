#include "TerrainManager.h"

TerrainManager::TerrainManager(void)
{
}


TerrainManager::~TerrainManager(void)
{
}


void  TerrainManager::createTerrain()
{
	terrain_globals_ = OGRE_NEW Ogre::TerrainGlobalOptions();
	terrain_group_ = OGRE_NEW Ogre::TerrainGroup(RenderManager::getSingletonPtr()->getSceneManager(), Ogre::Terrain::ALIGN_X_Z, TERRAIN_SIZE, TERRAIN_WORLD_SIZE);
	terrain_group_->setFilenameConvention(Ogre::String("BasicTutorial3Terrain"), Ogre::String("dat"));

	Ogre::Vector3 lightdir(0.55f, -0.3f, 0.75f);
    lightdir.normalise();
 
    Ogre::Light* light = RenderManager::getSingletonPtr()->getSceneManager()->createLight("tstLight");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(lightdir);
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue(0.4f, 0.4f, 0.4f));
 
    RenderManager::getSingletonPtr()->getSceneManager()->setAmbientLight(Ogre::ColourValue(0.2f, 0.2f, 0.2f));

	configureTerrainDefaults(light);

	for (long x = 0; x <= 1; ++x)
        for (long y = 0; y <= 1; ++y)
            defineTerrain(x, y);

	terrain_group_->loadAllTerrains(true);

	if (terrains_imported_)
    {
        Ogre::TerrainGroup::TerrainIterator ti = terrain_group_->getTerrainIterator();
        while(ti.hasMoreElements())
        {
            Ogre::Terrain* t = ti.getNext()->instance;
            initBlendMaps(t);
        }
    }

	terrain_group_->freeTemporaryResources();
	RenderManager::getSingletonPtr()->getSceneManager()->setSkyDome(true, "FirstSkyDome", 50.0f, 6.123f);
}

void TerrainManager::defineTerrain(long x, long y)
{
	Ogre::String filename = terrain_group_->generateFilename(x, y);
    if (Ogre::ResourceGroupManager::getSingleton().resourceExists(terrain_group_->getResourceGroup(), filename))
    {
        terrain_group_->defineTerrain(x, y);
    }
    else
    {
        Ogre::Image img;
        getTerrainImage(x, y , img);
        terrain_group_->defineTerrain(x, y, &img);
        terrains_imported_ = true;
    }
}

void TerrainManager::getTerrainImage(long x, long y, Ogre::Image& img)
{
	std::stringstream x_conv;
	x_conv << x;
	std::stringstream y_conv;
	y_conv << y;

	std::cout << "Looking for heightmap height_map_"+x_conv.str()+"_"+y_conv.str()+".bmp" << std::endl;

    img.load("height_map_"+x_conv.str()+"_"+y_conv.str()+".bmp", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
}

void TerrainManager::initBlendMaps(Ogre::Terrain* terrain)
{
	Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
    Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
    Ogre::Real minHeight0 = 140;
    Ogre::Real fadeDist0 = 80;
    Ogre::Real minHeight1 = 140;
    Ogre::Real fadeDist1 = 30;
    float* pBlend0 = blendMap0->getBlendPointer();
    float* pBlend1 = blendMap1->getBlendPointer();
    for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
    {
        for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
        {
            Ogre::Real tx, ty;
 
            blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);

			//TODO: Blending causes Access Violation
            //*pBlend0++ = Ogre::Math::Clamp((terrain->getHeightAtTerrainPosition(tx, ty) - minHeight0) / fadeDist0, Ogre::Real(0), Ogre::Real(1));;
            //*pBlend1++ = Ogre::Math::Clamp((terrain->getHeightAtTerrainPosition(tx, ty) - minHeight1) / fadeDist1, Ogre::Real(0), Ogre::Real(1));

            /*;
            Ogre::Real val = (height - minHeight0) / fadeDist0;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend0++ = val;
 
            val = (height - minHeight1) / fadeDist1;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend1++ = val;*/
        }
    }
    blendMap0->dirty();
    blendMap1->dirty();
    blendMap0->update();
    blendMap1->update();
}

void TerrainManager::configureTerrainDefaults(Ogre::Light* light)
{
	// Configure global
    terrain_globals_->setMaxPixelError(8);
    // testing composite map
    terrain_globals_->setCompositeMapDistance(300);

	// Important to set these so that the terrain knows what to use for derived (non-realtime) data
    terrain_globals_->setLightMapDirection(light->getDerivedDirection());
    terrain_globals_->setCompositeMapAmbient(RenderManager::getSingletonPtr()->getSceneManager()->getAmbientLight());
    terrain_globals_->setCompositeMapDiffuse(light->getDiffuseColour());
	terrain_globals_->setSkirtSize(50.0f);

	// Configure default import settings for if we use imported image
    Ogre::Terrain::ImportData& defaultimp = terrain_group_->getDefaultImportSettings();
    defaultimp.terrainSize = TERRAIN_SIZE;//257;
    defaultimp.worldSize = TERRAIN_WORLD_SIZE;//3600.0f;
    defaultimp.inputScale = 800; // due terrain.png is 8 bpp
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;

	// textures
    defaultimp.layerList.resize(3);
    defaultimp.layerList[0].worldSize = 100;
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
    defaultimp.layerList[1].worldSize = 30;
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
    defaultimp.layerList[2].worldSize = 200;
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
}