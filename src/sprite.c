#include "gba.h"


extern ObjAttr sprite_list[128];
// Private helper functions

static void initSprite(ObjAttr *sprite_obj, u16 attr0, u16 attr1, u16 attr2) {
    sprite_obj->attr0 = attr0;
    sprite_obj->attr1 = attr1;
    sprite_obj->attr2 = attr2;
}

static void drawSprite(char x, char y, ObjAttr *sprite_obj, u16 flipCode) {
    sprite_obj->attr0 = (sprite_obj->attr0 & 0xFC00) | (y & 0x00FF);
    sprite_obj->attr1 = (sprite_obj->attr1 & 0xCE00) | (x & 0x01FF) | flipCode;
}

static ObjAttr *addSprite(ObjAttrImageInfo *info, SpriteInfo s_info) {
    static int sprite_num = 0;
    ObjAttr *sprite_obj = &sprite_list[sprite_num];
    initSprite(sprite_obj, info->shape | s_info.type | ATTR0_HIDE, info->size, info->palette_id | info->id);
    sprite_num++;
    return sprite_obj;
}

// ---------------------------------------------------------------------------
//                       Sprite Library Functions
// ---------------------------------------------------------------------------

Gif createGif(char *gif_name, Size size) {
    GifInfo g_info = getGifInfo(gif_name);
    SpriteInfo s_info = getSpriteInfo();
    ObjAttrImageInfo *frame0_info = g_info.frame0_obj;
    ObjAttr *frame0_obj = addSprite(frame0_info, s_info);
    for (int i = 1; i < g_info.num_frames; i++) {
        addSprite(frame0_info + i, s_info);
    }
    return (Gif) {size, gif_name, frame0_obj, g_info.num_frames, 0};
}

Sprite createSprite(char *image_name, Size size) {
    ObjAttrImageInfo *info = getObjAttrImageInfo(image_name);
    SpriteInfo s_info = getSpriteInfo();
    ObjAttr *sprite_obj = addSprite(info, s_info);
    return (Sprite) {size, info->image_name, sprite_obj};
}

void showGif(Gif gif) {
    
}

void hideGif(Gif gif) {
    ObjAttr *curr_frame = gif.frame0_obj + (size_t) curr_frame;
    curr_frame->attr0 = (curr_frame->attr0 & 0xFCFF) | ATTR0_HIDE;
}

void showSprite(Sprite sprite) {
    sprite.sprite_obj->attr0 &= 0xFCFF;
}

void hideSprite(Sprite sprite) {
    sprite.sprite_obj->attr0 = (sprite.sprite_obj->attr0 & 0xFCFF) | ATTR0_HIDE;
}

bool isSpriteHidden(Sprite sprite) {
    return (sprite.sprite_obj->attr0 & 0x0300) == ATTR0_HIDE; 
}

void updatePosition(Sprite sprite, Position pos) {
    sprite.sprite_obj->attr0 = (sprite.sprite_obj->attr0 & 0xFC00) | (pos.y & 0x00FF);
    sprite.sprite_obj->attr1 = (sprite.sprite_obj->attr1 & 0xCE00) | (pos.x & 0x01FF);
}

Position getPosition(Sprite sprite) {
    u32 x = sprite.sprite_obj->attr1 & 0x01FF;
    u32 y = sprite.sprite_obj->attr0 & 0x00FF;
    return (Position) {x, y};
}

void updateImage(Sprite sprite, char *image_name) {
    ObjAttrImageInfo *info = getObjAttrImageInfo(image_name);
    SpriteInfo s_info = getSpriteInfo();
    sprite.sprite_obj->attr0 = (sprite.sprite_obj->attr0 & 0x03FF) | info->shape | s_info.type;
    sprite.sprite_obj->attr1 = (sprite.sprite_obj->attr1 & 0x01FF) | info->size;
    sprite.sprite_obj->attr2 = info->palette_id | info->id;
}