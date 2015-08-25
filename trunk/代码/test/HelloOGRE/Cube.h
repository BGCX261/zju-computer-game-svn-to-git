#pragma once
#include <iostream>
#include <vector>
#include <Ogre.h>
#include <OIS.h>
#include <OISKeyboard.h>
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>



#define CUBE_TYPE_COUNT 7


/************************************************************************
   �������һ�������ֻ࣬��ͨ��̬�� getInstence() ������ȡʵ��
 ************************************************************************/
class Cube
{
	enum CubeType{
		TYPE_1 = 0,
		TYPE_2,
		TYPE_3,
		TYPE_4,
		TYPE_5,
		TYPE_6,
		TYPE_7,
		TYPE_8
	};
private:
	static Cube* theCube[CUBE_TYPE_COUNT];
	static Ogre::Real fRotateSpeed;				// ������ת���ٶ�
	static Ogre::Real fFallSpeed;				// �����½����ٶ�
	static Ogre::Real fMoveSpeed;				// ����ƽ�Ƶ��ٶ�
	static Ogre::SceneManager *mScene;			// �ⲿ�ĳ���������
public:
	static void initialize(Ogre::SceneManager *scene);
	static Cube &getCube(Cube::CubeType t)
	{
		//t = ((int)t<CUBE_TYPE_COUNT)?t:(CUBE_TYPE_COUNT-1);
		return *theCube[t];
	}
	static void       rotateSpeed(const Ogre::Real &v){fRotateSpeed = v;}
	static Ogre::Real rotateSpeed(){return fRotateSpeed;}
	static void       fallSpeed(const Ogre::Real &v){fFallSpeed = v;}
	static Ogre::Real fallSpeed(){return fFallSpeed;}
	static void       moveSpeed(const Ogre::Real &v){fMoveSpeed = v;};
	static Ogre::Real moveSpeed(){return fMoveSpeed;}


	typedef Ogre::Matrix4 Block[4];

protected:
	Ogre::Entity *cube;			// �����Ӧ����άʵ��
	Ogre::Vector3 mVelocity;	// ������ٶ�
	Ogre::Vector3 mRotateAxis;	// �������ת��
	Ogre::ColourValue mColor;	// �������ɫ
	Block mBlocks;				// ������ռ��λ��

	// �����ƽ��
	bool		  isMoveLeft;		// �Ƿ�������ƽ��
	bool		  isMoveRight;		// �Ƿ�������ƽ��
	bool		  isMoveForward;	// �Ƿ�����ǰƽ��
	bool		  isMoveBack;		// �Ƿ����ں�ƽ��
	bool		  isMoveUp;			// �Ƿ�������ƽ��
	bool		  isMoveDown;		// �Ƿ�������ƽ��

	// �������ת
	bool          isRotateX;	// �Ƿ�������תX��
	bool          isRotateY;	// �Ƿ�������תY��
	bool          isRotateZ;	// �Ƿ�������תZ��

public:
	virtual void injectKeyDown(const OIS::KeyEvent& evt);
	virtual void injectKeyUp(const OIS::KeyEvent& evt);
	virtual const Block& getBlocks(){return mBlocks;}
	
	virtual ~Cube()
	{
		delete cube;
	}
private:
	Cube(const Ogre::String &meshfile);
	Cube(const Cube&);
	Cube& operator=(const Cube&);

	bool canMoveLeft();
	bool canMoveRight();
	bool canMoveForward();
	bool canMoveBackward();


protected:
	virtual void rotateX();
	virtual void rotateY();
	virtual void rotateZ();
	virtual void moveLeft();
	virtual void moveRight();
	virtual void moveForword();
	virtual void moveBackrword();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual void setColor(const Ogre::ColourValue &c) { mColor = c; }
};
