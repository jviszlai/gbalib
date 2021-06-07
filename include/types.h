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
    u32 x;
    u32 y;
} Position;

typedef struct {
    u32 width;
    u32 height;
} Size;

#endif