#pragma once

#include <string>

#include "miniaudio.h"

#include "AudioClip.h"

class AudioSource
{
private:
    AudioSource();

    ma_engine audio_engine;
    float masterVolume;

    friend class PiXELGraph;

    static void Dispose();

public:
    AudioSource(const AudioSource &) = delete;
    void operator=(const AudioSource &) = delete;

    static AudioSource &GetInstance();

    static void PlaySound(const AudioClip &clip);

    static void SetVolume(float newVolume);
    static float GetVolume();
};