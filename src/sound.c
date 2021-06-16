#include "gba.h"

#define INVALID_CHANNEL 255
#define NUM_CHANNELS 8

static SoundData sound_datas[128]; // Max 128 sounds
static bool channel_paused[NUM_CHANNELS];

static u8 findAvailableChannel() {
    for (int i = 0; i < NUM_CHANNELS; i++) {
        if (!AAS_SFX_IsActive(i) && !channel_paused[i]) {
            return i;
        }
    }
    return INVALID_CHANNEL;
}


// ---------------------------------------------------------------------------
//                       Sound Library Functions
// ---------------------------------------------------------------------------

Sound createSound(char *sound_name) {
    static int sound_num = 0;
    Sound sound = getSound(sound_name);
    sound_datas[sound_num].channel = INVALID_CHANNEL;
    sound_datas[sound_num].volume = 64;
    sound.sound_data = &sound_datas[sound_num];
    sound_num++;
    return sound;
}

void playSound(Sound sound, bool loop) {
    u8 channel = findAvailableChannel();
    if (channel == INVALID_CHANNEL) {
        //TODO: complain angrily
        return;
    }
    sound.sound_data->channel = channel;
    AAS_SFX_Play( channel, sound.sound_data->volume, 8000, sound.mem_start, sound.mem_end, loop ? sound.mem_start : AAS_NULL );
}

void stopSound(Sound sound) {
    u8 channel = sound.sound_data->channel;
    if (channel == INVALID_CHANNEL) {
        return;
    }
    if (!AAS_SFX_IsActive(channel)) {
        return;
    }
    AAS_SFX_Stop(channel);
}

void pauseSound(Sound sound) {
    u8 channel = sound.sound_data->channel;
    if (channel == INVALID_CHANNEL) {
        return;
    }
    if (!AAS_SFX_IsActive(channel)) {
        return;
    }
    AAS_SFX_Stop(channel);
    channel_paused[channel] = true;
}

void resumeSound(Sound sound) {
    u8 channel = sound.sound_data->channel;
    if (channel == INVALID_CHANNEL) {
        return;
    }
    if (!channel_paused[channel]) {
        return;
    }
    AAS_SFX_Resume(channel);
    channel_paused[channel] = false;
}

void setVolume(Sound sound, int volume) {
    // convert range [0-100] -> [0-64]
    int adjusted_volume = (int) (64 * (volume / 100.0));
    sound.sound_data->volume = adjusted_volume;
    u8 channel = sound.sound_data->channel;
    if (channel == INVALID_CHANNEL) {
        return;
    }
    if (!AAS_SFX_IsActive(channel)) {
        return;
    }
    AAS_SFX_SetVolume(channel, adjusted_volume);
}

int getVolume(Sound sound) {
    return sound.sound_data->volume;
}