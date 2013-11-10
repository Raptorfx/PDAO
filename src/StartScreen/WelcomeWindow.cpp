#include "WelcomeWindow.h"


WelcomeWindow::WelcomeWindow(StartupFrameListener* parentFrameListener)
{
	mParentFrameListener_ = parentFrameListener;

	CEGUI::Imageset& welcomeWindowSet = CEGUI::ImagesetManager::getSingletonPtr()->create("WelcomeWindow.imageset", "Imagesets");

	mWindowManager_ = CEGUI::WindowManager::getSingletonPtr();
	mRootWindow_ = mWindowManager_->loadWindowLayout("WelcomeWindow.layout");
	CEGUI::System::getSingletonPtr()->setGUISheet(mRootWindow_);

	mTitleBar_ = (CEGUI::Titlebar*)mRootWindow_->getChild("WelcomeWindow/TitleBar");
	mTitleBar_->setText("Welcome "+GlobalValueStore::getSingletonPtr()->getStdString("LoginUsername"));

	mStartButton_ = (CEGUI::PushButton*)mRootWindow_->getChild("WelcomeWindow/StartButton");
	mStartButton_->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&WelcomeWindow::OnStartButtonClicked, this));
}


WelcomeWindow::~WelcomeWindow(void)
{
	mRootWindow_->destroy();
}


bool WelcomeWindow::OnStartButtonClicked(const CEGUI::EventArgs& e)
{
	mParentFrameListener_->changeToMainFrameListener();
	return true;
}
