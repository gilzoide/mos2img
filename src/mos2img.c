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
void getPngCharDimensions (MOSAIC *img, double font_size, int *char_width, int *char_height) {
	// create a surface and context for getting the desired font extents
	cairo_surface_t *surface;
	cairo_t *cr;

	surface = cairo_image_surface_create (CAIRO_FORMAT_RGB24, 1, 1);
	cr = cairo_create (surface);

	cairo_select_font_face (cr, "monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size (cr, font_size);
	cairo_font_extents_t fe;
	cairo_font_extents (cr, &fe);

	*char_width = fe.max_x_advance;
	*char_height = fe.height;
	
	cairo_destroy (cr);
	cairo_surface_destroy (surface);
}


void CreateAndSavePNG (MOSAIC *img, const char *output, double font_size, char colored, char transparent) {
	// one char's dimension, for drawing the mosaic
	int char_width, char_height;
	getPngCharDimensions (img, font_size, &char_width, &char_height);
	// the entire image's dimension, for building the cairo surface right
	const int width = img->width * char_width;
	const int height = img->height * char_height;

	// cairo stuff
	cairo_surface_t *surface = cairo_image_surface_create (transparent ? CAIRO_FORMAT_ARGB32 : 
			CAIRO_FORMAT_RGB24, width, height);
	cairo_t *cr = cairo_create (surface);

	cairo_set_source_rgb (cr, 1, 1, 1);
	cairo_select_font_face (cr, "monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size (cr, font_size);

	// get the font extents, so that we know how to draw it right
	cairo_font_extents_t fe;
	cairo_font_extents (cr, &fe);

	// the current letter, expressed in a null terminated string
	char letter[2] = {0, 0};
	int i, j;	// pointers
	mos_attr attr, bold;	// auxiliary for the attr
	mos2img_color_t color;	// and one more
	for (i = 0; i < img->height; i++) {
		cairo_move_to (cr, 0, fe.ascent + i * char_height);
		for (j = 0; j < img->width; j++) {
			if (colored) {
				// get the attribute
				attr = _mosGetAttr (img, i, j);
				getRgba (&color, attr);
				//   Draw the background 
				cairo_set_source_rgba (cr, color.br, color.bg, color.bb, color.ba);
				cairo_rectangle (cr, j * char_width,
						i * char_height, char_width, char_height);
				cairo_fill (cr);
				//   Draw the char
				// bold?
				bold = extractBold (&attr);
				cairo_select_font_face (cr, "monospace", CAIRO_FONT_SLANT_NORMAL,
						bold ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL);
				cairo_set_source_rgb (cr, color.fr, color.fg, color.fb);
				cairo_move_to (cr, j * char_width, fe.ascent + i * char_height);
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


void getRgba (mos2img_color_t *color, mos_attr attr) {
	mos_attr fore = GetFore (attr);
	mos_attr back = GetBack (attr);

	// colors table. Note that it skips Normal, so use `attr - 1' to index it
	const char colors_rgb[][3] = {
		{0, 0, 0},
		{1, 0, 0},
		{0, 1, 0},
		{1, 1, 0},
		{0, 0, 1},
		{1, 0, 1},
		{0, 1, 1},
		{1, 1, 1}
	};

	// if Normal, use default colors
	if (fore == Normal) {
		fore = NW;
	}
	if (back == Normal) {
		back = NBk;
		color->ba = 0;
	}
	else {
		color->ba = 1;
	}
	// color--, so we can use directly with the colors_rgb table
	fore--;
	back--;

	color->fr = colors_rgb[fore][0];
	color->fg = colors_rgb[fore][1];
	color->fb = colors_rgb[fore][2];

	color->br = colors_rgb[back][0];
	color->bg = colors_rgb[back][1];
	color->bb = colors_rgb[back][2];
}
