#ifndef _AUDIO_MANAGER_HPP_
#define _AUDIO_MANAGER_HPP_

#include <vector>
#include "../Manager.hpp"
#include "ResourceManager.hpp"

class AudioManager :public Manager<AudioManager>
{
	friend Manager<AudioManager>;
	
private:
	const MusicPool* musicPool;   //音乐池
	const SoundPool* soundPool;   //音效池

public:
	void PlayBGM(MusicResID);     //播放音乐
	void PlaySFX(SoundResID);     //播放音效

private:
	AudioManager();
	~AudioManager() = default;
};

void AudioManager::PlayBGM(MusicResID _id)
{
	//混音播放Mix_Music*音效
}

void AudioManager::PlaySFX(SoundResID _id)
{
	//混音播放Mix_Chunk*音效，第一参数-1表示任意找一个可用的Channel进行播放，第二参数指代要播放的音频，第三参数0代表只播放一次（-1无限循环）
	Mix_PlayChannel(-1, soundPool->find(_id)->second, 0);
}

AudioManager::AudioManager()
{
	//获取音频资源
	musicPool = &ResourceManager::Instance()->GetMusicPool();
	soundPool = &ResourceManager::Instance()->GetSoundPool();
}

#endif
