#ifndef _AUDIO_MANAGER_HPP_
#define _AUDIO_MANAGER_HPP_

#include <vector>
#include "../Manager.hpp"
#include "ResourceManager.hpp"

class AudioManager :public Manager<AudioManager>
{
	friend Manager<AudioManager>;
	
private:
	const MusicPool* musicPool;   //���ֳ�
	const SoundPool* soundPool;   //��Ч��

public:
	void PlayBGM(MusicResID);     //��������
	void PlaySFX(SoundResID);     //������Ч

private:
	AudioManager();
	~AudioManager() = default;
};

void AudioManager::PlayBGM(MusicResID _id)
{
	//��������Mix_Music*��Ч
}

void AudioManager::PlaySFX(SoundResID _id)
{
	//��������Mix_Chunk*��Ч����һ����-1��ʾ������һ�����õ�Channel���в��ţ��ڶ�����ָ��Ҫ���ŵ���Ƶ����������0����ֻ����һ�Σ�-1����ѭ����
	Mix_PlayChannel(-1, soundPool->find(_id)->second, 0);
}

AudioManager::AudioManager()
{
	//��ȡ��Ƶ��Դ
	musicPool = &ResourceManager::Instance()->GetMusicPool();
	soundPool = &ResourceManager::Instance()->GetSoundPool();
}

#endif
