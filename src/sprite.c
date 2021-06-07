#include "gba.h"


extern ObjAttr sprite_list[32];
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

static void showMovingSprite(ObjAttr *sprite_obj, char slow) {
    for (int i = 0; i < slow; i++) {
        waitForVBlank();
    }
    DMA[3].src = sprite_obj;
    DMA[3].dst = SPRITEMEM;
    DMA[3].cnt = 32 * 4 | DMA_ON;
}

static ObjAttr *addSprite(ObjAttrImageInfo *info, PaletteInfo p_info) {
    static int sprite_num = 0;
    ObjAttr *sprite_obj = &sprite_list[sprite_num];
    initSprite(sprite_obj, p_info.dimension_type | info->shape | p_info.type | ATTR0_HIDE, info->size, info->palette_id | info->id);
    sprite_num++;
    return sprite_obj;
}

// ---------------------------------------------------------------------------
//                       Sprite Library Functions
// ---------------------------------------------------------------------------

Sprite createSprite(char *image_name) {
    ObjAttrImageInfo *info = getObjAttrImageInfo(image_name);
    PaletteInfo p_info = getPaletteInfo();
    ObjAttr *sprite_obj = addSprite(info, p_info);
    return (Sprite) {info->image_name, sprite_obj};
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
    PaletteInfo p_info = getPaletteInfo();
    sprite.sprite_obj->attr0 = sprite.sprite_obj->attr0 & 0x00FF | p_info.dimension_type | info->shape | p_info.type;
    sprite.sprite_obj->attr1 = sprite.sprite_obj->attr1 & 0x01FF | info->size;
    sprite.sprite_obj->attr2 = info->palette_id | info->id;
}