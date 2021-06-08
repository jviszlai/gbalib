#ifndef SOUND_H
#define SOUND_H
#include "types.h"
#include "gbalib.h"

void populateSounds();
Sound getSound(char *sound_name);
#endif