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

#ifdef WIN32
#include <SDL.h>
#undef main
#else
#include <SDL2/SDL.h>
#endif

#endif
#endif

enum SFX {
    sfxConfirm = 0,
    sfxCancel = 1,
    sfxSelect = 2,
    sfxDeny = 3,
    sfxEnemyShoot = 4,
    sfxEnemyShootMedium = 5,
    sfxExplosionSmall = 6,
    sfxExplosionBig = 7,
    sfxGetDollar = 8,
    sfxGetItem = 9,
    sfxGrenade = 10,
    sfxLaser = 11,
    sfxLaserCharge = 12,    
    sfxMissile = 13,
    sfxPlayerGun = 14,
    sfxPlayerGun2x = 15,
    sfxHit1 = 16,
    sfxHit2 = 17,
    sfxRing = 18,
    sfxBomber = 19,

    sfxCount
};
extern const char * sfx_names[];

enum Song {
    musNone = 0,
    musCape = 1,
    musOverworld = 2,
    musOverworldMain = 3,
    musBoss = 4,
    musOcean = 5,
    musCanyon = 6,
    musSwamp = 7,

    musCount
};
extern const char * music_names[];

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

    static std::vector<Sint16> raw_buffers[sfxCount];

    static RawMusic music[musCount];

#endif
#endif
public:
#ifdef DESKTOP_BUILD
#ifdef SDL_CORE
    static uint64_t s_high_res_counter;
#endif
#endif
    static void initialize();

    static void setMusicFraction(float fraction);

    static void setVolume(float value);

    static float getVolume();

    static void play(SFX);

    static void playSong(Song);

    static void setMusicOn(bool on);
    static void setSfxOn(bool on);

    static int s_active_music;
    static bool sfx_on;
    static bool mus_on;
};

#endif // AUDIOSYSTEM_H
