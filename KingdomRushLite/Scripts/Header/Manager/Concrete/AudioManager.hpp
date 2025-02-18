#ifndef _AUDIO_MANAGER_HPP_
#define _AUDIO_MANAGER_HPP_

#include <vector>
#include "../Manager.hpp"
#include "../Concrete/ResourceManager.hpp"

class AudioManager :public Manager<AudioManager>
{
	friend Manager<AudioManager>;
	
private:
	const MusicPool* musicPool;   //“Ù¿÷≥ÿ
	const SoundPool* soundPool;   //“Ù–ß≥ÿ

public:
	void PlayBGM(MusicResID);     //≤•∑≈“Ù¿÷
	void PlaySFX(SoundResID);     //≤•∑≈“Ù–ß

private:
	AudioManager();
	~AudioManager() = default;
};

#endif
