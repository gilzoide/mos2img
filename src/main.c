#include "mos2img.h"
#include <argp.h>

/*   Argp stuff   */
const char *argp_program_version = "mos2img 0.1.0";
const char *argp_program_bug_address = "<gilzoide@gmail.com>";
static char doc[] = "A utility to convert *.mosi files into png images.";
static char args_doc[] = "FILE";
/// our options
static struct argp_option options[] = {
	{"font-size", 'f', "size", 0, "Set the font size. Default is `30.0'."},
	{"no-color", 'n', 0, 0,  "Produce monochromatic output."},
	{"output", 'o', "file", 0, "The output file name. \
Default is `image.png'."},
	{"stream", 's', 0, 0, "Get mosaic in a stream fashion, perfect for \
when piping from other program. Ignores file name."},
	{"transparent", 't', 0, 0, "Uses transparent as default background instead \
of black."},
	{ 0 }
};
/// Used by main to communicate with parse_opt
struct arguments {
	char *input, *output;
	char color, stream, transparent;
	double font_size;
};
/// The parsing function
error_t parse_opt (int key, char *arg, struct argp_state *state) {
	struct arguments *argumentos = (struct arguments*) state->input;

	switch (key) {
		case 'n':
			argumentos->color = 0;
			break;
		case 's':
			argumentos->stream = 1;
			break;
		case 't':
			argumentos->transparent = 1;
			break;
		case 'o':
			argumentos->output = arg;
			break;
		case 'f':
			argumentos->font_size = atof (arg);
			break;

		case ARGP_KEY_ARG:
			if (state->arg_num >= 1) {
				/* Too many arguments. */
				argp_usage (state);
			}
			argumentos->input = arg;
			break;

		case ARGP_KEY_END:
			if (state->arg_num < 1 && !argumentos->stream) {
				/* Too few arguments, if not waiting from stdin. */
				argp_usage (state);
			}
			break;

		default:
			return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

/// Our argp parser
static struct argp argp = { options, parse_opt, args_doc, doc };

int main (int argc, char **argv) {
	char *default_file_name = "image.png";

	struct arguments arguments;
	arguments.color = 1;
	arguments.stream = 0;
	arguments.transparent = 0;
	arguments.font_size = 30;
	arguments.output = default_file_name;
	// parse arguments
	argp_parse (&argp, argc, argv, 0, 0, &arguments);

	// image to be loaded
	MOSAIC *img = NewMOSAIC (0, 0);
	if (!img) {
		fprintf (stderr, "Couldn't create MOSAIC =/\n");
		exit (1);
	}

	// load from file or stdin
	int load_result = (!arguments.stream) ? LoadMOSAIC (img, arguments.input) :
			fgetMOSAIC (img, stdin);

	// if unknown format, can't genarate image with colors
	if (load_result == EUNKNSTRGFMT || load_result == ERR) {
		if (arguments.color) {
			fprintf (stderr, "Couldn't figure out attribute format."
					" Disabling colors!\n");

			arguments.color = 0;
		}
		load_result = 0;
	}

	if (load_result == 0) {
		CreateAndSavePNG (img, arguments.output, arguments.font_size,
				arguments.color, arguments.transparent);
	}
	else if (load_result == ENODIMENSIONS) {
		fprintf (stderr, "There are no dimensions in this file..."
				"It's probably not a mosaic image!\n");
	}
	else {
		fprintf (stderr, "Couldn't load file. %s.\n", strerror (errno));
	}

	FreeMOSAIC (img);

	return 0;
}
