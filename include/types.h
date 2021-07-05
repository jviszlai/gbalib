#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// ---------------------------------------------------------------------------
//                       USEFUL TYPEDEFS
// ---------------------------------------------------------------------------
/** An unsigned 32-bit (4-byte) type */
typedef unsigned int u32;

/** An unsigned 16-bit (2-byte) type */
typedef unsigned short u16;

/** An unsigned 8-bit (1-byte) type. Note that this type cannot be written onto RAM directly. */
typedef unsigned char u8;

// ---------------------------------------------------------------------------
//                       COMMON TYPES
// ---------------------------------------------------------------------------

typedef struct {
    u16 attr0;
    u16 attr1;
    u16 attr2;
    u16 fill;
} ObjAttr;

typedef struct {
    u16 palette_id;
    u16 shape;
    u16 size;
    u16 id; 
    char *image_name;
} ObjAttrImageInfo;

typedef struct {
    ObjAttr *gif_obj;
    ObjAttrImageInfo *frame0_info;
    u16 num_frames;
    u16 curr_frame;
} GifInfo;

typedef struct {
    u8 volume;
    u8 channel;
} SoundData;

#endif