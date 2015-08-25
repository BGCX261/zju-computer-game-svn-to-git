#include "Cube.h"

#define CUBE_LENGTH 20.0f

static const Ogre::String cubeMesh = "cube.mesh";

extern Matrix mMatrix;

Cube::Cube(SceneManager* mSceneMgr)
:mCubeNode(NULL)
,mCurrentQuat(Quaternion::IDENTITY)
,mNextQuat(Quaternion::IDENTITY)
,mCurrentPos(Vector3::ZERO)
,mNextPos(Vector3::ZERO)
{
	assert(mSceneMgr);
	mCubeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(mCurrentPos,mCurrentQuat);
	for(int i=0;i<MAX_CUBE_IN_ONE_BLOCK;++i)
	{
		mUnitCubeEntity[i] = mSceneMgr->createEntity(cubeMesh);
		mUnitCubeEntity[i]->setCastShadows(true);
		mUnitCubeNode[i] = mCubeNode->createChildSceneNode();
		mUnitCubeNode[i]->attachObject(mUnitCubeEntity[i]);
		mUnitCubeNode[i]->scale(20.0,20.0,20.0);
	}

}
Cube::~Cube(void)
{

}
void Cube::moveTo(const Vector3& v)
{
	if (canMoveTo(v))
		mNextPos = CUBE_LENGTH*v;
}
void Cube::moveX(MoveDirection md /* = POSITIVE */)
{
	if (!mFastDown)
		if (canMoveX(md))
		{
			mMatrix.moveX(md);
			UnitOffset pos = mMatrix.getPosition();
			mNextPos = Vector3(CUBE_LENGTH*pos.x,CUBE_LENGTH*pos.y,CUBE_LENGTH*pos.z);
		}
}
bool Cube::moveY( MoveDirection md /*= POSITIVE*/ )
{
	bool rtnval = false;
	if (!mFastDown)
	{
		if (canMoveY(md))
		{
			mMatrix.moveY(md);
			UnitOffset pos = mMatrix.getPosition();
			mNextPos = Vector3(CUBE_LENGTH*pos.x,CUBE_LENGTH*pos.y,CUBE_LENGTH*pos.z);
			rtnval = true;
		}
	}
	return rtnval;
}
void Cube::moveZ(MoveDirection md /* = POSITIVE */)
{
	if (!mFastDown)
		if (canMoveZ(md))
		{
			mMatrix.moveZ(md);
			UnitOffset pos = mMatrix.getPosition();
			mNextPos = Vector3(CUBE_LENGTH*pos.x,CUBE_LENGTH*pos.y,CUBE_LENGTH*pos.z);
		}
}
void Cube::rotateX(RotateDirection rd /* = COUNTERCLOCKWISE */)
{
	if (!mFastDown)
		if (canRotateX(rd)){
			mMatrix.RotateX(rd);
			Quaternion q;
			q.FromAngleAxis((rd==COUNTERCLOCKWISE?1.0f:-1.0f)*Radian(Degree(90.0f)),Vector3(1.0f,0.0f,0.0f));
			mNextQuat = q*mNextQuat;
		}
}
void Cube::rotateY(RotateDirection rd /* = COUNTERCLOCKWISE */)
{
	if (!mFastDown)
		if (canRotateY(rd)){
			mMatrix.RotateY(rd);
			Quaternion q;
			q.FromAngleAxis((rd==COUNTERCLOCKWISE?1.0f:-1.0f)*Radian(Degree(90.0f)),Vector3(0.0f,1.0f,0.0f));
			mNextQuat = q*mNextQuat;
		}
}
void Cube::rotateZ(RotateDirection rd /* = COUNTERCLOCKWISE */)
{
	if (!mFastDown)
		if (canRotateZ(rd)){
			mMatrix.RotateZ(rd);
			Quaternion q;
			q.FromAngleAxis((rd==COUNTERCLOCKWISE?1.0f:-1.0f)*Radian(Degree(90.0f)),Vector3(0.0f,0.0f,1.0f));
			mNextQuat = q*mNextQuat;
		}
}
bool Cube::canMoveTo(const Vector3& v)
{
	return false;
}
bool Cube::canMoveX(MoveDirection md /* = POSITIVE */)
{
	return mMatrix.canMoveX(md);
}
bool Cube::canMoveY(MoveDirection md /* = POSITIVE */)
{
	return mMatrix.canMoveY(md);
}
bool Cube::canMoveZ(MoveDirection md /* = POSITIVE */)
{
	return mMatrix.canMoveZ(md);
}
bool Cube::canRotateX(RotateDirection rd /* = COUNTERCLOCKWISE */)
{
	return mMatrix.canRotateX(rd);
}
bool Cube::canRotateY(RotateDirection rd /* = COUNTERCLOCKWISE */)
{
	return mMatrix.canRotateY(rd);
}
bool Cube::canRotateZ(RotateDirection rd /* = COUNTERCLOCKWISE */)
{
	return mMatrix.canRotateZ(rd);
}
void Cube::frameRenderingQueued(const FrameEvent& evt)
{
	if ((mNextPos - mCurrentPos).squaredLength() < std::numeric_limits<Real>::epsilon())
	{
		mCurrentPos = mNextPos;
		mFastDown = false;
	}
	else
	{
		mCurrentPos = mNextPos * 0.2 + mCurrentPos * 0.8;
		mCubeNode->setPosition(mCurrentPos);
	}

	if (mCurrentQuat != mNextQuat)
	{
		mCurrentQuat = Quaternion::Slerp(0.2,mCurrentQuat,mNextQuat,true);
		mCubeNode->setOrientation(mCurrentQuat);
	}
}
int Cube::getLowest()
{
	return 0;
}
void Cube::setShape(const UnitOffset off[4])
{
	for (int i=0;i<4;++i)
	{
		mUnitCubeNode[i]->setPosition(Vector3(1.0f*off[i].x,1.0f*off[i].y,1.0f*off[i].z)*CUBE_LENGTH);
	}
}
void Cube::reset()
{
	mMatrix.reset();
	mNextQuat = mCurrentQuat = Quaternion::IDENTITY;
	UnitOffset pos = mMatrix.getPosition();
	mCurrentPos = mNextPos = Vector3(pos.x*CUBE_LENGTH,pos.y*CUBE_LENGTH,pos.z*CUBE_LENGTH);
	mCubeNode->setPosition(mCurrentPos);
	mCubeNode->setOrientation(mCurrentQuat);
	mFastDown = false;
}
void Cube::setMaterial(const MaterialPtr& material)
{
	for (int i=0;i<4;++i)
	{
		mUnitCubeEntity[i]->setMaterial(material);
	}
}
void Cube::setMaterialName(const Ogre::String& name)
{
	MaterialPtr m = MaterialManager::getSingleton().getByName(name);
	setMaterial(m);
}
void Cube::setPosition(int x,int y,int z)
{
	mMatrix.setPosition(x,y,z);
	mNextPos = mCurrentPos = CUBE_LENGTH*Vector3(1.0f*x,1.0f*y,1.0f*z);
	mCubeNode->setPosition(mCurrentPos);
}
void Cube::fastDown()
{
	while(mMatrix.canMoveY(NAGTIVE))
	{
		mMatrix.moveY(NAGTIVE);
		mFastDown = true;
	}
	UnitOffset pos = mMatrix.getPosition();
	mNextPos = Vector3(CUBE_LENGTH*pos.x,CUBE_LENGTH*pos.y,CUBE_LENGTH*pos.z);
}


Cube* CubeManager::createCube(CubeType type)
{
	
	Cube *cube = Cube::getSingletonPtr();
	cube->reset();
	if (mMatrix.setShape(type))
	{
		cube->setShape(mMatrix.getBlocks());
		return cube;
	}
	else
		return NULL;
}

CubeManager::~CubeManager()
{
	delete Cube::getSingletonPtr();
}

CubeManager::CubeManager( SceneManager* mgr )
{
	assert(mgr);
	new Cube(mgr);
}
