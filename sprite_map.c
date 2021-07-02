#include "sprite.h"
#include "sprite_data.h"

// This is cursed. I apologize

#define NUM_SPRITES /*NUM_SPRITES*/
#define NUM_GIFS /*NUM_GIFS*/

ObjAttr sprite_list[128]; // max 128 sprites
static ObjAttrImageInfo images[NUM_SPRITES];
static GifInfo gifs[NUM_GIFS];
static ObjAttrImageInfo gif_frames[/*TOTAL_FRAMES*/];
static int curr_frame = 0;

static void addGifFrame(u16 palette_id, u16 shape, u16 size, u16 id) {
    gif_frames[curr_frame].palette_id = palette_id;
    gif_frames[curr_frame].shape = shape;
    gif_frames[curr_frame].size = size;
    gif_frames[curr_frame].id = id;
    curr_frame++;
}

static void addGif(char *gif_name, u16 num_frames, u16 frame_id_jump, u16 palette_id, u16 shape, u16 size, u16 start_id) {
    static int curr_gif = 0;
    gifs[curr_gif].gif_name = gif_name;
    gifs[curr_gif].frame0_obj = &gif_frames[curr_frame];
    gifs[curr_gif].num_frames = num_frames;
    for (int i = 0; i < num_frames; i++) {
        addGifFrame(palette_id, shape, size, start_id + i * frame_id_jump);
    }
    curr_gif++;
}

void populateGifs() {
    /*POPULATE_GIFS*/
}

static void addImage(u16 palette_id, u16 shape, u16 size, u16 id, char *image_name) {
    static int curr_image = 0;
    images[curr_image].palette_id = palette_id;
    images[curr_image].shape = shape;
    images[curr_image].size = size;
    images[curr_image].id = id;
    images[curr_image].image_name = image_name;

    curr_image++;
}

void populateSpriteImages() {
    /*POPULATE_SPRITES*/
}

SpriteInfo getSpriteInfo() {
    return (SpriteInfo) {SPRITE_DATA_PALETTE_TYPE, SPRITE_DATA_DIMENSION_TYPE, SPRITE_DATA_PALETTE_LENGTH, SPRITE_DATA_LENGTH};
}

GifInfo getGifInfo(char *gif_name) {
    int len = strlen(gif_name);
    char upper_gif_name[len + 1];
    for (int i = 0; i < len; i++) {
        upper_gif_name[i] = toupper(gif_name[i]);
    }
    upper_gif_name[len] = '\0';
    for (int i = 0; i < NUM_GIFS; i++) {
        if (!strcmp(gifs[i].gif_name, upper_gif_name)) {
            return &gifs[i];
        }
    }
    //TODO: exit angrily
    return &gifs[0];
}

ObjAttrImageInfo *getObjAttrImageInfo(char *image_name) {
    int len = strlen(image_name);
    char upper_img_name[len + 1];
    for (int i = 0; i < len; i++) {
        upper_img_name[i] = toupper(image_name[i]);
    }
    upper_img_name[len] = '\0';
    for (int i = 0; i < NUM_SPRITES; i++) {
        if (!strcmp(images[i].image_name, upper_img_name)) {
            return &images[i];
        }
    }
    //TODO: exit angrily
    return &images[0];
}