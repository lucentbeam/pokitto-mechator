#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#ifdef DESKTOP_BUILD
#include <array>
#include <iostream>
#include <fstream>
#include <limits>
#include <vector>

#ifdef  SFML_CORE
#include <SFML/Audio.hpp>
class RawMusic : public sf::SoundStream
{
public:
    bool open(std::string location);
private:
    std::string fname;
    std::ifstream data;
    std::vector<sf::Int16> samples;
    virtual bool onGetData(Chunk& data) override;
    virtual void onSeek(sf::Time timeOffset) override;
};

#else
#include <SDL2/SDL.h>

#endif
#endif

enum SFX {
    sfxConfirm = 0,
    sfxCancel = 1,
    sfxBlip = 2,
    sfxHurt = 3,
    sfxZoomIn = 4,
    sfxZoomOut = 5,
    sfxTimeUp = 6,
    sfxDoor = 7,
    sfxCollect = 8,
    sfxCrash = 9,
    sfxWhoosh = 10,
    sfxStrike = 11,
    sfxJump = 12
};

const int sfx_count = 13;

enum Song {
    musNone = 0,
    musTown,
    musWind,
    musArcade,
    musUpcycle,
    musGame
};

class AudioSystem
{
#ifdef DESKTOP_BUILD
#ifdef SFML_CORE
    static sf::Sound channels[4];
    static int current_channel;
    static RawMusic music[5];
    static std::vector<sf::Int16> raw_buffers[sfx_count];
    static sf::SoundBuffer buffers[sfx_count];
#else
public:
    class RawMusic
    {
    public:
        void open(std::string location);
        Uint8 getData();
        Sint16 getData16();
        void reset();
        int buffer_frames = 0;
    private:
        std::string fname;
        std::ifstream data;
    };

    struct SoundClip {
        float start_time;
        const Sint16 * active_sound;
        int sound_size = 0;
        bool playing = false;

        void set(float time, const Sint16 data[], int size);
    };

    static SDL_AudioDeviceID s_device_id;

    static SoundClip s_clips[4];

    static constexpr float s_audio_freq = 16000;

    static float s_current_time;

    static std::vector<Sint16> raw_buffers[sfx_count];

    static RawMusic music[5];

    static int s_active_music;

#endif
#endif
public:
#ifdef DESKTOP_BUILD
#ifdef SDL_CORE
    static uint64_t s_high_res_counter;
#endif
#endif
    static void initialize();

    static void setVolume(float value);

    static float getVolume();

    static void play(SFX);

    static void playSong(Song);
};

#endif // AUDIOSYSTEM_H
