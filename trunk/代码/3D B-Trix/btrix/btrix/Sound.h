#pragma once
#include <Windows.h>
#include <MMSystem.h>

#pragma comment(lib,"winmm.lib")

enum Snd_Type
{
    GAME_MENU = 0,
    GAME_OVER,
    GAME_BEGIN,
    GAME_MERGE,
    GAME_COLLISION,
    SND_TOTAL
};

const CHAR *file_name[SND_TOTAL] = {
    "1.wav",
    "2.wav"
};

class Sound
{
public:
    Sound(void);
    virtual ~Sound(void);

public:
    virtual void onGameMenu();
    virtual void onGameBegin();
    virtual void onCollision();
    virtual void onMerge();
    virtual void onGameOver();
};

