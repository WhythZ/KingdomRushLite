#ifndef _AUDIO_MANAGER_HPP_
#define _AUDIO_MANAGER_HPP_

#include <vector>
#include "../Manager.hpp"
#include "../Concrete/ResourceManager.hpp"

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

#endif
