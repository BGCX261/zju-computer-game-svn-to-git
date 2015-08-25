#include "Sound.h"


Sound::Sound(void)
{

}


Sound::~Sound(void)
{
}

void Sound::onGameMenu()
{
    PlaySound(NULL,NULL,SND_FILENAME); // 停止正在播放的声音
    PlaySound(file_name[GAME_MENU],NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
}