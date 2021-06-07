#include "gba.h"

// Private helper functions




// ---------------------------------------------------------------------------
//                       Background Library Functions
// ---------------------------------------------------------------------------

Background createBackground(char *image_name) {
    return getBackground(image_name);
}

void drawBackground(Background background, Position pos) {
    //Todo: error bound checking
    if (background.size.width == WIDTH) {
        DMA[3].src = background.background_data;
        DMA[3].dst = &videoBuffer[OFFSET(pos.y, pos.x, WIDTH)];
        DMA[3].cnt = background.size.width * background.size.height | DMA_ON;
    } else {
        for (int r = 0; r < background.size.height; r++) {
            DMA[3].src = &background.background_data[OFFSET(r, 0, background.size.width)];
            DMA[3].dst = &videoBuffer[OFFSET(pos.y + r, pos.x, WIDTH)];
            DMA[3].cnt = background.size.width | DMA_ON;
        }
    }
}