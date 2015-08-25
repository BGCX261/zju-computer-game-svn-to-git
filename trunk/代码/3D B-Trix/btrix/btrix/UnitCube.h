#pragma once
#include <Ogre.h>
using namespace Ogre;


static const int CUBE_LENGTH = 20;

class UnitCube 
{

private:
	static MaterialPtr mMaterial;
	static SceneNode* mParentNode;
	static SceneManager* mSceneMgr;
public:
	static void setParentNode(SceneNode *node){ mParentNode=node; }
	static void setSceneManager(SceneManager* mgr){ mSceneMgr=mgr; }
	static void setMaterial(const MaterialPtr &material){mMaterial = material;}
	static void setMaterialName(const Ogre::String& name){mMaterial = Ogre::MaterialManager::getSingleton().getByName(name);}
	static const MaterialPtr& getMeterial(){ return mMaterial; }
	static SceneNode* createUnitCube(const Vector3& offset = Vector3::ZERO)
	{
		Entity *cube = mSceneMgr->createEntity("cube.mesh");
		cube->setCastShadows(true);
		if (!mMaterial.isNull())
			cube->setMaterial(mMaterial);
		SceneNode *node = mParentNode->createChildSceneNode(offset*CUBE_LENGTH);
		node->attachObject(cube);
		node->scale(18,18,18);

		return node;
	}
	static SceneNode* createUnitCube(int x,int y,int z)
	{
		return createUnitCube(Vector3(1.0f*x,1.0f*y,1.0f*z));
	}
private:
	UnitCube();
	virtual ~UnitCube(void);

};
