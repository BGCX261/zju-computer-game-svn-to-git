#include "Scene.h"
#include "Matrix.h"
#include <assert.h>

extern struct TagScene mSceneCubes;
extern Matrix mMatrix;

Scene::Scene(Ogre::SceneManager* mSceneMgr)
:mEntity(NULL)
,mSceneMan(mSceneMgr)
{
	assert(mSceneMan);
	SceneNode *node =  mSceneMan->getRootSceneNode()->createChildSceneNode("Game Borad Node");
	makeEntity();
	node->attachObject(mEntity);
	mSceneNode = node->createChildSceneNode();
	updateScene();
}

Scene::~Scene(void)
{
}
void Scene::generateRandom()
{

}
void Scene::makeEntity()
{
	mEntity = mSceneMan->createManualObject("The Game Borad");
	
	// 下面, begin的第一个参数为要设置的材质名称
	mEntity->begin("Examples/BumpyMetal",RenderOperation::OT_TRIANGLE_FAN);	
	mEntity->position( SCENE_OFFSET_X*CUBE_LENGTH, 0, SCENE_OFFSET_Z*CUBE_LENGTH);	//0
	mEntity->textureCoord(1, 0);
	mEntity->position( SCENE_OFFSET_X*CUBE_LENGTH, 0, -SCENE_OFFSET_Z*CUBE_LENGTH);	//1
	mEntity->textureCoord(0, 0);
	mEntity->position( -SCENE_OFFSET_X*CUBE_LENGTH, 0, -SCENE_OFFSET_Z*CUBE_LENGTH);	//2
	mEntity->textureCoord(0, 1);
	mEntity->position( -SCENE_OFFSET_X*CUBE_LENGTH, 0, SCENE_OFFSET_Z*CUBE_LENGTH);	//3
	mEntity->textureCoord(1, 1);
	mEntity->end();

	// 前面
	mEntity->begin("template/gameboradwall", RenderOperation::OT_TRIANGLE_FAN);
	mEntity->position( -SCENE_OFFSET_X*CUBE_LENGTH, 0, SCENE_OFFSET_Z*CUBE_LENGTH);	//3
	mEntity->textureCoord(1, 0);
	mEntity->position( -SCENE_OFFSET_X*CUBE_LENGTH,SCENE_MAX_WIDTH_Y*CUBE_LENGTH,SCENE_OFFSET_Z*CUBE_LENGTH);    //7
	mEntity->textureCoord(0, 0);
	mEntity->position( SCENE_OFFSET_X*CUBE_LENGTH,SCENE_MAX_WIDTH_Y*CUBE_LENGTH,SCENE_OFFSET_Z*CUBE_LENGTH);    //4
	mEntity->textureCoord(0, 1);
	mEntity->position( SCENE_OFFSET_X*CUBE_LENGTH, 0, SCENE_OFFSET_Z*CUBE_LENGTH);	//0
	mEntity->textureCoord(1, 1);
	mEntity->end();

	/// 右面
	mEntity->begin("template/gameboradwall", RenderOperation::OT_TRIANGLE_FAN);
	mEntity->position( SCENE_OFFSET_X*CUBE_LENGTH, 0, -SCENE_OFFSET_Z*CUBE_LENGTH);	//1
	mEntity->textureCoord(1, 0);
	mEntity->position( SCENE_OFFSET_X*CUBE_LENGTH,SCENE_MAX_WIDTH_Y*CUBE_LENGTH,-SCENE_OFFSET_Z*CUBE_LENGTH);    //5
	mEntity->textureCoord(0, 0);
	mEntity->position( -SCENE_OFFSET_X*CUBE_LENGTH,SCENE_MAX_WIDTH_Y*CUBE_LENGTH,-SCENE_OFFSET_Z*CUBE_LENGTH);    //6
	mEntity->textureCoord(0, 1);
	mEntity->position( -SCENE_OFFSET_X*CUBE_LENGTH, 0, -SCENE_OFFSET_Z*CUBE_LENGTH);	//2
	mEntity->textureCoord(1, 1);
	mEntity->end();
	/// 前面
	mEntity->begin("template/gameboradwall", RenderOperation::OT_TRIANGLE_FAN);
	mEntity->position( SCENE_OFFSET_X*CUBE_LENGTH, 0, -SCENE_OFFSET_Z*CUBE_LENGTH);	//1
	mEntity->textureCoord(1, 0);
	mEntity->position( SCENE_OFFSET_X*CUBE_LENGTH, 0, SCENE_OFFSET_Z*CUBE_LENGTH);	//0
	mEntity->textureCoord(0, 0);
	mEntity->position( SCENE_OFFSET_X*CUBE_LENGTH,SCENE_MAX_WIDTH_Y*CUBE_LENGTH,SCENE_OFFSET_Z*CUBE_LENGTH);    //4
	mEntity->textureCoord(0, 1);
	mEntity->position( SCENE_OFFSET_X*CUBE_LENGTH,SCENE_MAX_WIDTH_Y*CUBE_LENGTH,-SCENE_OFFSET_Z*CUBE_LENGTH);    //5
	mEntity->textureCoord(1, 1);
	mEntity->end();
	/// 后面
	mEntity->begin("template/gameboradwall", RenderOperation::OT_TRIANGLE_FAN);
	mEntity->position( -SCENE_OFFSET_X*CUBE_LENGTH, 0, -SCENE_OFFSET_Z*CUBE_LENGTH);	//2
	mEntity->textureCoord(1, 0);
	mEntity->position( -SCENE_OFFSET_X*CUBE_LENGTH,SCENE_MAX_WIDTH_Y*CUBE_LENGTH,-SCENE_OFFSET_Z*CUBE_LENGTH);    //6
	mEntity->textureCoord(0, 0);
	mEntity->position( -SCENE_OFFSET_X*CUBE_LENGTH,SCENE_MAX_WIDTH_Y*CUBE_LENGTH,SCENE_OFFSET_Z*CUBE_LENGTH);    //7
	mEntity->textureCoord(0, 1);
	mEntity->position( -SCENE_OFFSET_X*CUBE_LENGTH, 0, SCENE_OFFSET_Z*CUBE_LENGTH);	//3
	mEntity->textureCoord(1, 1);
	mEntity->end();
	
	mEntity->setCastShadows(false);
}
void Scene::updateScene()
{
	mSceneNode->removeAllChildren();
	for (int x=-4;x < SCENE_OFFSET_X;++x)
	{
		for (int y=0;y < SCENE_MAX_WIDTH_Y;++y)
		{
			for (int z=-4;z < SCENE_OFFSET_Z;++z)
			{
				if (mSceneCubes.isOccupy(x,y,z))
				{
					Entity *cube = mSceneMan->createEntity("cube.mesh");
					cube->setMaterialName("template/unitcube");
					SceneNode *node = mSceneNode->createChildSceneNode(
						Vector3((x)*CUBE_LENGTH,
								(y)*CUBE_LENGTH,
								(z)*CUBE_LENGTH));
					node->attachObject(cube);
					node->scale(CUBE_LENGTH,CUBE_LENGTH,CUBE_LENGTH);
				}
			}
		}
	}
}
void Scene::merge()
{
	const UnitOffset *cubes = mMatrix.getBlocks();
	UnitOffset pos = mMatrix.getPosition();
    for(int i=0;i<MAX_CUBE_IN_ONE_BLOCK;++i)
    {
        mSceneCubes.occupy(cubes[i].x+pos.x,cubes[i].y+pos.y,cubes[i].z+pos.z);
    }
    updateScene();
}
int Scene::levelsToDelete()
{
    return mSceneCubes.levelsToDelete();
}
void Scene::deleteLevels(int count)
{
    mSceneCubes.deleteLevels(count);
    
    updateScene();
}

void Scene::clear()
{
	mSceneCubes.reset();
	updateScene();
}
