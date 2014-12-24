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

/**
 * Auxiliary struct for holding the color values individualy
 */
typedef struct {
	double fr, fg, fb;	// foreground rgb
	double br, bg, bb, ba;	// background rgba
} mos2img_color_t;

/// Get the fore/background RGBAs
void getRgba (mos2img_color_t *color, mos_attr attr);

/// Do all the work of processing the MOSAIC and turning it into a .png image
void CreateAndSavePNG (MOSAIC *img, const char *output, double font_size, char colored, char transparent);

#endif
