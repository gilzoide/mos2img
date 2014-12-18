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

void CreateAndSavePNG (MOSAIC *img, const char *output);

#endif
