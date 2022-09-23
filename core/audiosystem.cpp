#include "audiosystem.h"

#include <string>

#ifdef DESKTOP_BUILD

float vol = 100;

float mus_frac = 1.0f;

#ifdef SFML_CORE
sf::Sound AudioSystem::channels[4];
int AudioSystem::current_channel = 0;
RawMusic AudioSystem::music[5];
std::vector<sf::Int16> AudioSystem::raw_buffers[sfx_count];
sf::SoundBuffer AudioSystem::buffers[sfx_count];


bool RawMusic::open(std::string location)
{
    fname = location;
    for(int i = 0; i < 2048; ++i) {
        samples.push_back(0);
    }
    initialize(1, 22500);
    return true;
}

bool RawMusic::onGetData(sf::SoundStream::Chunk &chunk)
{
    if (!data.is_open()) data.open(fname);
    uint8_t raw;
    for(int i = 0; i < 2048; ++i) {
        if (data.peek() == EOF) {
            data.seekg(0);
        }
        data.read(reinterpret_cast<char*>(&raw), 1);
        samples[i] = ((float(raw) - 128.0f)/128.0f * std::numeric_limits<int16_t>::max());
    }
    chunk.samples = samples.data();
    chunk.sampleCount = 2048;
    return true;
}

void RawMusic::onSeek(sf::Time timeOffset)
{
    if (!data.is_open()) data.open(fname);
//    position = timeOffset.asMilliseconds();
    data.seekg(0);
}

void AudioSystem::initialize()
{
    std::string path = "../data/buttoncity/";
    std::string names[] = {"confirm","cancel","blip","hurt","zoomin","zoomout","timeup","door","collect","crash","woosh","strike","jump"};
    for(int i = 0; i < sfx_count; ++i) {
        std::ifstream input;
        input.open(path + names[i] + ".raw", std::ios::binary | std::ios::in);
        if (!input.is_open()){
            printf("could not open file %s\n",names[i]);
            continue;
        }
        uint8_t raw;
        while( input.peek() != EOF )
        {
            input.read(reinterpret_cast<char*>(&raw), 1);
            raw_buffers[i].push_back((float(raw) - 128.0f)/128.0f * std::numeric_limits<int16_t>::max());
        }
        if (!buffers[i].loadFromSamples(raw_buffers[i].data(), raw_buffers[i].size(), 1, 22500)) {
            std::cout << "Could not read buffer " << std::endl;
        }
        input.close();
    }

    std::string musics[] = {"bctown", "wind", "bcarcade", "bcupcycle", "prismabops"};
    for(int i = 0; i < 5; ++i) {
        music[i].open(path + musics[i] + ".raw");
    }
}

void AudioSystem::play(SFX sfx)
{
    current_channel = (current_channel + 1) % 4;
    channels[current_channel].setBuffer(buffers[int(sfx)]);
    channels[current_channel].setVolume(vol);
    channels[current_channel].play();
}

void AudioSystem::playSong(Song song)
{
    for(int i = 0; i < 5; ++i) {
        music[i].stop();
        music[i].setVolume(vol);
    }
    if (song != musNone) {
        music[int(song) - 1].play();
    }
}
#else

AudioSystem::SoundClip AudioSystem::s_clips[4];

std::vector<Sint16> AudioSystem::raw_buffers[sfx_count];

AudioSystem::RawMusic AudioSystem::music[song_count];

float AudioSystem::s_current_time = 0.0f;

int AudioSystem::s_active_music = 0;

SDL_AudioDeviceID AudioSystem::s_device_id;

uint64_t AudioSystem::s_high_res_counter = 0;

void AudioSystem::RawMusic::open(std::string location)
{
    fname = location;
}

Uint8 AudioSystem::RawMusic::getData()
{
    if (!data.is_open()) data.open(fname);
    if (data.peek() == EOF) {
        data.seekg(0);
    }
    if (buffer_frames > 0) {
        --buffer_frames;
        return 128;
    }
    Uint8 raw;
    data.read(reinterpret_cast<char*>(&raw), 1);
    return raw;
}

Sint16 AudioSystem::RawMusic::getData16()
{
    if (!data.is_open()) data.open(fname, std::ifstream::binary | std::ifstream::in);
    if (data.peek() == EOF || !data.good()) {
        data.seekg(0);
    }
    if (buffer_frames > 0) {
        --buffer_frames;
        return 0;
    }
    Sint16 raw;
    data.read(reinterpret_cast<char*>(&raw), sizeof(Sint16));
    return raw;
}

void AudioSystem::RawMusic::reset()
{
    if (!data.is_open()) {
        data.open(fname, std::ifstream::binary | std::ifstream::in);
    }
    else data.seekg(0);
}

void AudioSystem::SoundClip::set(float time, const Sint16 data[], int size)
{
    start_time = time;
    active_sound = data;
    sound_size = size;
    playing = true;
}

void audioCallback(void *userdata, Uint8 * b, int len) {
    int16_t * buffer = reinterpret_cast<int16_t*>(b);
    len = len/2;
    static int callnum = 0;
    callnum++;
    for (unsigned long int i = 0; i < len; ++i) {
        float out = 0;
        int count = 0;
        for(int j = 0; j < 4; ++j) {
            if (AudioSystem::s_clips[j].playing) {
                float delta = AudioSystem::s_current_time - AudioSystem::s_clips[j].start_time;
                if (delta < 0) {
                    continue;
                }
                size_t idx = size_t(AudioSystem::s_audio_freq * delta + i);
                if (idx < AudioSystem::s_clips[j].sound_size) {
                    count++;
                    out += (float(AudioSystem::s_clips[j].active_sound[idx]));
                } else {
                    AudioSystem::s_clips[j].playing = false;
                }
            }
        }

        if (AudioSystem::s_active_music > 0) {
            out += float(AudioSystem::music[AudioSystem::s_active_music - 1].getData16()) * mus_frac;
        }

        out /= 5;
        *buffer++ = int16_t(out * vol / 100);
    }
    AudioSystem::s_current_time += float(len) / AudioSystem::s_audio_freq;
    AudioSystem::s_high_res_counter = SDL_GetPerformanceCounter();
}

void AudioSystem::initialize() {
    SDL_AudioSpec spec;
    spec.callback = audioCallback;
    spec.samples = 512;
    spec.channels = 1;
    spec.freq = int(s_audio_freq);
    spec.format = AUDIO_S16;
    SDL_AudioSpec result;
    s_device_id = SDL_OpenAudioDevice(NULL, 0, &spec, &result, 0);

#ifdef DESKTOP_BUILD
#ifdef WIN32
#ifdef DEBUGS
    std::string path = "../content/sfx_desktop/";
#else
    std::string path = "data/mechator/";
#endif
#else
    std::string path = "../pokitto-mechator/content/sfx_desktop/";
#endif
#endif
    std::string names[] = {"confirm","cancel","select","deny","enemy_shoot","enemy_shoot_big","explosion_small","explosion_big","get_dollar","get_item","grenade","laser","laser_charge", "missile", "playergun", "playergun_2x", "playerhit", "playerhit2"};
    for(int i = 0; i < sfx_count; ++i) {
        std::ifstream input;
        input.open(path + names[i] + ".raw", std::ios::binary | std::ios::in);
        if (!input.is_open()){
            printf("could not open file %s\n",names[i].c_str());
            continue;
        }
        int16_t raw;
        while( input.peek() != EOF )
        {
            input.read(reinterpret_cast<char*>(&raw), sizeof(raw));
            raw_buffers[i].push_back(raw);
        }
        input.close();
    }

    std::string musics[] = {"storm", "overworld", "overworld_main", "boss"};
    for(int i = 0; i < song_count; ++i) {
        music[i].open(path + musics[i] + ".raw");
    }

    if (s_device_id == 0) {
        SDL_Log("Failed to open audio: %s", SDL_GetError());
    } else {
        if (result.format != spec.format) { /* we let this one thing change. */
            SDL_Log("We didn't get Uint8 audio format.");
        }
        SDL_PauseAudioDevice(s_device_id, 0); /* start audio playing. */
    }
    AudioSystem::s_high_res_counter = SDL_GetPerformanceCounter();
}

void AudioSystem::play(SFX sfx)
{
    for (int i = 0; i < 4; i++) {
        if (!s_clips[i].playing) {
            s_clips[i].set(s_current_time, raw_buffers[int(sfx)].data(), raw_buffers[int(sfx)].size());
            return;
        }
    }
}

void AudioSystem::playSong(Song song)
{
    if (s_active_music == int(song)) return;
    SDL_LockAudio();
    s_active_music = int(song);
    if (s_active_music == musNone)
    {
        SDL_UnlockAudio();
        return;
    }

    music[s_active_music-1].reset();

    uint64_t t = SDL_GetPerformanceCounter();
    double delta = double(t - s_high_res_counter) * 1000;
    delta /= double(SDL_GetPerformanceFrequency());

    double spms = 512.0f / float(s_audio_freq) * 1000;
    music[s_active_music-1].buffer_frames = spms * delta;
    SDL_UnlockAudio();
}

#endif

void AudioSystem::setMusicFraction(float fraction)
{
    mus_frac = fraction;
}

void AudioSystem::setVolume(float value)
{
    vol = value;
}

float AudioSystem::getVolume()
{
    return vol;
}

#else

#include <cstdint>

#include "Pokitto.h"
#include <LibAudio>

void AudioSystem::initialize()
{

}

void AudioSystem::setVolume(float value)
{
    Audio::setVolume(value * 128.0f);
}

float AudioSystem::getVolume()
{
    return float(Pokitto::Sound::getVolume()) / 128.0f;
}

void AudioSystem::setMusicFraction(float fraction) {
    //mus_frac = fraction;
}

Audio::Sink<6, PROJ_AUD_FREQ> audio;

template <int N>
void playOnChannel(const char * path) {
    Audio::play<N>(path)->setLoop(false);
}

void AudioSystem::play(SFX sfx) {
    std::string path = "/data/mechator/";
    std::string names[] = {"confirm","cancel","select","deny","enemy_shoot","enemy_shoot_big","explosion_small","explosion_big","get_dollar","get_item","grenade","laser","laser_charge", "missile", "playergun", "playergun_2x", "playerhit", "playerhit2"};
    path += names[int(sfx)];
    path += ".raw";
    static int channel = 0;
    channel = (channel + 1) % 5;
    switch(channel) {
        case 0:
            playOnChannel<1>(path.c_str());
            break;
        case 1:
            playOnChannel<2>(path.c_str());
            break;
        case 2:
            playOnChannel<3>(path.c_str());
            break;
        case 3:
            playOnChannel<4>(path.c_str());
            break;
        case 4:
            playOnChannel<5>(path.c_str());
            break;
        default:
            break;
    }
}

void AudioSystem::playSong(Song song) {
    switch(song) {
    case musNone:
        Audio::stop<0>();
        break;
    case musCape:
        Audio::play<0>("data/mechator/storm.raw");
        break;
    case musOverworld:
        Audio::play<0>("data/mechator/overworld.raw");
        break;
    case musOverworldMain:
        Audio::play<0>("data/mechator/overworld_main.raw");
        break;
    case musBoss:
        Audio::play<0>("data/mechator/boss.raw");
        break;
//    case musUpcycle:
//        Audio::play<0>("data/buttoncity/bcupcycle.raw");
//        break;
//    case musGame:
//        Audio::play<0>("data/buttoncity/prismabops.raw");
//        break;
    }
}

#endif
