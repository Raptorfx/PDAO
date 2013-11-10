#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <CEGUI.h>
#include "RendererModules/Ogre/CEGUIOgreRenderer.h"
#include "Ogre\Ogre.h"

#include "StartupFrameListener.h"
#include "../GlobalValueStore.h"
#include "../Functions/md5.h"

class StartupFrameListener;

class LoginWindow
{
public:
	LoginWindow(StartupFrameListener* parentFrameListener);
	~LoginWindow(void);

private:
	StartupFrameListener* mParentFrameListener_;
	CEGUI::WindowManager* mWindowManager_;
	CEGUI::Window* mRootWindow_;

	CEGUI::Editbox* mLoginUsername_;
	CEGUI::Editbox* mLoginPassword_;
	CEGUI::PushButton*  mLoginButton_;

	bool OnLoginButtonClicked(const CEGUI::EventArgs& e);
	bool OnUsernameFieldClicked(const CEGUI::EventArgs& e);
	bool OnPasswordFieldClicked(const CEGUI::EventArgs& e);

	bool mUsernameFirstClick_;
	bool mPasswordFirstClick_;
};


#endif
