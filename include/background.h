#ifndef BACKGROUND_H
#define BACKGROUND_H
#include "types.h"

typedef struct {
    Size size;
    char *image_name;
    const u16 *background_data;
} Background;

void populateBackgrounds(); 
Background getBackground(char *image_name);
#endif