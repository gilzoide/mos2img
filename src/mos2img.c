#include "mos2img.h"

/**
 * Gets the width and height necessary for creating the .png image.
 *
 * It relies on the MOSAIC's size and font extents.
 *
 * @param[in] img The mosaic
 * @param[out] width The width found
 * @param[out] height The height found
 */
void getPngDimensions (MOSAIC *img, int *width, int *height) {
	// create a surface and context for getting the desired font extents
	cairo_surface_t *surface;
	cairo_t *cr;

	surface = cairo_image_surface_create (CAIRO_FORMAT_RGB24, 1, 1);
	cr = cairo_create (surface);

	cairo_select_font_face (cr, "monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size (cr, 30.0);
	cairo_font_extents_t fe;
	cairo_font_extents (cr, &fe);

	*width = fe.max_x_advance * img->width + 3 * X_PADDING;
	*height = fe.height * img->height;
	
	cairo_destroy (cr);
	cairo_surface_destroy (surface);
}

void CreateAndSavePNG (MOSAIC *img, const char *output, char color, char transparent) {
	cairo_surface_t *surface;
	cairo_t *cr;

	int height, width;
	getPngDimensions (img, &width, &height);

	surface = cairo_image_surface_create (transparent ? CAIRO_FORMAT_ARGB32 : 
			CAIRO_FORMAT_RGB24, width, height);
	cr = cairo_create (surface);

	cairo_set_source_rgb (cr, 255, 255, 255);
	cairo_select_font_face (cr, "monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size (cr, 30.0);

	// get the font extents, so that we know how to draw it right
	cairo_font_extents_t fe;
	cairo_font_extents (cr, &fe);

	// the current letter, expressed in a null terminated string
	char letter[2] = {0, 0};
	int i, j;
	mos_attr attr, bold;
	for (i = 0; i < img->height; i++) {
		// those literal numbers are tottally arbitrary
		cairo_move_to (cr, X_PADDING, fe.ascent + i * fe.height);
		for (j = 0; j < img->width; j++) {
			if (color) {
				attr = _mosGetAttr (img, i, j);
				bold = extractBold (&attr);
				cairo_select_font_face (cr, "monospace", CAIRO_FONT_SLANT_NORMAL,
						bold ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL);
			}
			letter[0] = _mosGetCh (img, i, j);

			// as we show only one letter at a time, and it's monospace,
			// there's no need to move in the X axis
			cairo_show_text (cr, letter);
		}
	}

	cairo_surface_write_to_png (surface, output);

	cairo_destroy (cr);
	cairo_surface_destroy (surface);
}
