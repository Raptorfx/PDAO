#include "LoginWindow.h"


LoginWindow::LoginWindow(StartupFrameListener* parentFrameListener)
{
	mParentFrameListener_ = parentFrameListener;
	mWindowManager_ = CEGUI::WindowManager::getSingletonPtr();
	mRootWindow_ = mWindowManager_->loadWindowLayout("LoginWindow.layout");
	CEGUI::System::getSingletonPtr()->setGUISheet(mRootWindow_);

	mLoginUsername_ = (CEGUI::Editbox*)mRootWindow_->getChild("LoginWindowRoot/LoginUsername");
	mLoginPassword_ = (CEGUI::Editbox*)mRootWindow_->getChild("LoginWindowRoot/LoginPassword");
	mLoginButton_ = (CEGUI::PushButton*)mRootWindow_->getChild("LoginWindowRoot/LoginButton");

	mUsernameFirstClick_ = true;
	mPasswordFirstClick_ = true;

	mLoginUsername_->subscribeEvent(CEGUI::Editbox::EventMouseClick, CEGUI::Event::Subscriber(&LoginWindow::OnUsernameFieldClicked, this));
	mLoginPassword_->subscribeEvent(CEGUI::Editbox::EventMouseClick, CEGUI::Event::Subscriber(&LoginWindow::OnPasswordFieldClicked, this));
	mLoginButton_->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&LoginWindow::OnLoginButtonClicked, this));
}


LoginWindow::~LoginWindow(void)
{
	mRootWindow_->destroy();
}

bool LoginWindow::OnLoginButtonClicked(const CEGUI::EventArgs& e)
{
	GlobalValueStore* gvs = GlobalValueStore::getSingletonPtr();

	std::string loginUsername = mLoginUsername_->getText().c_str();
	std::string loginPassword = mLoginPassword_->getText().c_str();

	if(loginUsername.empty() || loginPassword.empty())
	{
		//Give some error message
	}
	else
	{
		gvs->setStdString("LoginUsername", loginUsername);

		MD5 md5_gen;
		md5_gen.update(loginPassword.c_str(), loginPassword.length());
		md5_gen.finalize();

		gvs->setStdString("LoginPassword", md5_gen.hexdigest());

		//Verify the data with the server

		mParentFrameListener_->changeToWelcomeWindow();
	}
	return true;
}

bool LoginWindow::OnUsernameFieldClicked(const CEGUI::EventArgs& e)
{
	if(mUsernameFirstClick_)
	{
		mLoginUsername_->setText("");
		mUsernameFirstClick_ = false;
	}
	return true;
}

bool LoginWindow::OnPasswordFieldClicked(const CEGUI::EventArgs& e)
{
	if(mPasswordFirstClick_)
	{
		mLoginPassword_->setText("");
		mPasswordFirstClick_ = false;
	}
	return true;
}

