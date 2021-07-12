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

/**
 * A size representing a width and height in pixels. Note the GBA screen is 240 pixels wide by 160 pixels tall, so typically the largest size you could have would be width=240, height=160.
 */
typedef struct {
    u32 width;
    u32 height;
} Size;

/**
 * A sprite which is the primary type to represent a constantly moving image. These are handled differently than backgrounds by the GBA hardware which is why we can efficiently animate them.
 * 
 * Use createSprite() to make one.
 */
typedef struct {
    Size size;
    char *image_name;
    ObjAttr *sprite_obj;
} Sprite;

/**
 * A sprite but the image is a GIF
 * 
 * Use createGif() to make one.
 */
typedef struct {
    Size size;
    char *gif_name;
    GifInfo *gif_info;
} Gif;

/**
 * A background can be thought of as a stamp on the GBA screen. Contains an image of any size which can be drawn on the screen using functions like drawBackground(). 
 * 
 * Use createBackground() to make one.
 */
typedef struct {
    Size size;
    char *image_name;
    const u16 *background_data;
} Background;

/**
 * A sound which represents sounds playing through the GBA speakers. Can be manipulated by using functions like playSound() and stopSound().
 * 
 * Use createSound() to make one.
 */
typedef struct {
    const char *mem_start;
    const char *mem_end;
    char *sound_name;
    SoundData *sound_data;
} Sound;

/**
 * A color that can be assigned to a pixel on the GBA screen. The GBA needs this packed into a single value, so use createColor() to more easily create your own color.
 */
typedef struct {
    u16 value;
} Color;

/**
 * All of the buttons on the GBA that we can read the state of. Used as arguments for functions like isButtonDown().
 */
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
 * Generate a random integer in the range [min, max]
 * 
 * @param min The minimum integer in the range
 * @param max The maximum integer in the range
 * 
 * @returns The randomly selected integer in the range
 */
int randomInteger(int min, int max);

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
 * Checks if \p gif is overlapping at a specified Position.
 * 
 * @param gif The Gif sprite to check
 * @param pos The Position to check if gif is overlapping at
 * 
 * @return A boolean value for if \p gif is overlapping at \p pos
 */
bool checkCollisionPositionGif(Gif gif, Position pos);

/**
 * Checks if \p gif is overlapping with a pixel of color \p color;
 * 
 * @param gif The Gif sprite to check collision of
 * @param color The color to check collision of
 * 
 * @return A boolean value for if \p gif is overlapping a pixel of color \p color
 */
bool checkCollisionColorGif(Gif gif, Color color);

/**
 * Checks if \p gif1 is colliding with \p gif2.
 * 
 * @param gif1 The first Gif
 * @param gif2 The second Gif
 * 
 * @return A boolean for if \p gif1 is colliding with \p gif2
 */
bool checkCollisionGif(Gif gif1, Gif gif2);
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
 * Checks if \p sprite is colliding with a rectangel defined by \p rectPos and \p rectSize.
 * 
 * @param sprite The Sprite to check
 * @param rectPos The rectangle Position
 * @param rectSize The rectangle Size
 * 
 * @return A boolean value for if \p sprite is overlapping with the rectangle
 */
bool checkCollisionRectangle(Sprite sprite, Position rectPos, Size rectSize);

/**
 * Checks if \p gif is colliding with a rectangel defined by \p rectPos and \p rectSize.
 * 
 * @param gif The Gif sprite to check
 * @param rectPos The rectangle Position
 * @param rectSize The rectangle Size
 * 
 * @return A boolean value for if \p gif is overlapping with the rectangle
 */
bool checkCollisionRectangleGif(Gif gif, Position rectPos, Size rectSize);

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
 * Checks if \p sprite is overlapping with a pixel of color \p color;
 * 
 * @param sprite The sprite to check collision of
 * @param color The color to check collision of
 * 
 * @return A boolean value for if \p sprite is overlapping a pixel of color \p color
 */
bool checkCollisionColor(Sprite sprite, Color color);

/**
 * Just like createSprite except the sprite image is a gif.
 * 
 * @param gif_name The name of the gif, without the .gif extension. Should be in the sprites/ folder
 * @param size The logical size of the sprites as described in createSprite()
 * 
 * @return Your shiny new gif sprite in a Gif struct
 */
Gif createGif(char *gif_name, Size size);

/**
 * Just like showSprite() but for a Gif-backed sprite
 * 
 * @param gif The gif sprite to show
 */
void showGif(Gif gif);

/**
 * Just like hideSprite() but for a Gif-backed sprite
 * 
 * @param gif The gif sprite to show
 */
void hideGif(Gif gif);

/**
 * Just like updatePosition() but for a Gif-backed sprite
 * 
 * @param gif The gif to update the position of
 * @param pos The position to update to
 */
void updateGifPosition(Gif gif, Position pos);

/**
 * Just like getPosition() but for a Gif-backed sprite 
 * 
 * @param gif The gif sprite to get the position of
 * @return The position /p gif is at
 */
Position getGifPosition(Gif gif);

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

/**
 * Creates a Background from the given image.
 * @warning Currently only .png files are supported for backgrounds
 * 
 * @param image_name The name of the image used for the Background, located in the Backgrounds/ folder. This should not include the file extension. So to use Forest.png, pass in "Forest", for example.
 * 
 * @returns Your shiny new Background in a cool Background struct.
 */
Background createBackground(char *image_name);

/**
 * Creates a Color for you (this is mostly necessary since the color format the GBA uses is slightly unintuitive).
 * 
 * @param red The amount of red in the Color. Range is from [0-255] (inclusive)
 * @param green The amount of green in the Color. Range is from [0-255] (inclusive)
 * @param blue The amount of blue in the Color. Range is from [0-255] (inclusive)
 * 
 * @returns Your cool mixed Color in a Color struct.
 */
Color createColor(u16 red, u16 green, u16 blue);

/**
 * Draws a filled in rectangle on the GBA screen with the specified Color, Position, and Size.
 * 
 * @param color The Color of the rectangle
 * @param pos The Position of the rectangle. This is where the top left of the rectangle will be
 * @param size The Size of the rectangle
 */
void drawFilledRectangle(Color color, Position pos, Size size);

/**
 * Draws a hollow rectangle with 1 pixel wide lines on the GBA screen with the specified Color, Position, and Size.
 * 
 * @param color The Color of the rectangle.
 * @param pos The Position of the rectangle. This is where the top left of the rectangle will be
 * @param size The Size of the rectangle
 */
void drawHollowRectangle(Color color, Position pos, Size size);

/**
 * Draws text on the screen starting at the specified position in the specified color.
 * 
 * @warning Text drawn with this function will not wrap onto multiple lines. Use drawBlockText() for that.
 * 
 * @param text The text to be drawn on the GBA screen
 * @param color The color of the text
 * @param pos The starting position of the text
 */
void drawText(char *text, Color color, Position pos);

/**
 * Draws text on the screen starting at the specified position in the specified color. This text is bounded by a rectangle and can wrap onto multiple lines.
 * 
 * @param text The text to be drawn on the GBA screen
 * @param color The color of the text
 * @param pos The starting position of the text and the top left corner of the bounding rectangle
 * @param size The size of the bounding rectangle
 */
void drawBlockText(char *text, Color color, Position pos, Size size);

/**
 * Draws text similar to drawBlockText(), however, animates it so the characters appear sequentially. 
 * 
 * @warning During the execution of this function, user input is not checked.
 * 
 * @param text The text to be drawn on the GBA screen
 * @param color The color of the text
 * @param pos The starting position of the text and the top left corner of the bounding rectangle
 * @param size The size of the bounding rectangle
 */
void animateTextFast(char *text, Color color, Position pos, Size size);

/**
 * Draws text similar to animateTextFast(), however, checks if \p button is pressed during animation, and fast-forwards if so. 
 * 
 * @warning During the execution of this function, only \b button is checked
 * 
 * @param text The text to be drawn on the GBA screen
 * @param color The color of the text
 * @param pos The starting position of the text and the top left corner of the bounding rectangle
 * @param size The size of the bounding rectangle
 * @param button The button to check
 * 
 * @returns A boolean value for if \p button was pressed during the animation
 */
bool animateTextFastButton(char *text, Color color, Position pos, Size size, Button button);

/**
 * Same behavior as animateTextFast() but with a longer animation, meaning more time between characters.
 * 
 * @param text The text to be drawn on the GBA screen
 * @param color The color of the text
 * @param pos The starting position of the text and the top left corner of the bounding rectangle
 * @param size The size of the bounding rectangle
 */
void animateTextSlow(char *text, Color color, Position pos, Size size);

/**
 * Same behavior as animateTextSlow() but after each character the specified Sound is played.
 * 
 * @param text The text to be drawn on the GBA screen
 * @param color The color of the text
 * @param sound The sound to play after drawing each character
 * @param pos The starting position of the text and the top left corner of the bounding rectangle
 * @param size The size of the bounding rectangle
 */
void animateTextSound(char *text, Color color, Sound sound, Position pos, Size size);

/**
 * Creates a Sound from the given sound_name.
 * @warning currently only .wav and .mp3 files are supported.
 * 
 * @param sound_name The name of the sound file used for the Sound, located in the Sounds/ folder. This should not include the file extension. So to use Jazz.mp3, pass in "Jazz", for example.
 * 
 * @returns Your shiny new Sound in a cool Sound struct
 */
Sound createSound(char *sound_name);

/**
 * Plays the specified sound, with an option to loop the sound or not.
 * 
 * @param sound The Sound to be played
 * @param loop If true, \p sound will play on loop
 */
void playSound(Sound sound, bool loop);

/**
 * Stops the specified sound if it is playing.
 * 
 * @param sound The Sound to stop
 */
void stopSound(Sound sound);

/**
 * Pauses a specified sound so it can be resumed from the same point later.
 * 
 * @param sound The Sound to pause
 */
void pauseSound(Sound sound);

/**
 * Resumes a specified sound that was previously paused.
 * 
 * @param sound The Sound to resume
 */
void resumeSound(Sound sound);

/**
 * Sets the volume of a Sound. It doens't matter if the Sound is currently playing or not.
 * 
 * @param sound The Sound to set the volume of
 * @param volume The volume to set \p sound to. In the range of [0-100] (inclusive)
 */
void setVolume(Sound sound, int volume);

/**
 * Gets the current volume of a Sound. 
 * 
 * @param sound The sound to get the volume of
 * 
 * @returns The current volume of \p sound, in the range [0-100]
 */
int getVolume(Sound sound);

#endif