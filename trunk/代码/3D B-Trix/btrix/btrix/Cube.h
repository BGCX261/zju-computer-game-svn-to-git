#pragma once

//
// System Header
//
#include<assert.h>

//
// Library Header
//
#include<Ogre.h>
using namespace Ogre;

//
// Project Header
//
#include "Matrix.h"

using namespace Ogre;

class Cube;
class CubeManager;

extern struct TagScene mSceneCubes;

class Cube:public Ogre::Singleton<Cube>
{

public:
	virtual ~Cube(void);
	explicit Cube(SceneManager*);

protected:
	Vector3 mCurrentPos;
	Vector3 mNextPos;
	Quaternion mCurrentQuat;
	Quaternion mNextQuat;
	SceneNode *mCubeNode;
	ColourValue mColor;
	SceneNode* mUnitCubeNode[4];
	Entity* mUnitCubeEntity[4];
	bool mFastDown;
public:
	virtual void frameRenderingQueued(const FrameEvent& evt);		// 方块动画实现，需要在每一帧的绘制代码中调用此方法

	// 以下方法判断方块是否可以进行相应的旋转操作，并作相应的旋转操作
	virtual bool canRotateX(RotateDirection rd = COUNTERCLOCKWISE);
	virtual bool canRotateY(RotateDirection rd = COUNTERCLOCKWISE);
	virtual bool canRotateZ(RotateDirection rd = COUNTERCLOCKWISE);
	virtual void rotateX(RotateDirection rd = COUNTERCLOCKWISE);
	virtual void rotateY(RotateDirection rd = COUNTERCLOCKWISE);
	virtual void rotateZ(RotateDirection rd = COUNTERCLOCKWISE);

	virtual void fastDown();
	// 以下方法判断方块是否可以进行平移操作，并作相应的平移操作
	virtual bool canMoveTo(const Vector3& v);
	virtual void moveTo(const Vector3& v);
	virtual bool canMoveX(MoveDirection md = POSITIVE);
	virtual bool canMoveY(MoveDirection md = POSITIVE);
	virtual bool canMoveZ(MoveDirection md = POSITIVE);
	virtual void moveX(MoveDirection md = POSITIVE);
	virtual bool moveY(MoveDirection md = POSITIVE);
	virtual void moveZ(MoveDirection md = POSITIVE);

	// 得到当前最低方块的位置（距地面）
	virtual int getLowest();
	// 设置方块位置
	virtual void setPosition(int x,int y,int z);
	// 方块重置，位置和转向
	virtual void reset();
	// 设置方块形状，共有 CUBE_TOTAL_NUM = 8 种
	virtual void setShape(const UnitOffset off[4]);
	// 设置方块材质
	virtual void setMaterial(const MaterialPtr& material);
	virtual void setMaterialName(const Ogre::String& name);
};
Cube* Singleton<Cube>::ms_Singleton = NULL;

class CubeManager:public Ogre::Singleton<CubeManager>
{
public:

	explicit CubeManager(SceneManager* mgr);
	~CubeManager();

	Cube *createCube(CubeType type);

};

CubeManager* Singleton<CubeManager>::ms_Singleton = NULL;


