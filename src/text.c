#include "gba.h"

// Private helper functions

#define SPACE 32

static void setPixel(int x, int y, u16 color) {
    videoBuffer[OFFSET(y, x, WIDTH)] = color;
}

static void drawChar(int col, int row, char ch, u16 color) {
    for(int r = 0; r<8; r++) {
        for(int c=0; c<6; c++) {
            if(fontdata_6x8[OFFSET(r, c, 6) + ch*48]) {
                setPixel(col+c, row+r, color);
            }
        }
    }
}

static void drawString(int col, int row, char *str, u16 color) {
    while(*str) {
        drawChar(col, row, *str++, color);
        col += 6;
    }
}

static void drawCenteredString(int col, int row, int width, int height, char *str, u16 color) {
    u32 len = 0;
    char *strCpy = str;
    while (*strCpy) {
        len++;
        strCpy++;
    }

    u32 strWidth = 6 * len;
    u32 strHeight = 8;

    int x = col + ((width - strWidth) >> 1);
    int y = row + ((height - strHeight) >> 1);
    drawString(x, y, str, color);
}

static bool animateWord(char *word, Color color, Sound sound, int r, int c, int vblankcount, Button button, bool checkButton) {
    char *curr = word;
    bool buttonPressed = false;
    while (*curr && *curr != SPACE) {
        for (int i = 0; i < vblankcount; i++) {
            if (checkButton && isButtonDown(button)) {
                vblankcount = 0;
                buttonPressed = true;
            } else {
                waitForVBlank();
            }
        }
        drawChar(c, r, *curr, color.value);
        if (sound.sound_data) {
            playSound(sound, false);
        }
        c += 6;
        curr++;
    }
    return buttonPressed;
}

static bool animateText(char *text, Color color, Sound sound, Position pos, Size size, int vblankcount, Button button, bool checkButton) {
    char *curr = text;
    int curr_c = pos.x;
    int curr_r = pos.y;
    bool button_pressed = false;
    while (*curr) {
        int word_len = 0;
        char *word_end = curr;
        while (*word_end && *word_end != SPACE) {
            word_len++;
            word_end++;
        }
        if (*word_end) {
            word_len++;
        }
        if (6 * word_len > size.width) {
            return button_pressed;
        }
        if (curr_c + 6 * word_len > pos.x + size.width || curr_c + 6 * word_len > WIDTH) {
            if (curr_r + 16 > pos.y + size.height || curr_r + 16 > HEIGHT) {
                return button_pressed;
            } else {
                curr_c = pos.x;
                curr_r += 8;
            }
        } 
        if (animateWord(curr, color, sound, curr_r, curr_c, vblankcount, button, checkButton)) {
            vblankcount = 0;
            button_pressed = true;
        }
        if (*word_end) {
            drawChar(curr_c + 6 * (word_len - 1), curr_r, SPACE, color.value);
        } else {
            return button_pressed;
        }
        curr_c += 6 * word_len;
        curr = word_end + 1;
    }
    return button_pressed;
}

// ---------------------------------------------------------------------------
//                       Text Library Functions
// ---------------------------------------------------------------------------

void drawText(char *text, Color color, Position pos) {
    drawString(pos.x, pos.y, text, color.value);
}

void drawBlockText(char *text, Color color, Position pos, Size size) {
    Sound dummySound;
    dummySound.sound_data = NULL;
    animateText(text, color, dummySound, pos, size, 0, A, false);
}

void animateTextFast(char *text, Color color, Position pos, Size size) {
    Sound dummySound;
    dummySound.sound_data = NULL;
    animateText(text, color, dummySound, pos, size, 1, A, false);
}

bool animateTextFastButton(char *text, Color color, Position pos, Size size, Button button) {
    Sound dummySound;
    dummySound.sound_data = NULL;
    return animateText(text, color, dummySound, pos, size, 1, button, true);
}

void animateTextSlow(char *text, Color color, Position pos, Size size) {
    Sound dummySound;
    dummySound.sound_data = NULL;
    animateText(text, color, dummySound, pos, size, 4, A, false);
}

void animateTextSound(char *text, Color color, Sound sound, Position pos, Size size) {
    animateText(text, color, sound, pos, size, 4, A, false);
}
