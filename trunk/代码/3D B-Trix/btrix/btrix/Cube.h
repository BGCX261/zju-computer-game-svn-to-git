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
	virtual void frameRenderingQueued(const FrameEvent& evt);		// ���鶯��ʵ�֣���Ҫ��ÿһ֡�Ļ��ƴ����е��ô˷���

	// ���·����жϷ����Ƿ���Խ�����Ӧ����ת������������Ӧ����ת����
	virtual bool canRotateX(RotateDirection rd = COUNTERCLOCKWISE);
	virtual bool canRotateY(RotateDirection rd = COUNTERCLOCKWISE);
	virtual bool canRotateZ(RotateDirection rd = COUNTERCLOCKWISE);
	virtual void rotateX(RotateDirection rd = COUNTERCLOCKWISE);
	virtual void rotateY(RotateDirection rd = COUNTERCLOCKWISE);
	virtual void rotateZ(RotateDirection rd = COUNTERCLOCKWISE);

	virtual void fastDown();
	// ���·����жϷ����Ƿ���Խ���ƽ�Ʋ�����������Ӧ��ƽ�Ʋ���
	virtual bool canMoveTo(const Vector3& v);
	virtual void moveTo(const Vector3& v);
	virtual bool canMoveX(MoveDirection md = POSITIVE);
	virtual bool canMoveY(MoveDirection md = POSITIVE);
	virtual bool canMoveZ(MoveDirection md = POSITIVE);
	virtual void moveX(MoveDirection md = POSITIVE);
	virtual bool moveY(MoveDirection md = POSITIVE);
	virtual void moveZ(MoveDirection md = POSITIVE);

	// �õ���ǰ��ͷ����λ�ã�����棩
	virtual int getLowest();
	// ���÷���λ��
	virtual void setPosition(int x,int y,int z);
	// �������ã�λ�ú�ת��
	virtual void reset();
	// ���÷�����״������ CUBE_TOTAL_NUM = 8 ��
	virtual void setShape(const UnitOffset off[4]);
	// ���÷������
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


