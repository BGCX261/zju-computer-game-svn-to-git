#include "Sound.h"


Sound::Sound(void)
{

}


Sound::~Sound(void)
{
}

void Sound::onGameMenu()
{
    PlaySound(NULL,NULL,SND_FILENAME); // ֹͣ���ڲ��ŵ�����
    PlaySound(file_name[GAME_MENU],NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
}