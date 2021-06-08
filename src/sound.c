#include "gba.h"


// ---------------------------------------------------------------------------
//                       Sound Library Functions
// ---------------------------------------------------------------------------

Sound createSound(char *sound_name) {
    return getSound(sound_name);
}

void playSound(Sound sound) {
    AAS_SFX_Play( 0, 64, 8000, sound.mem_start, sound.mem_end, AAS_NULL );
}