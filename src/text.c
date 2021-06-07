#include "gba.h"

// Private helper functions

void setPixel(int x, int y, u16 color) {
    videoBuffer[OFFSET(y, x, WIDTH)] = color;
}

void drawChar(int col, int row, char ch, u16 color) {
    for(int r = 0; r<8; r++) {
        for(int c=0; c<6; c++) {
            if(fontdata_6x8[OFFSET(r, c, 6) + ch*48]) {
                setPixel(col+c, row+r, color);
            }
        }
    }
}

void drawString(int col, int row, char *str, u16 color) {
    while(*str) {
        drawChar(col, row, *str++, color);
        col += 6;
    }
}


// ---------------------------------------------------------------------------
//                       Text Library Functions
// ---------------------------------------------------------------------------

void drawText(char *text, Position pos, Color color) {
    drawString(pos.x, pos.y, text, color.value);
}