#include "mos2img.h"

void CreateAndSavePNG (MOSAIC *img, const char *output) {
	cairo_surface_t *surface;
	cairo_t *cr;

	surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 1000, 1000);
	cr = cairo_create (surface);

	cairo_set_source_rgb (cr, 255, 255, 255);
	cairo_select_font_face (cr, "monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size (cr, 20.0);

	// get the font extents, so that we know how to draw it right
	cairo_font_extents_t fe;
	cairo_font_extents (cr, &fe);

	// the current letter, expressed in a null terminated string
	char letter[2] = {0, 0};
	int i, j;
	for (i = 0; i < img->height; i++) {
		cairo_move_to (cr, 10, (i + 1) * fe.height);
		for (j = 0; j < img->width; j++) {
			letter[0] = _mosGetCh (img, i, j);

			cairo_show_text (cr, letter);
		}
	}

	cairo_surface_write_to_png (surface, output);

	cairo_destroy (cr);
	cairo_surface_destroy (surface);
}
