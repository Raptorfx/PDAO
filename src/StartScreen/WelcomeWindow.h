#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <CEGUI.h>
#include "RendererModules/Ogre/CEGUIOgreRenderer.h"
#include "Ogre\Ogre.h"

#include "StartupFrameListener.h"
#include "../GlobalValueStore.h"

class StartupFrameListener;

class WelcomeWindow
{
public:
	WelcomeWindow(StartupFrameListener* parentFrameListener);
	~WelcomeWindow(void);

private:

	bool OnStartButtonClicked(const CEGUI::EventArgs& e);


	StartupFrameListener* mParentFrameListener_;
	CEGUI::WindowManager* mWindowManager_;
	CEGUI::Window* mRootWindow_;

	CEGUI::Titlebar* mTitleBar_;
	CEGUI::PushButton*  mStartButton_;
};


#endif
