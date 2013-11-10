#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <map>

#include "Ogre/OgreSingleton.h"
#include "OIS\OIS.h"

#include "LogManager.h"
#include "DAO_FrameListener.h"

class DAO_FrameListener;

class InputManager :
        public Ogre::Singleton<InputManager>,
        public OIS::MouseListener,
        public OIS::KeyListener
{
public:
	InputManager(void);
	~InputManager(void);

	inline bool getLMouseDown() const {return mLMouseDown_;}
	inline bool getRMouseDown() const {return mRMouseDown_;}

	//Camera handling
	inline float getRotX() const {return rotX_;}
	inline void setRotX(float x) {rotX_ = x;}

	inline float getRotY() const {return rotY_;}
	inline void setRotY(float y) {rotY_ = y;}

	inline float getRelAngleX() { return m_rel_angle_x_;}
	inline void setRelAngleX(float x) {m_rel_angle_x_ = x;}

	inline float getRelAngleY() { return m_rel_angle_y_;}
	inline void setRelAngleY(float y) {m_rel_angle_y_ = y;}

	inline Ogre::Vector4 getMouseExtraData() { return m_mouse_extra_data_;}
	inline void setMouseExtraData(Ogre::Vector4 data) { m_mouse_extra_data_ = data;}

	bool mouseMoved     (const OIS::MouseEvent &arg);
    bool mousePressed   (const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool mouseReleased  (const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool keyPressed     (const OIS::KeyEvent &arg);
    bool keyReleased    (const OIS::KeyEvent &arg);
    void handleHotkeys  (OIS::KeyCode keycode);

	inline OIS::Mouse*      getMouse()      const   {return mMouse_;}
    inline OIS::Keyboard*   getKeyboard()   const   {return mKeyboard_;}

	bool isKeyPressed(OIS::KeyCode code);
	bool isMouseButtonPressed(OIS::MouseButtonID id);

private:

	DAO_FrameListener*  frameListener_;
	OIS::InputManager*	mInputManager_;
    OIS::Mouse*         mMouse_;
    OIS::Keyboard*      mKeyboard_;

	bool                directionKeyPressed_;
	bool                mLMouseDown_, mRMouseDown_;
    bool                mouseDownOnCEGUIWindow_;

	std::map<OIS::KeyCode, bool>		m_keyboard_store_;
	std::map<OIS::MouseButtonID, bool>	m_mouse_id_store_;

	CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);

	//New extra mouse movements
	Ogre::Vector4 m_mouse_extra_data_;

	//Last event storage
	int move_height_;
	int move_width_;
	int x_abs_;
	int y_abs_;

	//Camera movement translation
	float rotX_;
	float rotY_;

	//Relative angles for rotation by mouse per frame
	float m_rel_angle_x_;
	float m_rel_angle_y_;
};

#endif
