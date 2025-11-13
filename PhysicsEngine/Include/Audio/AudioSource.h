#pragma once

#include <string>

#include "miniaudio.h"

class Audio
{
private:
    Audio();

    ma_engine audio_engine;

    friend class PiXELGraph;

    static void Dispose();

public:
    Audio(const Audio &) = delete;
    void operator=(const Audio &) = delete;

    static Audio &GetInstance();

    static void PlaySound(std::string file);
};