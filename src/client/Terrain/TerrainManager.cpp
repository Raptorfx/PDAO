#include "TerrainManager.h"

TerrainManager::TerrainManager(void)
{
}


TerrainManager::~TerrainManager(void)
{
	OGRE_DELETE terrain_globals_;
	OGRE_DELETE terrain_group_;
}


void  TerrainManager::createTerrain()
{
	terrain_globals_ = OGRE_NEW Ogre::TerrainGlobalOptions();

	Ogre::Vector3 lightdir(0.55f, -0.3f, 0.75f);
    lightdir.normalise();
 
    Ogre::Light* light = RenderManager::getSingletonPtr()->getSceneManager()->createLight("tstLight");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(lightdir);
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue(0.4f, 0.4f, 0.4f));
 
    RenderManager::getSingletonPtr()->getSceneManager()->setAmbientLight(Ogre::ColourValue(0.2f, 0.2f, 0.2f));

	mTerrainDefiner = OGRE_NEW TerrainDefiner();
	mTerrainPageProvider = OGRE_NEW TerrainPageProvider();

	terrain_group_ = OGRE_NEW Ogre::TerrainGroup(RenderManager::getSingletonPtr()->getSceneManager(), Ogre::Terrain::ALIGN_X_Z, TERRAIN_SIZE, TERRAIN_WORLD_SIZE);
	terrain_group_->setFilenameConvention(Ogre::String("BasicTutorial3Terrain"), Ogre::String("dat"));
	terrain_group_->setOrigin(Ogre::Vector3(0,0,0));
	terrain_group_->setAutoUpdateLod( Ogre::TerrainAutoUpdateLodFactory::getAutoUpdateLod(Ogre::BY_DISTANCE) );

	configureTerrainDefaults(light);

	terrain_group_->loadAllTerrains(true);

	mPageManager = OGRE_NEW Ogre::PageManager();
	mPageManager->setPageProvider(mTerrainPageProvider);
	mPageManager->addCamera(RenderManager::getSingletonPtr()->getCamera());
	mPageManager->setDebugDisplayLevel(0);
	mTerrainPaging = OGRE_NEW Ogre::TerrainPaging(mPageManager);
	mPagedWorld = mPageManager->createWorld();
	mTerrainPagedWorldSection = mTerrainPaging->createWorldSection(mPagedWorld, terrain_group_, 400, 500, 
		TERRAIN_PAGE_MIN_X, TERRAIN_PAGE_MIN_Y, 
		TERRAIN_PAGE_MAX_X, TERRAIN_PAGE_MAX_Y);

	mTerrainPagedWorldSection->setDefiner( mTerrainDefiner );

	terrain_group_->freeTemporaryResources();
	RenderManager::getSingletonPtr()->getSceneManager()->setSkyDome(true, "FirstSkyDome", 50.0f, 6.123f);
}

void TerrainManager::configureTerrainDefaults(Ogre::Light* light)
{
	// Configure global
    terrain_globals_->setMaxPixelError(8);
    // testing composite map
    terrain_globals_->setCompositeMapDistance(3000);

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


void TerrainManager::frameStarted()
{
	terrain_group_->autoUpdateLodAll(true, Ogre::Any( Ogre::Real(HOLD_LOD_DISTANCE) ));
}