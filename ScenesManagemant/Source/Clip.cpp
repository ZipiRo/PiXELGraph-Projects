#include "Audio/AudioClip.h"

AudioClip::AudioClip()
{
    volume = 1.0f;
}

AudioClip::AudioClip(const std::string &audioFile, float volume)
{
    audioFilePath = audioFile;
    this->volume = volume;
}

void AudioClip::SetVolume(float newVolume)
{
    volume = newVolume;
}

float AudioClip::GetVolume()
{
    return volume;
}

void AudioClip::SetAudioFile(const std::string &audioFile)
{
    audioFilePath = audioFile;
}
