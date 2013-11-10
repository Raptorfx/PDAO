#include "RenderManager.h"

template<> RenderManager* Ogre::Singleton<RenderManager>::msSingleton = 0;

RenderManager::RenderManager():
		mainCamera_(0),
        sceneManager_(DAO_Application::getSingletonPtr()->getRoot()->createSceneManager(Ogre::ST_GENERIC)), //ST_EXTERIOR_CLOSE
        viewport_(0),
        shaderGenerator_(0),
        initialized_(false)
{
	//Initialise RTshader system
    /*if (!Ogre::RTShader::ShaderGenerator::initialize()) {
        //TODO - exit properly
        LogManager::getSingletonPtr()->logMessage("FATAL:"
                "Failed to initialise shader generator, exiting", Ogre::LML_CRITICAL);
        exit(1);
    }*/
}


RenderManager::~RenderManager()
{
	DAO_Application::getSingletonPtr()->getRoot()->shutdown();
	delete mTerrainManager_;
}

/*! \brief Sets up the main camera
*
*/
void RenderManager::createCamera()
{
    mainCamera_ = sceneManager_->createCamera("PlayerCam");

	mainCamera_->setNearClipDistance(0.1f);
	mainCamera_->setFarClipDistance(500);

	if (DAO_Application::getSingleton().getRoot()->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
    {
        mainCamera_->setFarClipDistance(0);   // enable infinite far clip distance if we can
    }
}

/*! \brief setup the viewports
*
*/
void RenderManager::createViewports()
{
    viewport_ = DAO_Application::getSingleton().getWindow()->addViewport(mainCamera_);
    viewport_->setBackgroundColour(Ogre::ColourValue(0.3f, 0.3f, 0.3f));
    mainCamera_->setAspectRatio(Ogre::Real(viewport_->getActualWidth()) / Ogre::Real(viewport_->getActualHeight()));
}

void RenderManager::initCEGUI()
{
	//Init the gui
	gui_renderer_ = &CEGUI::OgreRenderer::bootstrapSystem();

	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");	
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");

	CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");

	CEGUI::MouseCursor::getSingleton().setImage( CEGUI::System::getSingleton().getDefaultMouseCursor());
	CEGUI::MouseCursor::getSingleton().setPosition(CEGUI::Point(CEGUI::Vector2(0,0))); 
}

/*! \brief setup the scene
*
*/
void RenderManager::createScene()
{
	//Set up the shader generator
    //shaderGenerator_ = Ogre::RTShader::ShaderGenerator::getSingletonPtr();

	mTerrainManager_ = new TerrainManager();
	mTerrainManager_->createTerrain();

	sceneManager_->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
}

