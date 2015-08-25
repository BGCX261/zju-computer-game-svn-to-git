#include "Game.h"
#include <stdlib.h>

Game::Game(void):
mRoot(0),
mCamera(0),
mCameraMan(0),
mSceneMgr(0),
mWindow(0),
mResourcesCfg(Ogre::StringUtil::BLANK),
mPluginsCfg(Ogre::StringUtil::BLANK),
mCursorWasVisible(false),
mShutDown(false),
mInputManager(0),
mMouse(0),
mKeyboard(0),
mGameBorad(0),
mAxis(0),
mCubeMan(0),
mGameState(MENU),
mShiftDown(false)
{
	srand(time(NULL));
	generateNext();
}

Game::~Game(void)
{
	//Remove ourself as a Window listener
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
	delete mRoot;
	delete mAxis;
	delete mCubeMan;
}

//-------------------------------------------------------------------------------------
bool Game::configure(void)
{
	// Show the configuration dialog and initialise the system
	// You can skip this and use root.restoreConfig() to load configuration
	// settings if you were sure there are valid ones saved in ogre.cfg
	// Show the configuration dialog and initialise the system
	// You can skip this and use root.restoreConfig() to load configuration
	// settings if you were sure there are valid ones saved in ogre.cfg
	if(mRoot->restoreConfig() || mRoot->showConfigDialog())
	{
		// If returned true, user clicked OK so initialise
		// Here we choose to let the system create a default rendering window by passing 'true'
		mWindow = mRoot->initialise(true, "3D B-trix");

		// Let's add a nice window icon
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		HWND hwnd;
		mWindow->getCustomAttribute("WINDOW", (void*)&hwnd);
		LONG iconID   = (LONG)LoadIcon( GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1) );
		SetClassLong( hwnd, GCL_HICON, iconID );
#endif
		return true;
	}
	else
	{
		return false;
	}
}
//-------------------------------------------------------------------------------------
void Game::chooseSceneManager(void)
{
	// Get the SceneManager, in this case a generic one
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

}
void Game::start(void)
{
#ifdef _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif

	if (!setup())
		return;

	mRoot->startRendering();

	// clean up
	destroyScene();
}
bool Game::setup(void)
{
	mRoot = new Ogre::Root(mPluginsCfg);

	setupResources();

	bool carryOn = configure();
	if (!carryOn)
		return false;

	chooseSceneManager();
	createCamera();
	createViewports();

	// Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// Create any resource listeners (for loading screens)
	createResourceListener();
	// Load resources
	loadResources();

	// Create the scene
	createScene();

	createFrameListener();
	createMenu();
	return true;
}
void Game::setupResources(void)
{
	// Load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}
}
void Game::createCamera(void)
{
	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");
	// Position it at 500 in Z direction
	mCamera->setPosition(Ogre::Vector3(0,0,80));
	// Look back along -Z
	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(5);

	mCameraMan = new CameraManager(mCamera);   // create a default camera controller
}
void Game::createViewports(void)
{
	// Create one viewport, entire window
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

}
void Game::createResourceListener()
{

}
void Game::loadResources(void)
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
void Game::createFrameListener(void)
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputManager = OIS::InputManager::createInputSystem( pl );

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);

	//Set initial mouse clipping size
	windowResized(mWindow);

	//Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mMouse, this);
	mTrayMgr->getTrayContainer(TL_NONE)->hide();
	mTrayMgr->showBackdrop("SdkTrays/Bands");
	mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	mRoot->addFrameListener(this);
}
void Game::createScene()
{
	// setup some basic lighting for our scene
	mSceneMgr->setAmbientLight(ColourValue(.8, .8, .8));
	//mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	mSceneMgr->setShadowColour(ColourValue(0.0,0.0,0.0));


	// Create the SkyBox
	mSceneMgr->setSkyBox(true, "Examples/CloudyNoonSkyBox");


	Light *mSun = mSceneMgr->createLight("MainLight");
	mSun->setType(Light::LT_DIRECTIONAL);
//	mSun->setPosition(60, 500, 60);
//	mSun->setSpotlightRange(Degree(30), Degree(50));
	mSun->setDirection(-6.0f,-50.0f,-6.0f);
//	mSun->setVisible(true);

	mSun->setDiffuseColour(1.0,1.0,1.0);
	mSun->setSpecularColour(1, 0.9, 0.6);

	mSun = mSceneMgr->createLight();
	mSun->setType(Light::LT_POINT);
	mSun->setPosition(120, 500, 250);
	//mSun->setSpotlightRange(Degree(30), Degree(50));
	//mSun->setDirection(-mSun->getPosition());

	mSun->setDiffuseColour(0.8,0.8,0.8);
	mSun->setSpecularColour(0.8,0.8,0.8);

	createAxis();
}
void Game::destroyScene()
{
	Ogre::ResourceGroupManager::getSingleton().shutdownAll();
}
void Game::createMenu( void )
{
	mTrayMgr->destroyAllWidgets();
	
	mTrayMgr->showLogo(OgreBites::TL_CENTER);
	mTrayMgr->createButton(OgreBites::TL_CENTER,"Start","Play");
	mTrayMgr->createButton(OgreBites::TL_CENTER,"Quit","Quit");
	mTrayMgr->moveWidgetToTray("Start",OgreBites::TL_CENTER);
	mTrayMgr->moveWidgetToTray("Quit",OgreBites::TL_CENTER);
	mTrayMgr->showBackdrop("SdkTrays/Bands");
	mTrayMgr->showAll();
// 	Overlay* pOverlay = OverlayManager::getSingleton().getByName("Overlays/Menu"); 
// 	pOverlay->show();
	//OverlayElement *menu = OverlayManager::getSingleton().getOverlayElement("Overlays/MenuOverlay");
	//Overlay *lay = OverlayManager::getSingleton().create("Menu");
	//lay->add2D((OverlayContainer*)menu);
	//lay->show();

}

void Game::createAxis()
{
	static OverlayManager& om = OverlayManager::getSingleton();
	mAxisOverlay = om.create("Axis");
	mAxisOverlay->setZOrder(200);
	
	mAxis = new Ogre::SceneNode(mSceneMgr,"The Axis Node");
	int width = mWindow->getViewport(0)->getActualWidth();
	int height = mWindow->getViewport(0)->getActualHeight();
	mAxis->setPosition(35,-25,-90);
	Ogre::Entity *axisObj = mSceneMgr->createEntity("zb.mesh");
	/*
	Ogre::ManualObject *axisObj = mSceneMgr->createManualObject();    
	axisObj->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);    
	axisObj->position(0, 0, 0);
	axisObj->colour(1, 0, 0);
	axisObj->position(10, 0, 0);   // X axis, red

	axisObj->position(0, 0, 0);
	axisObj->colour(0, 1, 0);
	axisObj->position(0, 10, 0);   // Y axis, green

	axisObj->position(0, 0, 0);
	axisObj->colour(0, 0, 1);
	axisObj->position(0, 0, 10);   // Z axis, blue
	axisObj->end();
	axisObj->setRenderQueueGroup(105);
	*/
	mAxis->attachObject(axisObj);

	mAxisOverlay->add3D(mAxis);
}
bool Game::keyPressed( const OIS::KeyEvent &arg )
{
	// Your own code here.

	if (mTrayMgr->isDialogVisible())
		return true;   // don't process any more keys if dialog is up

	if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats
	{
		mTrayMgr->toggleAdvancedFrameStats();
	}
	else if(arg.key == OIS::KC_F5)   // refresh all textures
	{
		Ogre::TextureManager::getSingleton().reloadAll();
	}
	else if (arg.key == OIS::KC_LSHIFT || arg.key == OIS::KC_RSHIFT)
	{
		mShiftDown = true;
	}
	switch(mGameState)
	{
	case MENU:
		break;
	case PLAYING:
		if (arg.key == OIS::KC_Z)
		{
			if (mShiftDown)
				mCubeEntity->rotateZ(CLOCKWISE);
			else
				mCubeEntity->rotateZ();
		}
		else if (arg.key == OIS::KC_X)
		{
			if (mShiftDown)
				mCubeEntity->rotateX(CLOCKWISE);
			else
				mCubeEntity->rotateX();
		}
		else if (arg.key == OIS::KC_C)
		{
			if (mShiftDown)
				mCubeEntity->rotateY(CLOCKWISE);
			else
				mCubeEntity->rotateY();
		}
		else if (arg.key == OIS::KC_A)
		{
			mCubeEntity->moveX(NAGTIVE);
		}
		else if (arg.key == OIS::KC_D)
		{
			mCubeEntity->moveX(POSITIVE);
		}
		else if (arg.key == OIS::KC_W)
		{
			mCubeEntity->moveZ(NAGTIVE);
		}
		else if (arg.key == OIS::KC_S)
		{
			mCubeEntity->moveZ();
		}
		else if (arg.key == OIS::KC_SPACE)
		{
			mCubeEntity->fastDown();
		}
		else if (arg.key == OIS::KC_ESCAPE)
		{
			mAxisOverlay->hide();
			mTrayMgr->showAll();
			mTrayMgr->showYesNoDialog("Warning!","Back to menu?");
		}
		else if (arg.key == OIS::KC_P)
			mGameState = PAUSE;
		break;
	case PAUSE:
		if (arg.key == OIS::KC_P)
			mGameState = PLAYING;
		break;
	case OVER:
		Ogre::OverlayManager::getSingleton().getByName("Overlays/GameOver")->hide();
		mAxisOverlay->hide();
		mTrayMgr->showAll();
		mTrayMgr->showYesNoDialog("Do you","Want to play again?");
		break;
	default:
		break;
	}



	mCameraMan->injectKeyDown(arg);
	return true;
}
bool Game::keyReleased( const OIS::KeyEvent &arg )
{
	// Your own code here.
	if ((arg.key == OIS::KC_LSHIFT || arg.key == OIS::KC_RSHIFT)&&mShiftDown)
	{
		mShiftDown = false;
	}
	mCameraMan->injectKeyUp(arg);
	return true;
}
bool Game::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	// Your own code here.
	if (mTrayMgr->injectMouseDown(arg,id)) return true;
	mCameraMan->injectMouseDown(arg,id);
	return true;
}
bool Game::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	// Your own code here.
	if (mTrayMgr->injectMouseUp(arg,id)) return true;
	mCameraMan->injectMouseUp(arg,id);
	return true;
}
bool Game::mouseMoved( const OIS::MouseEvent &arg )
{
	// Your own code here.
	if (mTrayMgr->injectMouseMove(arg))
		return true;
	switch(mGameState)
	{
	case MENU:
		break;
	case PLAYING:
		mCameraMan->injectMouseMove(arg);
		break;
	case PAUSE:
		break;
	default:
		break;
	}


	return true;
}

void Game::buttonHit(OgreBites::Button* button)
{
	if (button->getName() == "Start")
	{
		if (!mCubeMan) mCubeMan = new CubeManager(mSceneMgr);
		if (!mGameBorad) mGameBorad = new Scene(mSceneMgr);
		mGameBorad->clear();
		mCubeEntity = CubeManager::getSingletonPtr()->createCube(mNextCube);
		mCubeEntity->setMaterialName("template/unitcube");
		generateNext();
		mTrayMgr->showBackdrop("SdkTrays/Shade");
		mTrayMgr->hideAll();
		mAxisOverlay->show();

		mGameState = PLAYING;
	}
	else if (button->getName() == "Quit")
	{
		mTrayMgr->showYesNoDialog("Warning!","Do you really want to Quit?");
	}
}


void Game::yesNoDialogClosed( const Ogre::DisplayString& question, bool yesHit )
{
	if (yesHit)
	{
		if (question == "Back to menu?")
		{
			mGameState = MENU;
			mAxisOverlay->hide();
			mTrayMgr->showBackdrop("SdkTrays/Bands");
			mTrayMgr->showAll();
		}
		else if (question == "Want to play again?")
		{
			mTrayMgr->buttonHit((OgreBites::Button*)mTrayMgr->getWidget("Start"));
		}
		else
		{
			mShutDown = true;
		}
	}
	else
	{
		if (question == "Back to menu?")
		{
			mTrayMgr->showBackdrop("SdkTrays/Shade");
			mTrayMgr->hideAll();
			mAxisOverlay->show();
		}
		else if (question == "Want to play again?")
		{
			mShutDown = true;
		}
		else
		{

		}
	}
}

bool Game::frameStarted(const FrameEvent& evt)
{
	// Your own code here.

	return true;
}
bool Game::frameRenderingQueued(const FrameEvent& evt)
{
	static float timesplashed = 0.0f;

	if(mWindow->isClosed())
		return false;

	if(mShutDown)
		return false;

	// Need to capture/update each device
	mKeyboard->capture();
	mMouse->capture();

	mTrayMgr->frameRenderingQueued(evt);

	if (!mTrayMgr->isDialogVisible())
	{
		mCameraMan->frameRenderingQueued(evt);   // if dialog isn't up, then update the camera
		switch(mGameState)
		{
		case MENU:
			break;
		case PLAYING:
			mCubeEntity->frameRenderingQueued(evt);
			mAxis->setOrientation(mCameraMan->getOrientation());
			if ((int)timesplashed != (int)(timesplashed+evt.timeSinceLastFrame))
			{
				
				if (!mCubeEntity->moveY(NAGTIVE))
				{
					mGameBorad->merge();
					mGameBorad->deleteLevels(mGameBorad->levelsToDelete());
					
					mCubeEntity = mCubeMan->createCube(mNextCube);
					if (mCubeEntity)
					{
						mCubeEntity->reset();
						generateNext();
					}
					else
					{
						mGameState = OVER;
						Ogre::OverlayManager::getSingleton().getByName("Overlays/GameOver")->show();
					}
				}
			}
			timesplashed += evt.timeSinceLastFrame;
			break;
		case PAUSE:
			break;
		case OVER:
			
			break;
		default:
			break;
		}
	}
	return true;
}
bool Game::frameEnded(const FrameEvent& evt)
{
	// Your own code here.

	return true;
}
//Adjust mouse clipping area
void Game::windowResized(Ogre::RenderWindow* rw)
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void Game::windowClosed(Ogre::RenderWindow* rw)
{
	//Only close for window that created OIS (the main window in these demos)
	if( rw == mWindow )
	{
		if( mInputManager )
		{
			mInputManager->destroyInputObject( mMouse );
			mInputManager->destroyInputObject( mKeyboard );

			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
	}
}

void Game::generateNext()
{
	mNextCube = static_cast<CubeType>(rand()%CUBE_TOTAL_NUM);
}
