#include "GUIMain.h"

template<> GUIMain* Ogre::Singleton<GUIMain>::msSingleton = 0;

GUIMain::GUIMain()
{
	
}

GUIMain::~GUIMain()
{
	m_gui_->shutdown();
	delete m_gui_;
	m_platform_->shutdown();
	delete m_platform_;
}

void GUIMain::init()
{
	std::cout << DAO_Application::getSingletonPtr()->getWindow() << std::endl;
	std::cout << RenderManager::getSingletonPtr()->getSceneManager() << std::endl;

	m_platform_ = new MyGUI::OgrePlatform();
	m_platform_->initialise(DAO_Application::getSingletonPtr()->getWindow(), RenderManager::getSingletonPtr()->getSceneManager(), "MyGUI");

	m_gui_ = new MyGUI::Gui();
	m_gui_->initialise();
}

void GUIMain::injectMouseMove(const int& x, const int& y, const int& z)
{
	MyGUI::InputManager::getInstance().injectMouseMove(x, y, z);
}

void GUIMain::injectMousePress(const int& x, const int& y, OIS::MouseButtonID& id)
{
	MyGUI::InputManager::getInstance().injectMousePress(x, y, MyGUI::MouseButton::Enum(id));
}

void GUIMain::injectMouseRelease(const int& x, const int& y, OIS::MouseButtonID& id)
{
	MyGUI::InputManager::getInstance().injectMouseRelease(x, y, MyGUI::MouseButton::Enum(id));
}

void GUIMain::injectKeyPress(const OIS::KeyCode& key, const unsigned int& text)
{
	MyGUI::InputManager::getInstance().injectKeyPress(MyGUI::KeyCode::Enum(key), text);
}

void GUIMain::injectKeyRelease(const OIS::KeyCode& key)
{
	MyGUI::InputManager::getInstance().injectKeyRelease(MyGUI::KeyCode::Enum(key));
}