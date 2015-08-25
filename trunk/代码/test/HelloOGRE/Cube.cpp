#include "StdAfx.h"
#include "Cube.h"

/************************************************************************
	初始化代码
 ************************************************************************/
Ogre::Real Cube::fFallSpeed = 10.0f;
Ogre::Real Cube::fMoveSpeed = 10.0f;
Ogre::Real Cube::fRotateSpeed = 10.0f;
Ogre::SceneManager* Cube::mScene = NULL;
Cube* Cube::theCube[CUBE_TYPE_COUNT] = {0};




/************************************************************************
	类的实现
 ************************************************************************/
void Cube::initialize(Ogre::SceneManager *scene)
{
	Cube::mScene = scene;
	char name[255];
	for (int i=0;i<CUBE_TYPE_COUNT;++i)
	{
		sprintf(name,"%d.mesh",i);
		Cube::theCube[i] = new Cube(name);
	}
}


Cube::Cube(const Ogre::String &meshfile)
: mVelocity(Ogre::Vector3::ZERO)
, mColor(Ogre::ColourValue::Blue)
, isMoveBack(false)
, isMoveForward(false)
, isMoveLeft(false)
, isMoveRight(false)
, isMoveUp(false)
, isMoveDown(false)
, isRotateX(false)
, isRotateY(false)
, isRotateZ(false)
{
	cube = Cube::mScene->createEntity(meshfile);
}
void Cube::injectKeyDown(const OIS::KeyEvent& evt)
{
	using namespace OIS;
	switch(evt.key)
	{
	case KC_A:
		break;
	case KC_S:
		break;
	case KC_D:
		break;
	case KC_E:
		break;
	case KC_Z:
		break;
	case KC_X:
		break;
	case KC_C:
		break;
	case KC_SPACE:
		break;
	default:
		break;
	}
}
void Cube::injectKeyUp(const OIS::KeyEvent& evt)
{
	using namespace OIS;
	switch(evt.key)
	{
	case KC_A:
		break;
	case KC_S:
		break;
	case KC_D:
		break;
	case KC_E:
		break;
	case KC_Z:
		break;
	case KC_X:
		break;
	case KC_C:
		break;
	case KC_SPACE:
		break;
	default:
		break;
	}
}
void Cube::rotateX()
{

}
void Cube::rotateY()
{

}
void Cube::rotateZ()
{

}
void Cube::moveLeft()
{
	if (canMoveLeft())
	{
		isMoveLeft = true;
		mVelocity = Ogre::Vector3(-Cube::fMoveSpeed,0.0f,0.0f);
	}

}
void Cube::moveRight()
{

}
void Cube::moveBackrword()
{

}
void Cube::moveForword()
{

}
bool Cube::canMoveBackward()
{
	return true;
}
bool Cube::canMoveForward()
{
	return true;
}
bool Cube::canMoveLeft()
{
	return true;
}
bool Cube::canMoveRight()
{
	return true;
}
bool Cube::frameRenderingQueued(const Ogre::FrameEvent& evt)
{

	return true;
}