#ifndef GBALIB_H
#define GBALIB_H

#include "types.h"

typedef struct {
    char *image_name;
    ObjAttr *sprite_obj;
} Sprite;

typedef struct {
    Size size;
    char *image_name;
    const u16 *background_data;
} Background;

typedef struct {
    const char *mem_start;
    const char *mem_end;
    char *sound_name;
} Sound;

typedef struct {
    u16 value;
} Color;

typedef enum {
    A,
    B,
    SELECT,
    START,
    RIGHT,
    LEFT,
    UP, 
    DOWN,
    R,
    L
} Button;

void updateScreen();
bool isButtonDown(Button button);
bool wasButtonPressed(Button button);
bool wasButtonReleased(Button button);
Sprite createSprite(char *image_name);
void hideSprite(Sprite sprite);
void showSprite(Sprite sprite);
bool isSpriteHidden(Sprite sprite);
Position getPosition(Sprite sprite);
void updatePosition(Sprite sprite, Position pos);
void updateImage(Sprite sprite, char *image_name);
void drawBackground(Background background, Position pos);
Background createBackground(char *image_name);
Color createColor(u16 red, u16 blue, u16 green);
void drawText(char *text, Position pos, Color color);
Sound createSound(char *sound_name);
void playSound(Sound sound);

#endif