#include "audiosystem.h"

#ifdef POKITTO_SFML

float vol = 0;

void AudioSystem::setVolume(float value)
{
    vol = value;
}

float AudioSystem::getVolume()
{
    return vol;
}

void AudioSystem::play(SFX)
{

}

void AudioSystem::playSong(Song)
{

}

#else

#include <cstdint>

#include "Pokitto.h"
#include <LibAudio>

void AudioSystem::setVolume(float value)
{
    Audio::setVolume(value * 128.0f);
}

float AudioSystem::getVolume()
{
    return float(Pokitto::Sound::getVolume()) / 128.0f;
}



Audio::Sink<4, PROJ_AUD_FREQ> audio;

template <int N>
void playOnChannel(SFX sfx) {
    
}

void AudioSystem::play(SFX sfx) {
    static int channel = 0;
    channel = (channel + 1) % 3;
    switch(channel) {
        case 0:
            playOnChannel<1>(sfx);
            break;
        case 1:
            playOnChannel<2>(sfx);
            break;
        case 2:
            playOnChannel<3>(sfx);
            break;
        default:
            break;
    }
}

void AudioSystem::playSong(Song song) {

}

#endif
