#include <string>
#include <filesystem>

#include "Console/ErrorSystem.h"

#define MINIAUDIO_IMPLEMENTATION
#include "Audio/AudioSource.h"

#include "Audio/AudioClip.h"

AudioSource::AudioSource()
{
    ma_engine_init(NULL, &audio_engine);
    masterVolume = 1.0f;
}

void AudioSource::Dispose()
{
    auto &instance = AudioSource::GetInstance();

    ma_engine_uninit(&instance.audio_engine);
}

AudioSource &AudioSource::GetInstance()
{
    static AudioSource instance;
    return instance;
}

void AudioSource::PlaySound(const AudioClip &audioClip)
{
    if(audioClip.audioFilePath.empty())
        return;
        
    auto &instance = AudioSource::GetInstance();

    if(instance.masterVolume == 0) 
        return;

    if(!std::filesystem::exists(audioClip.audioFilePath)) 
        throw Error("File " + audioClip.audioFilePath + " can not be found");

    float volume = instance.masterVolume * audioClip.volume;

    ma_engine_set_volume(&instance.audio_engine, volume);
    ma_engine_play_sound(&instance.audio_engine, audioClip.audioFilePath.c_str(), NULL);
}

void AudioSource::SetVolume(float newVolume)
{
    auto &instance = AudioSource::GetInstance();
    instance.masterVolume = newVolume;
}

float AudioSource::GetVolume()
{
    auto &instance = AudioSource::GetInstance();
    return instance.masterVolume;
}
