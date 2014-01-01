#ifndef GUIMAIN_H
#define GUIMAIN_H

#include <MYGUI/MyGUI.h>
#include <MYGUI/MyGUI_OgrePlatform.h>
#include <OIS/OIS.h>

#include "../DAO_Application.h"
#include "../RenderManager.h"
#include "Ogre/OgreSingleton.h"

class GUIMain : public Ogre::Singleton<GUIMain>
{
public:
	GUIMain();
	~GUIMain();

	void init();

	void injectMouseMove(const int& x, const int& y, const int& z);
	void injectMousePress(const int& x, const int& y, OIS::MouseButtonID& id);
	void injectMouseRelease(const int& x, const int& y, OIS::MouseButtonID& id);
	void injectKeyPress(const OIS::KeyCode& key, const unsigned int& text);
	void injectKeyRelease(const OIS::KeyCode& key);

private:
	MyGUI::Gui* m_gui_;
	MyGUI::OgrePlatform* m_platform_;
};


#endif