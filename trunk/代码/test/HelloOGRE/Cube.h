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
   这个类是一个单例类，只能通静态的 getInstence() 方法获取实例
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
	static Ogre::Real fRotateSpeed;				// 方块旋转的速度
	static Ogre::Real fFallSpeed;				// 方块下降的速度
	static Ogre::Real fMoveSpeed;				// 方块平移的速度
	static Ogre::SceneManager *mScene;			// 外部的场景管理器
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
	Ogre::Entity *cube;			// 方块对应的三维实体
	Ogre::Vector3 mVelocity;	// 方块的速度
	Ogre::Vector3 mRotateAxis;	// 方块的旋转轴
	Ogre::ColourValue mColor;	// 方块的颜色
	Block mBlocks;				// 方块所占的位置

	// 方块的平移
	bool		  isMoveLeft;		// 是否正在左平移
	bool		  isMoveRight;		// 是否正在右平移
	bool		  isMoveForward;	// 是否正在前平移
	bool		  isMoveBack;		// 是否正在后平移
	bool		  isMoveUp;			// 是否正在上平移
	bool		  isMoveDown;		// 是否正在下平移

	// 方块的旋转
	bool          isRotateX;	// 是否正在旋转X轴
	bool          isRotateY;	// 是否正在旋转Y轴
	bool          isRotateZ;	// 是否正在旋转Z轴

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
