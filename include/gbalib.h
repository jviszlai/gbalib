#ifndef GBALIB_H
#define GBALIB_H

#include "types.h"

typedef struct {
    Size size;
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
    SoundData *sound_data;
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
void wait(float seconds);
bool waitAndCheckButton(float seconds, Button button);
bool isButtonDown(Button button);
bool wasButtonPressed(Button button);
bool wasButtonReleased(Button button);
bool checkCollisionPosition(Sprite sprite, Position pos);
bool checkCollisionSprite(Sprite sprite1, Sprite sprite2);
Sprite createSprite(char *image_name, Size size);
void hideSprite(Sprite sprite);
void showSprite(Sprite sprite);
bool isSpriteHidden(Sprite sprite);
Position getPosition(Sprite sprite);
void updatePosition(Sprite sprite, Position pos);
void updateImage(Sprite sprite, char *image_name);
void drawBackground(Background background, Position pos);
Background createBackground(char *image_name);
Color createColor(u16 red, u16 green, u16 blue);
void drawFilledRectangle(Color color, Position pos, Size size);
void drawHollowRectangle(Color color, Position pos, Size size);
void drawText(char *text, Color color, Position pos);
void drawBlockText(char *text, Color color, Position pos, Size size);
void animateTextFast(char *text, Color color, Position pos, Size size);
void animateTextSlow(char *text, Color color, Position pos, Size size);
void animateTextSound(char *text, Color color, Sound sound, Position pos, Size size);
Sound createSound(char *sound_name);
void playSound(Sound sound, bool loop);
void stopSound(Sound sound);
void pauseSound(Sound sound);
void resumeSound(Sound sound);
void setVolume(Sound sound, int volume);
int getVolume(Sound sound);

#endif