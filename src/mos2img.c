#include "mos2img.h"

void CreateAndSavePNG (MOSAIC *img, const char *output, char color, char transparent) {
	cairo_surface_t *surface, *aux_surface;
	cairo_t *cr, *aux_cr;

	surface = cairo_image_surface_create (transparent ? CAIRO_FORMAT_ARGB32 : 
			CAIRO_FORMAT_RGB24, 1000, 1000);
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
		cairo_move_to (cr, X_PADDING, (i + 1) * fe.height);
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
