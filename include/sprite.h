#ifndef SPRITE_H
#define SPRITE_H

#include "types.h"
#include "gbalib.h"
// *** Sprites =======================================================

typedef struct { u16 tileimg[8192]; } charblock;
typedef struct { u16 tilemap[1024]; } screenblock;

extern charblock *charbase;
extern screenblock *screenbase;

typedef struct {
    u16 palette_id;
    u16 shape;
    u16 size;
    u16 id; 
    char *image_name;
} ObjAttrImageInfo;

typedef struct {
    char *gif_name;
    ObjAttrImageInfo *frame0_obj;
    u16 num_frames;
} GifInfo;

typedef struct {
    u16 type;
    u16 dimension_type;
    u32 palette_size;
    u32 data_size;
} SpriteInfo;


//Sprite definitions
#define SPRITEMEM  ((volatile ObjAttr *)0x7000000)
#define SPRITEDATA ((volatile u16 *)(0x6010000) )
#define SPRITEPAL  ((volatile u16 *)0x5000200)
#define OBJ_ENABLE 0x1000
#define MODE1D (1<<6)

// ATTR0

// 0-7 Row position of the top of the sprite.

// 8-9 Object Mode
#define ATTR0_REG (0<<8)
#define ATTR0_AFF (1<<8)
#define ATTR0_HIDE (2<<8)
#define ATTR0_AFF_DBL (3<<8)

// 10-11 Graphics Mode for special effects
#define ATTR0_BLEND (1<<10) // Forbidden to use both of these
#define ATTR0_WIN (2<<10)

// 12 Mosaic Effect
#define ATTR0_MOSAIC (1<<12)

// 13 Color Mode
#define ATTR0_4BPP 0          // 16 colors
#define ATTR0_8BPP (1<<13)    // 256 colors

// 14-15 Shape
#define ATTR0_SQUARE 0
#define ATTR0_WIDE (1<<14)
#define ATTR0_TALL (2<<14)

// ATTR1

// 0-8 Column position of left hand side of sprite

// 9-13 Valid if Affine Flag (Attribute 0 Bit 8)
// Defines which OAM_AFF_ENTY this sprite uses.

// 12-13
#define ATTR1_NOFLIP 0
#define ATTR1_HFLIP (1<<12)
#define ATTR1_VFLIP (1<<13)
// Used only if the Affine Flag (Attribute 0 Bit 8) is clear

// 14-15 Size...Together with the shape bits Attribute 0 Bits 14-15
// these determine the sprite's real size, see table:

/*-------------------------------------------------------------
 *
 *       Size                   00      01      10      11
 *   Shape
 *              00              8x8     16x16   32x32   64x64
 *              01              16x8    32x8    32x16   64x32
 *              10              8x16    8x32    16x32   32x64
 *------------------------------------------------------------*/
#define ATTR1_SIZE8 0
#define ATTR1_SIZE16 (1<<14)
#define ATTR1_SIZE32 (2<<14)
#define ATTR1_SIZE64 (3<<14)

// ATTR2

//0-9 Base tile-index of sprite. Note that in bitmap modes this must
// be 512 or higher.

// 10-11 Priority. Higher priorities are drawn first (and therefore
// can be covered by later sprites and backgrounds). Sprites cover
// backgrounds of the same priority, and for sprites of the same
// priority, the higher OBJ_ATTRs are drawn first.
#define ATTR2_PRI0 0
#define ATTR2_PRI1 (1<<10)
#define ATTR2_PRI2 (2<<10)
#define ATTR2_PRI3 (3<<10)

// 12-15 Palette-bank to use when in 16-color mode. Has no effect if
//the color mode flag Attribute 0 Bit 12 is set.
#define ATTR2_PALETTE_BANK(pbn) ((pbn)<<12)

void populateSpriteImages();
SpriteInfo getSpriteInfo();
GifInfo getGifInfo(char *gif_name);
ObjAttrImageInfo *getObjAttrImageInfo(char *image_name);

#endif