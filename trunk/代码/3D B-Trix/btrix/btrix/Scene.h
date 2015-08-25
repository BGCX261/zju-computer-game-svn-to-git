#pragma once

//
// Library Header
//
#include <Ogre.h>
using namespace Ogre;

//
// Project Header
//
#include "Matrix.h"

//typedef Matrix<8,8,16> SceneMatrix;

class Scene
{
public:
	Scene(Ogre::SceneManager*);
	virtual ~Scene(void);

protected:
	//SceneMatrix _matrix;
	Ogre::ManualObject *mEntity;
	Ogre::SceneManager *mSceneMan;
	Ogre::SceneNode *mSceneNode;

	virtual void makeEntity();

public:
	//SceneMatrix getMatrix(){ return _matrix; }
	virtual void generateRandom();
	virtual void updateScene();
    virtual void merge();
    virtual int levelsToDelete();
    virtual void deleteLevels(int count);
	virtual void clear();
};
