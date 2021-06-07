#include "gba.h"

volatile unsigned short *videoBuffer = (volatile unsigned short *) 0x6000000;
u32 vBlankCounter = 0;

// Sprite definitions
charblock *charbase = (charblock*)0x6000000;
extern const unsigned short sprite_data_palette[];
extern const unsigned short sprite_data[];
extern ObjAttr sprite_list[32];
static u32 prev_buttons = 0;

void waitForVBlank(void) {
    // Want to be drawing to screen during VBlank period to avoid tearing artifcats.
    while (*SCANLINECOUNTER > 160);
    while (*SCANLINECOUNTER < 160);
    vBlankCounter++;
}

static int __qran_seed= 42;
static int qran(void) {
    __qran_seed= 1664525*__qran_seed+1013904223;
    return (__qran_seed>>16) & 0x7FFF;
}

int randint(int min, int max) {
    return (qran()*(max-min)>>15)+min;
}

void gbaSetup() {
    populateSpriteImages();
    populateBackgrounds();
    PaletteInfo p_info = getPaletteInfo();
    REG_DISPCNT = MODE3 | BG2_ENABLE | OBJ_ENABLE | p_info.dimension_type;

    // Sprite setup
    DMA[3].src = sprite_data_palette;
    DMA[3].dst = SPRITEPAL;
    DMA[3].cnt = p_info.length | DMA_ON;
    DMA[3].src = sprite_data;
    DMA[3].dst = &charbase[5];
    DMA[3].cnt = p_info.size | DMA_ON;
    for(int i = 0; i < 32; i++) {
        sprite_list[i].attr0 = ATTR0_HIDE;
    }
}

void updateScreen() {
    // TODO: potentially add internal buffer and flush drawing here to make it harder to tear
    waitForVBlank();

    u16 test_size = (1 << 14);
    u16 test_id = 512;

    // // Update sprites
    DMA[3].src = &sprite_list[0];
    DMA[3].dst = SPRITEMEM;
    DMA[3].cnt = 32 * 4 | DMA_ON;

    prev_buttons = BUTTONS;
}

bool isButtonDown(Button button) {
    return KEY_DOWN(1 << button, BUTTONS);
}

bool wasButtonPressed(Button button) {
    return KEY_JUST_PRESSED(1 << button, BUTTONS, prev_buttons);
}

bool wasButtonReleased(Button button) {
    return KEY_JUST_PRESSED(1 << button, prev_buttons, BUTTONS);
}

Color createColor(u16 red, u16 blue, u16 green) {
    // Need to convert [0-255) -> [0-32). So divide by 8
    return (Color) {COLOR(red >> 3, blue >> 3, green >> 3)};
}