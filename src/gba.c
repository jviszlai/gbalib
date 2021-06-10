#include "gba.h"

volatile unsigned short *videoBuffer = (volatile unsigned short *) 0x6000000;
u32 vBlankCounter = 0;

// Sprite definitions
charblock *charbase = (charblock*)0x6000000;
extern const unsigned short sprite_data_palette[];
extern const unsigned short sprite_data[];
extern ObjAttr sprite_list[128];
static u32 prev_buttons = 0;

void waitForVBlank(void) {
    // Want to be drawing to screen during VBlank period to avoid tearing artifcats.
    while (*SCANLINECOUNTER >= 160);
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

// Registers for interrupt handler
#define REG_IE (*(volatile u16 *)0x4000200)
#define REG_IF (*(volatile u16 *)0x4000202) 

void InterruptProcess()
{
	u16 intr_bits = REG_IE & REG_IF;
	
	// It's best to test for AAS's Timer 1 interrupt first
	if ( intr_bits & 0x10 ) // Timer 1
		AAS_Timer1InterruptHandler();
	
	// Process other interrupts here by testing appropriate bits of "intr_bits"

	// Clear the interrupt flags
	REG_IF |= REG_IF;
}

static void gbaSetup() {
    populateSpriteImages();
    populateBackgrounds();
    populateSounds();

    AAS_SetConfig( AAS_CONFIG_MIX_24KHZ, AAS_CONFIG_CHANS_8, AAS_CONFIG_SPATIAL_MONO, AAS_CONFIG_DYNAMIC_OFF );

    PaletteInfo p_info = getPaletteInfo();
    REG_DISPCNT = MODE3 | BG2_ENABLE | OBJ_ENABLE | p_info.dimension_type;

    // Sprite setup
    AAS_DoDMA3(sprite_data_palette, SPRITEPAL, p_info.length | DMA_ON);
    AAS_DoDMA3(sprite_data, &charbase[5], p_info.size | DMA_ON);
    for(int i = 0; i < 32; i++) {
        sprite_list[i].attr0 = ATTR0_HIDE;
    }

}

void AgbMain() {
    gbaSetup();
    main();
    while (1);
}

void updateScreen() {
    // TODO: potentially add internal buffer and flush drawing here to make it harder to tear
    prev_buttons = BUTTONS;
    waitForVBlank();

    u16 test_size = (1 << 14);
    u16 test_id = 512;

    // // Update sprites
    AAS_DoDMA3(&sprite_list[0], SPRITEMEM, 128 * 4 | DMA_ON);

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

Color createColor(u16 red, u16 green, u16 blue) {
    // Need to convert [0-255) -> [0-32). So divide by 8
    return (Color) {COLOR(red >> 3, blue >> 3, green >> 3)};
}

void drawFilledRectangle(Color color, Position pos, Size size) {
    volatile u16 color_val = color.value;
    if (size.width == WIDTH) {
        AAS_DoDMA3(&color_val, &videoBuffer[OFFSET(pos.y, pos.x, WIDTH)], size.width * size.height | DMA_SOURCE_FIXED | DMA_ON );
    } else {
        for (int r = 0; r < size.height; r++) {
            AAS_DoDMA3(&color_val, &videoBuffer[OFFSET(pos.y + r, pos.x, WIDTH)], size.width | DMA_SOURCE_FIXED | DMA_ON);
        }
    }
}

void drawHollowRectangle(Color color, Position pos, Size size) {
    for (int i = 0; i < size.width; i++) {
        videoBuffer[OFFSET(pos.y, pos.x + i, WIDTH)] = color.value;
        videoBuffer[OFFSET(pos.y + size.height - 1, pos.x + i, WIDTH)] = color.value;
    }
    for (int i = 1; i < size.height - 1; i++) {
        videoBuffer[OFFSET(pos.y + i, pos.x, WIDTH)] = color.value;
        videoBuffer[OFFSET(pos.y + i, pos.x + size.width - 1, WIDTH)] = color.value;
    }
}

bool checkCollisionPosition(Sprite sprite, Position pos) {
    Position sprite_pos = getPosition(sprite);
    return sprite_pos.x <= pos.x && sprite_pos.x + sprite.size.width > pos.x && sprite_pos.y <= pos.y && sprite_pos.y + sprite.size.height > pos.y;
}

bool checkCollisionSprite(Sprite sprite1, Sprite sprite2) {
    Position pos1 = getPosition(sprite1);
    Position pos2 = getPosition(sprite2);
    return pos1.x < pos2.x + sprite2.size.width && pos1.x + sprite1.size.width > pos2.x && pos1.y < pos2.y + sprite2.size.height && pos1.y + sprite1.size.height > pos2.y;
}
