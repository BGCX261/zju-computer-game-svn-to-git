#pragma once

#include "resource.h"

//
// Library Header
//
#include <Ogre.h>
#include <OIS.h>
#include <SdkTrays.h>

/*============================================================================*\
 * Project Header
\*============================================================================*/
#include "CameraManager.h"
#include "Scene.h"
#include "Cube.h"


using namespace Ogre;
using namespace OIS;
using namespace OgreBites;


class CameraManager;

class Game
	:public OIS::KeyListener
	,public OIS::MouseListener
	,public Ogre::FrameListener
	,public Ogre::WindowEventListener
	,public OgreBites::SdkTrayListener
{
public:
	Game(void);
	virtual ~Game(void);

	void start();
	void buttonHit(OgreBites::Button* button);
	void yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit);
protected:
	virtual bool setup();
	virtual void generateNext();
	virtual bool configure(void);
	virtual void chooseSceneManager(void);
	virtual void createCamera(void);
	virtual void createFrameListener(void);
	virtual void createScene(void);
	virtual void destroyScene(void);
	virtual void createMenu(void);
	virtual void createViewports(void);
	virtual void createAxis(void);
	virtual void setupResources(void);
	virtual void createResourceListener(void);
	virtual void loadResources(void);
	virtual bool frameStarted(const FrameEvent& evt);
	virtual bool frameRenderingQueued(const FrameEvent& evt);
	virtual bool frameEnded(const FrameEvent& evt);

	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	

	//Adjust mouse clipping area
	virtual void windowResized(Ogre::RenderWindow* rw);
	//Unattach OIS before window shutdown (very important under Linux)
	virtual void windowClosed(Ogre::RenderWindow* rw);

	Ogre::Root *mRoot;
	Ogre::Camera* mCamera;
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;

	Scene *mGameBorad;

	CameraManager *mCameraMan;
	CubeManager *mCubeMan;
	Cube* mCubeEntity;
	Ogre::Overlay *mAxisOverlay;
	Ogre::SceneNode *mAxis;

	OgreBites::SdkTrayManager *mTrayMgr;

	bool mCursorWasVisible;						// was cursor visible before dialog appeared
	bool mShutDown;

	//OIS Input devices
	OIS::InputManager* mInputManager;
	OIS::Mouse*    mMouse;
	OIS::Keyboard* mKeyboard;
	bool mShiftDown;

	enum {
		MENU,
		PLAYING,
		PAUSE,
		OVER
	}mGameState;

	CubeType mNextCube;
};
