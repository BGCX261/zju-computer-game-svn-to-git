#pragma once

/*===========================================================================*\

				Y
				^
				|   /------/------/------/------/
				|  /------/------/------/------/|
				| /------/------/------/------/|/
				|/------/------/------/------/|/|
				/------/------/------/------/|/|/
				|      |      |      |      |/|/|
				+------+------+------+------+|/|/
				|      |      |      |      |/|/|
				+------+------+------+------+|/|/
				|      |      |      |      |/|/
				+------+------+------+------+|/
				|      |      |      |      |/
				+------+------+------+------+-----------> X
			   /
		      /
		     /
		    /
		   v
		   Z
\*===========================================================================*/

#define MAX_CUBE_IN_ONE_BLOCK 4		// 一个形状最多拥有的小方块数

#define CUBE_LENGTH 20.0f

#define SCENE_MAX_WIDTH_X 8
#define SCENE_MAX_WIDTH_Y 20
#define SCENE_MAX_WIDTH_Z 8

#define SCENE_OFFSET_X (SCENE_MAX_WIDTH_X / 2)
#define SCENE_OFFSET_Y 0
#define SCENE_OFFSET_Z (SCENE_MAX_WIDTH_Z / 2)

typedef enum{
	CUBE_TYPE_0 = 0,
	CUBE_TYPE_1,
	CUBE_TYPE_2,
	CUBE_TYPE_3,
	CUBE_TYPE_4,
	CUBE_TYPE_5,
	CUBE_TYPE_6,
	CUBE_TYPE_7,
	CUBE_TOTAL_NUM
}CubeType;

typedef enum{
	CLOCKWISE,
	COUNTERCLOCKWISE,
}RotateDirection;

typedef enum{
	POSITIVE,
	NAGTIVE
}MoveDirection;

typedef struct TagUnit{
	int x,y,z;
	TagUnit(int _x=0,int _y=0,int _z=0){
		x = _x;
		y = _y;
		z = _z;
	}
}UnitOffset;

static const UnitOffset mCubeShape[CUBE_TOTAL_NUM][MAX_CUBE_IN_ONE_BLOCK] = {
	{TagUnit(0,0,0),TagUnit(-1,0,0),TagUnit(0,0,-1),TagUnit(-1,0,-1)},
	{TagUnit(0,0,0),TagUnit(0,0,1),TagUnit(0,0,-1),TagUnit(0,0,-2)},
	{TagUnit(0,0,0),TagUnit(1,0,0),TagUnit(2,0,0),TagUnit(0,0,1)},
	{TagUnit(0,0,0),TagUnit(1,0,0),TagUnit(0,0,1),TagUnit(0,0,-1)},
	{TagUnit(0,0,0),TagUnit(-1,0,0),TagUnit(0,0,1),TagUnit(-1,0,-1)},
	{TagUnit(0,0,0),TagUnit(0,-1,0),TagUnit(-1,0,0),TagUnit(0,0,-1)},
	{TagUnit(0,0,0),TagUnit(-1,0,0),TagUnit(0,0,-1),TagUnit(0,-1,-1)},
	{TagUnit(0,0,0),TagUnit(-1,0,0),TagUnit(0,0,-1),TagUnit(-1,-1,0)}
};

struct TagScene{
	int mCubes[10][22][10];
	TagScene(){
		reset();
	};
	inline void reset()
	{
		for (int x=0;x<10;++x)
		{
			for (int y=0;y<22;++y)
			{
				for (int z=0;z<10;++z)
				{
					if (x == 0 || x == 9 || y == 0 || z == 0 || z == 9)
						mCubes[z][y][x] = 1;
					else
						mCubes[z][y][x] = 0;
				}
			}
		}
	}
	inline void occupy(int x,int y,int z)
	{
		mCubes[z+5][y+1][x+5] = 1;
	}
	inline void unOccupy(int x,int y,int z)
	{
		mCubes[z+5][y+1][x+5] = 0;
	}
	inline bool isOccupy(int x,int y,int z)
	{
		return mCubes[z+5][y+1][x+5] == 1;
	}
    inline int levelsToDelete()
    {
        int count = 0;
        for(int y = 1;y <= 20; ++y)
        {
            for(int z = 1;z <= 8; ++z)
            {
                for(int x = 1;x <= 8; ++x)
                {
                    if (mCubes[z][y][x] == 0)
                        goto Out;
                }
            }
            ++count;
        }
Out:
        return count;
    }
    inline void deleteLevels(int count)
    {
        if (count<=0)
            return;
        int size = 20 - count;
        for(int y=1;y<=size;++y)
        {
            for(int z=1;z<=8;++z)
            {
                for(int x=1;x<=8;++x)
                {
                    mCubes[z][y][x] = mCubes[z][y+count][x];
                }
            }
        }
        for(int y=size+1;y<=20;++y)
        {
            for(int z=1;z<=8;++z)
            {
                for(int x=1;x<=8;++x)
                {
                    mCubes[z][y][x] = 0;
                }
            }
        }
    }
};

class Matrix
{
public:
	Matrix()
    :mPosition(0,20,0)
    {
		
	}
	virtual ~Matrix(void){};
	const UnitOffset * getBlocks()
	{
		return mBlocks;
	}
	const UnitOffset& getPosition()
	{
		return mPosition;
	}
	void setPosition(int x,int y,int z)
	{
		mPosition.x = x;
		mPosition.y = y;
		mPosition.z = z;
	}
    inline void reset()
    {
        mPosition.x = 0;
        mPosition.y = 20;
        mPosition.z = 0;
    }
	bool setShape(CubeType type);
protected:
	UnitOffset mPosition;
	UnitOffset mBlocks[MAX_CUBE_IN_ONE_BLOCK];

public:

	virtual bool canRotateX(RotateDirection rd = COUNTERCLOCKWISE);
	virtual bool canRotateY(RotateDirection rd = COUNTERCLOCKWISE);
	virtual bool canRotateZ(RotateDirection rd = COUNTERCLOCKWISE);
	virtual void RotateX(RotateDirection rd = COUNTERCLOCKWISE);
	virtual void RotateY(RotateDirection rd = COUNTERCLOCKWISE);
	virtual void RotateZ(RotateDirection rd = COUNTERCLOCKWISE);
    virtual bool canMoveX(MoveDirection md = POSITIVE);
    virtual bool canMoveY(MoveDirection md = POSITIVE);
    virtual bool canMoveZ(MoveDirection md = POSITIVE);
    virtual void moveX(MoveDirection md = POSITIVE);
    virtual void moveY(MoveDirection md = POSITIVE);
    virtual void moveZ(MoveDirection md = POSITIVE);
};
