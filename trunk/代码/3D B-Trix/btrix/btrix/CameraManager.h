#pragma once
#include <Ogre.h>
#include <OIS.h>

using namespace Ogre;

class CameraManager
{
public:
	CameraManager(Camera*);
	virtual ~CameraManager(void);
private:
	Camera *mCamera;
	SceneNode *mPitchNode;
	SceneNode *mYawNode;
	SceneNode *mDistNode;
	Real mPitchDegree;
	Real mCameraDistance;
	int mPitch,mYaw,mMove;
	Real mSpeed;
	Real mPitchSpeed;
	Real mYawSpeed;

	void updatePosition();
public:
	virtual void injectMouseMove(const OIS::MouseEvent &evt);
	virtual void injectMouseDown(const OIS::MouseEvent &evt,OIS::MouseButtonID id);
	virtual void injectMouseUp(const OIS::MouseEvent &evt,OIS::MouseButtonID id);

	virtual void injectKeyDown(const OIS::KeyEvent &evt);
	virtual void injectKeyUp(const OIS::KeyEvent &evt);
	virtual void frameRenderingQueued(const FrameEvent& evt);
	virtual Ogre::Quaternion getOrientation();
};
