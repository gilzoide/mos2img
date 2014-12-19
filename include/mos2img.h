/** @file mos2img.h
 */
#ifndef MOS2IMG_H
#define MOS2IMG_H

#include <mosaic/mosaic.h>
#include <mosaic/stream_io.h>

#include <cairo/cairo.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// The X padding, a space in pixels to pad from the left border
#define X_PADDING 2

/// Do all the work of processing the MOSAIC and turning it into a .png image
void CreateAndSavePNG (MOSAIC *img, const char *output, char color, char transparent);

#endif
