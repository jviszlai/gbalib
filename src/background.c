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
        AAS_DoDMA3(background.background_data, &videoBuffer[OFFSET(pos.y, pos.x, WIDTH)], background.size.width * background.size.height | DMA_ON);
    } else {
        for (int r = 0; r < background.size.height; r++) {
            AAS_DoDMA3(&background.background_data[OFFSET(r, 0, background.size.width)], &videoBuffer[OFFSET(pos.y + r, pos.x, WIDTH)], background.size.width | DMA_ON);
        }
    }
}