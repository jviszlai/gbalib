#include "gba.h"

volatile unsigned short *videoBuffer = (volatile unsigned short *) 0x6000000;

// Sprite definitions
charblock *charbase = (charblock*)0x6000000;
extern const unsigned short sprite_data_palette[];
extern const unsigned short sprite_data[];
extern ObjAttr sprite_list[128];
extern GifInfo gif_list[64];
extern int num_gifs;
static u32 prev_buttons = 0;

void waitForVBlank(void) {
    // Want to be drawing to screen during VBlank period to avoid tearing artifcats.
    while (*SCANLINECOUNTER >= 160);
    while (*SCANLINECOUNTER < 160);
}

static int __qran_seed= 42;
static int qran(void) {
    __qran_seed= 1664525*__qran_seed+1013904223;
    return (__qran_seed>>16) & 0x7FFF;
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

    // TODO: update gif based on Timer interrupt
    if (intr_bits & IRQ_TIMER2) {
    
        // Update gifs
        for (int i = 0; i < num_gifs; i++) {
            GifInfo *g_info = &gif_list[i];
            ObjAttrImageInfo *info;
            if (g_info->curr_frame == g_info->num_frames - 1) {
                g_info->curr_frame = 0;
            } else {
                g_info->curr_frame++;
            }
            info = g_info->frame0_info + g_info->curr_frame;
            SpriteInfo s_info = getSpriteInfo();
            g_info->gif_obj->attr0 = (g_info->gif_obj->attr0 & 0x03FF) | info->shape | s_info.type;
            g_info->gif_obj->attr1 = (g_info->gif_obj->attr1 & 0x01FF) | info->size;
            g_info->gif_obj->attr2 = info->palette_id | info->id;
        }
    }

	// Clear the interrupt flags
	REG_IF |= REG_IF;
}

static void gbaSetup() {
    populateSpriteImages();
    populateBackgrounds();
    populateSounds();
    populateGifs();

    AAS_SetConfig( AAS_CONFIG_MIX_24KHZ, AAS_CONFIG_CHANS_8, AAS_CONFIG_SPATIAL_MONO, AAS_CONFIG_DYNAMIC_OFF );

    SpriteInfo s_info = getSpriteInfo();
    REG_DISPCNT = MODE3 | BG2_ENABLE | OBJ_ENABLE | s_info.dimension_type;

    // Timer setup (for gif sprites)
    REG_TIMER[2].data = 0xF300;
    REG_TIMER[2].cnt = TM_IRQ | TM_ENABLE | TM_FREQ_1024;

    REG_IE |= IRQ_TIMER2;

    // Sprite setup
    AAS_DoDMA3(sprite_data_palette, SPRITEPAL, s_info.palette_size | DMA_ON);
    AAS_DoDMA3(sprite_data, &charbase[5], s_info.data_size | DMA_ON);
    for(int i = 0; i < 128; i++) {
        sprite_list[i].attr0 = ATTR0_HIDE;
    }

}

void AgbMain() {
    volatile u8 buff[100]; // Honestly have no clue but AAS seems to corrupt x3007eb0 (literally only that address) during interrupt handling. User stack starts at x3007f00, so buffer to start stack past x3007eb0...TODO: debug root cause when I have time
    gbaSetup();
    main();
    while (1) {
        updateScreen();
    }
}

void updateScreen() {
    // TODO: potentially add internal buffer and flush drawing here to make it harder to tear
    prev_buttons = BUTTONS;
    waitForVBlank();

    // Update sprites
    AAS_DoDMA3(&sprite_list[0], SPRITEMEM, 128 * 4 | DMA_ON);

}

int randomInteger(int min, int max) {
    return (qran()*(max-min)>>15)+min;
}

void wait(float seconds) {
    int vblanks = (int) (70.0 * seconds);
    for (int i = 0; i < vblanks; i++) {
        updateScreen();
    }
}

bool waitAndCheckButton(float seconds, Button button) {
    int vblanks = (int) (70.0 * seconds);
    bool button_pressed = false;
    for (int i = 0; i < vblanks; i++) {
        updateScreen();
        if (isButtonDown(button)) {
            button_pressed = true;
        }
    }
    return button_pressed;
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
    return (Color) {COLOR(red >> 3, green >> 3, blue >> 3)};
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

bool checkCollisionPositionGif(Gif gif, Position pos) {
    Position gif_pos = getGifPosition(gif);
    return gif_pos.x <= pos.x && gif_pos.x + gif.size.width > pos.x && gif_pos.y <= pos.y && gif_pos.y + gif.size.height > pos.y;
}


bool checkCollisionColorGif(Gif gif, Color color) {
    Position gif_pos = getGifPosition(gif);
    for (int i = 0; i < gif.size.width; i++) {
        for (int j = 0; j < gif.size.height; j++) {
            if (videoBuffer[OFFSET(gif_pos.y + j, gif_pos.x + i, WIDTH)] == color.value) {
                return true;
            }
        }
    }
    return false;
}

bool checkCollisionRectangleGif(Gif gif, Position rectPos, Size rectSize) {
    Position pos1 = getGifPosition(gif);
    return pos1.x < rectPos.x + rectSize.width && pos1.x + gif.size.width > rectPos.x && pos1.y < rectPos.y + rectSize.height && pos1.y + gif.size.height > rectPos.y;
}

bool checkCollisionRectangle(Sprite sprite, Position rectPos, Size rectSize) {
    Position pos1 = getPosition(sprite);
    return pos1.x < rectPos.x + rectSize.width && pos1.x + sprite.size.width > rectPos.x && pos1.y < rectPos.y + rectSize.height && pos1.y + sprite.size.height > rectPos.y;
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

bool checkCollisionColor(Sprite sprite, Color color) {
    Position sprite_pos = getPosition(sprite);
    for (int i = 0; i < sprite.size.width; i++) {
        for (int j = 0; j < sprite.size.height; j++) {
            if (videoBuffer[OFFSET(sprite_pos.y + j, sprite_pos.x + i, WIDTH)] == color.value) {
                return true;
            }
        }
    }
    return false;
}