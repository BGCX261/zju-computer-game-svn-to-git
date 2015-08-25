#include "CameraManager.h"
#include <math.h>


CameraManager::CameraManager(Camera *c)
:mCameraDistance(350.0f)
,mPitchDegree(-15.0f)
,mPitch(0)
,mYaw(0)
,mMove(0)
,mSpeed(0)
,mPitchSpeed(0)
,mYawSpeed(0)
{
	mCamera = c;
	
	SceneNode *root = mCamera->getSceneManager()->getRootSceneNode();
	mYawNode = root->createChildSceneNode("Y-Axis");
	mPitchNode = mYawNode->createChildSceneNode("X-Axis",Vector3(0.0,100.0,0.0));
	
	mDistNode = mPitchNode->createChildSceneNode("Z-Axis",Vector3(0.0,0.0,mCameraDistance));
	mDistNode->attachObject(mCamera);
	
	mPitchNode->pitch(Radian(Degree(mPitchDegree)));
	mYawNode->yaw(Radian(Degree(-15)));
}

CameraManager::~CameraManager(void)
{

}
void CameraManager::updatePosition()
{
	if (mCameraDistance>500)
		mCameraDistance = 500;
	else if (mCameraDistance < 30)
		mCameraDistance = 30;

	mDistNode->setPosition(0.0,0.0,mCameraDistance);

	if (mPitchDegree < - 45.0)
		mPitchDegree = -45.0;
	else if (mPitchDegree > 30.0)
		mPitchDegree = 30.0;

	mPitchNode->setOrientation(1,0,0,0);
	mPitchNode->pitch(Radian(Degree(mPitchDegree)));

}
void CameraManager::injectMouseDown(const OIS::MouseEvent &evt,OIS::MouseButtonID id)
{

}
void CameraManager::injectMouseUp(const OIS::MouseEvent &evt,OIS::MouseButtonID id)
{

}
void CameraManager::injectMouseMove(const OIS::MouseEvent &evt)
{
	mYawNode->yaw(Radian(evt.state.X.rel*0.005f));
	Real pitch = evt.state.Y.rel*0.1f;
	mPitchDegree += pitch;

	Real dist = evt.state.Z.rel*0.05f;
	mCameraDistance += dist;

	updatePosition();
}
void CameraManager::injectKeyDown(const OIS::KeyEvent &evt)
{
	if (evt.key == OIS::KC_PGUP)
		mMove = -1;
	else if (evt.key == OIS::KC_PGDOWN)
		mMove = 1;
	else if (evt.key == OIS::KC_LEFT)
		mYaw = -1;
	else if (evt.key == OIS::KC_RIGHT)
		mYaw = 1;
	else if (evt.key == OIS::KC_UP)
		mPitch = -1;
	else if (evt.key == OIS::KC_DOWN)
		mPitch = 1;
}
void CameraManager::injectKeyUp(const OIS::KeyEvent &evt)
{
	if (evt.key == OIS::KC_PGUP && mMove == -1) mMove = 0;
	else if (evt.key == OIS::KC_PGDOWN && mMove == 1) mMove = 0;
	else if (evt.key == OIS::KC_LEFT && mYaw == -1) mYaw = 0;
	else if (evt.key == OIS::KC_RIGHT && mYaw == 1) mYaw = 0;
	else if (evt.key == OIS::KC_UP && mPitch == -1) mPitch = 0;
	else if (evt.key == OIS::KC_DOWN && mPitch == 1) mPitch = 0;
}
void CameraManager::frameRenderingQueued(const FrameEvent& evt)
{
	static Real TopSpeed = 10.0;
	Real accel = 10.0;
	mSpeed += mMove*accel*evt.timeSinceLastFrame;
	if (fabs(mSpeed) > TopSpeed) mSpeed = TopSpeed;
	else if (fabs(mSpeed) < std::numeric_limits<Real>::epsilon()) mSpeed = 0.0;

	mCameraDistance += mSpeed;
	mSpeed *= 0.9;

	static Real TopRotateSpeed = 4.0;
	mPitchSpeed += mPitch*accel*evt.timeSinceLastFrame;
	if (fabs(mPitchSpeed) > TopRotateSpeed) mPitchSpeed = TopRotateSpeed;
	else if (fabs(mPitchSpeed)<std::numeric_limits<Real>::epsilon()) mPitchSpeed = 0.0;

	mPitchDegree += mPitchSpeed;
	mPitchSpeed *= 0.9;

	mYawSpeed += mYaw*accel*evt.timeSinceLastFrame;
	if (fabs(mYawSpeed) > TopRotateSpeed ) mYawSpeed = TopRotateSpeed;
	else if (fabs(mYawSpeed) < std::numeric_limits<Real>::epsilon()) mYawSpeed = 0.0;

	mYawNode->yaw(Radian(Degree(mYawSpeed)));
	mYawSpeed *= 0.9;

	updatePosition();
}
Ogre::Quaternion CameraManager::getOrientation()
{
	Quaternion ident;
	Radian angle;
	Vector3 vector;
	mPitchNode->getOrientation().ToAngleAxis(angle,vector);
	ident.FromAngleAxis(-angle,vector);
	Quaternion tmp;
	mYawNode->getOrientation().ToAngleAxis(angle,vector);
	tmp.FromAngleAxis(-angle,vector);
	ident = ident * tmp;

	return ident;
}