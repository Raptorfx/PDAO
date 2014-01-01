#include "InputManager.h"

template<> InputManager* Ogre::Singleton<InputManager>::msSingleton = 0;

InputManager::InputManager(void):
		frameListener_(DAO_FrameListener::getSingletonPtr())
{
	LogManager::getSingleton().logMessage("*** Initializing OIS ***");

	size_t windowHnd = 0;

    DAO_Application::getSingleton().getWindow()->getCustomAttribute("WINDOW", &windowHnd);

    std::ostringstream windowHndStr;
    windowHndStr << windowHnd;

    //setup parameter list for OIS
    OIS::ParamList paramList;
    paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	//setup InputManager
    mInputManager_ = OIS::InputManager::createInputSystem(paramList);

    //setup Keyboard
    mKeyboard_ = static_cast<OIS::Keyboard*>(mInputManager_->createInputObject(OIS::OISKeyboard, true));
    mKeyboard_->setEventCallback(this);
	mKeyboard_->setBuffered(true);

    //setup Mouse
    mMouse_ = static_cast<OIS::Mouse*>(mInputManager_->createInputObject(OIS::OISMouse, true));
    mMouse_->setEventCallback(this);
	mMouse_->setBuffered(true);

	rotX_ = 0.0f;
	rotY_ = 0.0f;

	move_height_ = move_width_ = 0;
	x_abs_ = y_abs_ = 0;

	mLMouseDown_ = false;
	mRMouseDown_ = false;

	m_mouse_extra_data_ = Ogre::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
}


InputManager::~InputManager(void)
{
	LogManager::getSingleton().logMessage("*** Destroying InputManager ***");
    mInputManager_->destroyInputObject(mMouse_);
    mInputManager_->destroyInputObject(mKeyboard_);
    OIS::InputManager::destroyInputSystem(mInputManager_);
    mInputManager_ = 0;
}

bool InputManager::isKeyPressed(OIS::KeyCode code)
{
	return m_keyboard_store_[code];
}

bool InputManager::isMouseButtonPressed(OIS::MouseButtonID id)
{
	return m_mouse_id_store_[id];
}


bool InputManager::mouseMoved(const OIS::MouseEvent &evt)
{
	//Inject Mouse for GUI
	GUIMain::getSingletonPtr()->injectMouseMove(evt.state.X.abs, evt.state.Y.abs, evt.state.Z.abs);

	m_rel_angle_x_ = (float)( (double)evt.state.X.rel / (double)DAO_Application::getSingleton().getWindow()->getViewport(0)->getActualWidth() );
	m_rel_angle_y_ = (float)( (double)evt.state.Y.rel / (double)DAO_Application::getSingleton().getWindow()->getViewport(0)->getActualHeight());

	m_mouse_extra_data_.x = m_rel_angle_x_;
	m_mouse_extra_data_.y = m_rel_angle_y_;
	m_mouse_extra_data_.w = (Ogre::Real)evt.state.Z.rel;

	rotX_ += evt.state.X.rel;
	rotY_ += evt.state.Y.rel;

	//Store the event
	move_height_ = evt.state.height;
	move_width_ = evt.state.width;
	x_abs_ = evt.state.X.abs;
	y_abs_ = evt.state.Y.abs;

	return true;
}

bool InputManager::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	//Inject Mouse for GUI
	GUIMain::getSingletonPtr()->injectMousePress(arg.state.X.abs, arg.state.Y.abs, id);

	switch(id)
	{
		case  OIS::MB_Left:
			m_mouse_id_store_[OIS::MB_Left] = true;
			mLMouseDown_ = true;
			break;

		case OIS::MB_Right:
			m_mouse_id_store_[OIS::MB_Right] = true;
			mRMouseDown_ = true;
			rotX_ = rotY_ = 0.0f; //Reset the angles, to prevent camera rotation on right mb
			break;

		case OIS::MB_Middle:
			m_mouse_id_store_[OIS::MB_Middle] = true;
			break;

		default:
			break;
	}
	return true;
}

bool InputManager::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	//Inject Mouse for GUI
	GUIMain::getSingletonPtr()->injectMouseRelease(arg.state.X.abs, arg.state.Y.abs, id);

	switch(id)
	{
		case  OIS::MB_Left:
			m_mouse_id_store_[OIS::MB_Left] = false;
			mLMouseDown_ = false;
			break;

		case OIS::MB_Right:
			m_mouse_id_store_[OIS::MB_Right] = false;
			mRMouseDown_ = false;
			break;

		case OIS::MB_Middle:
			m_mouse_id_store_[OIS::MB_Middle] = false;
			break;

		default:
			break;
	}
	return true;
}

bool InputManager::keyPressed(const OIS::KeyEvent &arg)
{
	//Inject Keyboard for GUI
	GUIMain::getSingletonPtr()->injectKeyPress(arg.key, arg.text);

	switch (arg.key)
    {
        case OIS::KC_GRAVE:
			m_keyboard_store_[OIS::KC_GRAVE] = true;
			break;

        case OIS::KC_F12:
			m_keyboard_store_[OIS::KC_F12] = true;
            break;

        case OIS::KC_LEFT:
			m_keyboard_store_[OIS::KC_LEFT] = true;
			break;

        case OIS::KC_A:
            directionKeyPressed_ = true;
			m_keyboard_store_[OIS::KC_A] = true;
            break;

        case OIS::KC_RIGHT:
			m_keyboard_store_[OIS::KC_RIGHT] = true;
            break;

		case OIS::KC_SPACE:
			m_keyboard_store_[OIS::KC_SPACE] = true;
			break;

        case OIS::KC_D:
            directionKeyPressed_ = true;
			m_keyboard_store_[OIS::KC_D] = true;
            break;

        case OIS::KC_UP:
			m_keyboard_store_[OIS::KC_UP] = true;
            break;

        case OIS::KC_W:
            directionKeyPressed_ = true;
			m_keyboard_store_[OIS::KC_W] = true;
            break;

        case OIS::KC_DOWN:
			m_keyboard_store_[OIS::KC_DOWN] = true;
            break;

        case OIS::KC_S:
            directionKeyPressed_ = true;
			m_keyboard_store_[OIS::KC_S] = true;
            break;

        case OIS::KC_PGUP:
			m_keyboard_store_[OIS::KC_PGUP] = true;
            break;

        case OIS::KC_E:
			m_keyboard_store_[OIS::KC_E] = true;
            break;

        case OIS::KC_INSERT:
			m_keyboard_store_[OIS::KC_INSERT] = true;
            break;

        case OIS::KC_Q:
			m_keyboard_store_[OIS::KC_Q] = true;
            break;

        case OIS::KC_HOME:
			m_keyboard_store_[OIS::KC_HOME] = true;
            break;

        case OIS::KC_END:
			m_keyboard_store_[OIS::KC_END] = true;
            break;

        case OIS::KC_DELETE:
			m_keyboard_store_[OIS::KC_DELETE] = true;
            break;

        case OIS::KC_PGDOWN:
			m_keyboard_store_[OIS::KC_PGDOWN] = true;
            break;

        case OIS::KC_R:
			m_keyboard_store_[OIS::KC_R] = true;
            break;

        case OIS::KC_COMMA:
			m_keyboard_store_[OIS::KC_COMMA] = true;
            break;

        case OIS::KC_PERIOD:
			m_keyboard_store_[OIS::KC_PERIOD] = true;
            break;

        case OIS::KC_B:
			m_keyboard_store_[OIS::KC_B] = true;
            break;

        case OIS::KC_T:
			m_keyboard_store_[OIS::KC_T] = true;
            break;

        case OIS::KC_ESCAPE:
			// Quit the game
			frameListener_->requestExit();
			m_keyboard_store_[OIS::KC_ESCAPE] = true;
            break;

        case OIS::KC_SYSRQ:
			// Print a screenshot
            DAO_ResourceManager::getSingleton().takeScreenshot();
			m_keyboard_store_[OIS::KC_SYSRQ] = true;
            break;

        case OIS::KC_1:
			m_keyboard_store_[OIS::KC_1] = true;
			break;

        case OIS::KC_2:
			m_keyboard_store_[OIS::KC_2] = true;
			break;

        case OIS::KC_3:
			m_keyboard_store_[OIS::KC_3] = true;
			break;

        case OIS::KC_4:
			m_keyboard_store_[OIS::KC_4] = true;
			break;

        case OIS::KC_5:
			m_keyboard_store_[OIS::KC_5] = true;
			break;

        case OIS::KC_6:
			m_keyboard_store_[OIS::KC_6] = true;
			break;

        case OIS::KC_7:
			m_keyboard_store_[OIS::KC_7] = true;
			break;

        case OIS::KC_8:
			m_keyboard_store_[OIS::KC_8] = true;
			break;

        case OIS::KC_9:
			m_keyboard_store_[OIS::KC_9] = true;
			break;

        case OIS::KC_0:
            m_keyboard_store_[OIS::KC_0] = true;
            break;

        default:
            break;
    }

	return true;
}

bool InputManager::keyReleased(const OIS::KeyEvent &arg)
{
	//Inject Keyboard for GUI
	GUIMain::getSingletonPtr()->injectKeyRelease(arg.key);

	switch (arg.key)
    {
		case OIS::KC_GRAVE:
			m_keyboard_store_[OIS::KC_GRAVE] = false;
			break;

        case OIS::KC_F12:
			m_keyboard_store_[OIS::KC_F12] = false;
            break;

        case OIS::KC_LEFT:
			m_keyboard_store_[OIS::KC_LEFT] = false;
			break;

        case OIS::KC_A:
            directionKeyPressed_ = false;
			m_keyboard_store_[OIS::KC_A] = false;
            break;

        case OIS::KC_RIGHT:
			m_keyboard_store_[OIS::KC_RIGHT] = false;
            break;

		case OIS::KC_SPACE:
			m_keyboard_store_[OIS::KC_SPACE] = false;
			break;

        case OIS::KC_D:
            directionKeyPressed_ = false;
			m_keyboard_store_[OIS::KC_D] = false;
            break;

        case OIS::KC_UP:
			m_keyboard_store_[OIS::KC_UP] = false;
            break;

        case OIS::KC_W:
            directionKeyPressed_ = true;
			m_keyboard_store_[OIS::KC_W] = false;
            break;

        case OIS::KC_DOWN:
			m_keyboard_store_[OIS::KC_DOWN] = false;
            break;

        case OIS::KC_S:
            directionKeyPressed_ = false;
			m_keyboard_store_[OIS::KC_S] = false;
            break;

        case OIS::KC_PGUP:
			m_keyboard_store_[OIS::KC_PGUP] = false;
            break;

        case OIS::KC_E:
			m_keyboard_store_[OIS::KC_E] = false;
            break;

        case OIS::KC_INSERT:
			m_keyboard_store_[OIS::KC_INSERT] = false;
            break;

        case OIS::KC_Q:
			m_keyboard_store_[OIS::KC_Q] = false;
            break;

        case OIS::KC_HOME:
			m_keyboard_store_[OIS::KC_HOME] = false;
            break;

        case OIS::KC_END:
			m_keyboard_store_[OIS::KC_END] = false;
            break;

        case OIS::KC_DELETE:
			m_keyboard_store_[OIS::KC_DELETE] = false;
            break;

        case OIS::KC_PGDOWN:
			m_keyboard_store_[OIS::KC_PGDOWN] = false;
            break;

        case OIS::KC_R:
			m_keyboard_store_[OIS::KC_R] = false;
            break;

        case OIS::KC_COMMA:
			m_keyboard_store_[OIS::KC_COMMA] = false;
            break;

        case OIS::KC_PERIOD:
			m_keyboard_store_[OIS::KC_PERIOD] = false;
            break;

        case OIS::KC_B:
			m_keyboard_store_[OIS::KC_B] = false;
            break;

        case OIS::KC_T:
			m_keyboard_store_[OIS::KC_T] = false;
            break;

        case OIS::KC_ESCAPE:
			m_keyboard_store_[OIS::KC_ESCAPE] = false;
            break;

        case OIS::KC_SYSRQ:
			m_keyboard_store_[OIS::KC_SYSRQ] = false;
            break;

        case OIS::KC_1:
			m_keyboard_store_[OIS::KC_1] = false;
			break;

        case OIS::KC_2:
			m_keyboard_store_[OIS::KC_2] = false;
			break;

        case OIS::KC_3:
			m_keyboard_store_[OIS::KC_3] = false;
			break;

        case OIS::KC_4:
			m_keyboard_store_[OIS::KC_4] = false;
			break;

        case OIS::KC_5:
			m_keyboard_store_[OIS::KC_5] = false;
			break;

        case OIS::KC_6:
			m_keyboard_store_[OIS::KC_6] = false;
			break;

        case OIS::KC_7:
			m_keyboard_store_[OIS::KC_7] = false;
			break;

        case OIS::KC_8:
			m_keyboard_store_[OIS::KC_8] = false;
			break;

        case OIS::KC_9:
			m_keyboard_store_[OIS::KC_9] = false;
			break;

        case OIS::KC_0:
            m_keyboard_store_[OIS::KC_0] = false;
            break;

        default:
            break;
    }

	return true;
}
