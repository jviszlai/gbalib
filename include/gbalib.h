#ifndef GBALIB_H
#define GBALIB_H

/**
 * @file gbalib.h
 */

#include "types.h"

/**
 * A position on the GBA screen in the unit of pixels. Note: the top left corner is the origin (0,0) and the bottom right corner is (239, 159). 
 * So the GBA screen is 240 pixels wide and 160 pixels tall.
 */
typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    u32 width;
    u32 height;
} Size;

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

/**
 * Updates the GBA screen and checks for button changes. Typically call this once each iteration / frame.
 */
void updateScreen();

/**
 * Waits for the specificed number of seconds (approximately). The screen is not updated and buttons aren't 
 * checked for updates during this time.
 * 
 * @param seconds The number of seconds to wait
 */
void wait(float seconds);

/**
 * Same behavior as wait() but also checks if a button was pressed while waiting. 
 * 
 * @param seconds The number of seconds to wait
 * @param button The button to check if pressed while waiting
 * 
 * @return A boolean value. True means \p button was pressed while waiting, False otherwise.
 */
bool waitAndCheckButton(float seconds, Button button);

/**
 * Checks if \p button is currently pressed down.
 * 
 * @param button The button to check
 * 
 * @return A boolean value for if \p button is pressed down.
 */
bool isButtonDown(Button button);

/**
 * Checks if \p button was pressed in the last time since updateScreen() was called (typically if \p button was pressed this frame).
 * 
 * @param button The button to check
 * 
 * @return A boolean value for if \p button was just pressed.
 */
bool wasButtonPressed(Button button);

/**
 * Checks if \p button was released in the last time since updateScreen() was called. Effectively the inverse of wasButtonPressed().
 * 
 * @param button The button to check
 * 
 * @return A boolean value for if \p button was just released.
 */
bool wasButtonReleased(Button button);

/**
 * Checks if \p sprite is overlapping at a specified Position.
 * 
 * @param sprite The sprite to check
 * @param pos The Position to check if sprite is overlapping at
 * 
 * @return A boolean value for if \p sprite is overlapping at \p pos
 */
bool checkCollisionPosition(Sprite sprite, Position pos);

/**
 * Checks if \p sprite1 is overlapping with \p sprite2.
 * 
 * @param sprite1 The first sprite
 * @param sprite2 The second sprite
 * 
 * @return A boolean value for if \p sprite1 and \p sprite2 are overlapping.
 */
bool checkCollisionSprite(Sprite sprite1, Sprite sprite2);

/**
 * Creates a sprite of a given image and size. By default the sprite is hidden. Use showSprite() to show it.
 * @warning Currently only .png files are supported for sprite images.
 * @warning Only the following image sizes are permitted: 4x4, 8x8, 16x16, 32x32
 * 
 * @param image_name The name of the image used for the sprite, located in the Sprites/ folder. This should not include the file extension. So to use Mario.png, pass in "Mario", for example.
 * @param size The "logical" size of the sprite. For example, if your image is 16x16 but the actual sprite is only 10x10, size would be width=10, height=10. Make sure the sprite is aligned to the top left of the image.
 * 
 * @returns Your shiny new sprite in a cool Sprite struct.
 */
Sprite createSprite(char *image_name, Size size);

/**
 * Hides the sprite so it is not visible.
 * 
 * @param sprite The Sprite to hide.
 */
void hideSprite(Sprite sprite);

/**
 * Shows the sprite so it is visible.
 * 
 * @param sprite The Sprite to show.
 */
void showSprite(Sprite sprite);

/**
 * Checks if the sprite is currently hidden.
 * 
 * @param sprite The Sprite to check.
 * 
 * @returns A boolean value for if \p sprite is hidden.
 */
bool isSpriteHidden(Sprite sprite);

/**
 * Gets the current Position of the sprite.
 * 
 * @param sprite The Sprite to get the Position of.
 * 
 * @returns The current Position of \p sprite.
 */
Position getPosition(Sprite sprite);

/**
 * Updates the Position of the sprite.
 * 
 * @param sprite The Sprite to update the Position of.
 * @param pos The Position to move \p sprite to.
 */
void updatePosition(Sprite sprite, Position pos);

/**
 * Updates the image of the sprite.
 * 
 * @param sprite The Sprite to update the image of.
 * @param image_name The new image for \p sprite. See createSprite() for limitations on images.
 */
void updateImage(Sprite sprite, char *image_name);

/**
 * Draws a Background at a specified Position.
 * 
 * @param background The Background to draw.
 * @param pos The Position to draw at. This should be the desired Position of the top left corner of \p background.
 */
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