#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_

#include <vector>
#include "../Manager.hpp"
#include "../Concrete/ResourceManager.h"

class AudioManager :public Manager<AudioManager>
{
	friend class Manager<AudioManager>;
	
private:
	const MusicPool* musicPool;   //音乐享元池
	const SoundPool* soundPool;   //音效享元池

public:
	void PlayBGM(MusicResID);     //播放音乐
	void PlaySFX(SoundResID);     //播放音效

private:
	AudioManager();
	~AudioManager() = default;
};

#endif
