#include <string>
#include <filesystem>

#include "Console/ErrorSystem.h"

#define MINIAUDIO_IMPLEMENTATION
#include "Audio/AudioSource.h"

Audio::Audio()
{
    ma_engine_init(NULL, &audio_engine);
}

void Audio::Dispose()
{
    auto &instance = Audio::GetInstance();

    ma_engine_uninit(&instance.audio_engine);
}

Audio &Audio::GetInstance()
{
    static Audio instance;
    return instance;
}

void Audio::PlaySound(std::string file)
{
    if(!std::filesystem::exists(file)) 
        throw::Error("File " + file + " can not be found");

    auto &instance = Audio::GetInstance();

    ma_engine_play_sound(&instance.audio_engine, file.c_str(), NULL);
}
