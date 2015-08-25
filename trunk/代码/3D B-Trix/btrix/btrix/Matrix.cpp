#include "Matrix.h"


struct TagScene mSceneCubes;
Matrix mMatrix;


bool Matrix::setShape(CubeType type)
{
	bool val = true;
	for (int i=0;i<MAX_CUBE_IN_ONE_BLOCK;++i)
	{
		mBlocks[i] = mCubeShape[type][i];
		if (mSceneCubes.isOccupy(mBlocks[i].x + mPosition.x,mBlocks[i].y + mPosition.y,mBlocks[i].z + mPosition.z))
		{
			val = false;
			break;
		}
	}
	return val;
}


void Matrix::RotateX(RotateDirection rd/* = COUNTERCLOCKWISE */)
{
    if (rd == COUNTERCLOCKWISE){    // (x0,y0,z0) ==> (x0,-z0-1,y0)
	    for(int i=0;i<MAX_CUBE_IN_ONE_BLOCK;++i) 
	    {
            int t = mBlocks[i].y;
		    mBlocks[i].y = -mBlocks[i].z-1;
            mBlocks[i].z = t;
	    }
    }
    else                            // (x0,y0,z0) ==> (x0,z0,-y0-1)
    {
        for(int i=0;i<MAX_CUBE_IN_ONE_BLOCK;++i)
        {
            int t = mBlocks[i].z;
            mBlocks[i].z = -mBlocks[i].y-1;
            mBlocks[i].y = t;
        }
    }
}
void Matrix::RotateY(RotateDirection rd/* = COUNTERCLOCKWISE */)
{
    if (rd == COUNTERCLOCKWISE){    // (x0,y0,z0) ==> (z0,y0,-x0-1)
	    for(int i=0;i<MAX_CUBE_IN_ONE_BLOCK;++i) 
	    {
            int t = mBlocks[i].z;
		    mBlocks[i].z = -mBlocks[i].x-1;
            mBlocks[i].x = t;
	    }
    }
    else                            // (x0,y0,z0) ==> (-z0-1,y0,x0)
    {
        for(int i=0;i<MAX_CUBE_IN_ONE_BLOCK;++i)
        {
            int t = mBlocks[i].x;
            mBlocks[i].x = -mBlocks[i].z-1;
            mBlocks[i].x = t;
        }
    }
}
void Matrix::RotateZ(RotateDirection rd/* = COUNTERCLOCKWISE */)
{
    if (rd == COUNTERCLOCKWISE){    // (x0,y0,z0) ==> (-y0-1,x0,z0)
	    for(int i=0;i<MAX_CUBE_IN_ONE_BLOCK;++i) 
	    {
            int t = mBlocks[i].x;
		    mBlocks[i].x = -mBlocks[i].y-1;
            mBlocks[i].y = t;
	    }
    }
    else                            // (x0,y0,z0) ==> (y0,-x0-1,z0)
    {
        for(int i=0;i<MAX_CUBE_IN_ONE_BLOCK;++i)
        {
            int t = mBlocks[i].y;
            mBlocks[i].y = -mBlocks[i].x-1;
            mBlocks[i].x = t;
        }
    }
}
bool Matrix::canRotateX(RotateDirection rd)
{
    Matrix mat = *this;
    mat.RotateX(rd);
    const UnitOffset *cubes = mat.getBlocks();
	UnitOffset pos = mat.getPosition();
	for(int i=0;i<MAX_CUBE_IN_ONE_BLOCK;++i)
	{
		if (mSceneCubes.isOccupy(cubes[i].x+pos.x,cubes[i].y+pos.y,cubes[i].z+pos.z))
			return false;
	}
    return true;
}
bool Matrix::canRotateY(RotateDirection rd)
{
    Matrix mat = *this;
    mat.RotateY(rd);
    const UnitOffset *cubes = mat.getBlocks();
	UnitOffset pos = mat.getPosition();
	for(int i=0;i<MAX_CUBE_IN_ONE_BLOCK;++i)
	{
		if (mSceneCubes.isOccupy(cubes[i].x+pos.x,cubes[i].y+pos.y,cubes[i].z+pos.z))
			return false;
	}
    return true;
}
bool Matrix::canRotateZ(RotateDirection rd)
{
    Matrix mat = *this;
    mat.RotateZ(rd);
    const UnitOffset *cubes = mat.getBlocks();
	UnitOffset pos = mat.getPosition();
    for(int i=0;i<MAX_CUBE_IN_ONE_BLOCK;++i)
    {
        if (mSceneCubes.isOccupy(cubes[i].x+pos.x,cubes[i].y+pos.y,cubes[i].z+pos.z))
            return false;
    }
    return true;
}

void Matrix::moveX(MoveDirection md)
{
    int delta = 1;
    if (md == NAGTIVE)
        delta = -1;
    mPosition.x += delta;
}

void Matrix::moveY(MoveDirection md)
{
    int delta = 1;
    if (md == NAGTIVE)
        delta = -1;
    mPosition.y += delta;
}

void Matrix::moveZ(MoveDirection md)
{
    int delta = 1;
    if (md == NAGTIVE)
        delta = -1;
    mPosition.z += delta;
}

bool Matrix::canMoveX(MoveDirection md)
{
    Matrix mat = *this;
    mat.moveX(md);
    const UnitOffset *cubes = mat.getBlocks();
	UnitOffset pos = mat.getPosition();
    for(int i=0;i<MAX_CUBE_IN_ONE_BLOCK;++i)
    {
        if (mSceneCubes.isOccupy(cubes[i].x+pos.x,cubes[i].y+pos.y,cubes[i].z+pos.z))
            return false;
    }
    return true;
}

bool Matrix::canMoveY(MoveDirection md)
{
    Matrix mat = *this;
    mat.moveY(md);
    const UnitOffset *cubes = mat.getBlocks();
	UnitOffset pos = mat.getPosition();
	for(int i=0;i<MAX_CUBE_IN_ONE_BLOCK;++i)
	{
		if (mSceneCubes.isOccupy(cubes[i].x+pos.x,cubes[i].y+pos.y,cubes[i].z+pos.z))
			return false;
	}
    return true;
}

bool Matrix::canMoveZ(MoveDirection md)
{
    Matrix mat = *this;
    mat.moveZ(md);
    const UnitOffset *cubes = mat.getBlocks();
	UnitOffset pos = mat.getPosition();
	for(int i=0;i<MAX_CUBE_IN_ONE_BLOCK;++i)
	{
		if (mSceneCubes.isOccupy(cubes[i].x+pos.x,cubes[i].y+pos.y,cubes[i].z+pos.z))
			return false;
	}
    return true;
}

