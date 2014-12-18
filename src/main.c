#include "mos2img.h"
#include <argp.h>

/*   Argp stuff   */
const char *argp_program_version = "mos2img 0.1.0";
const char *argp_program_bug_address = "<gilzoide@gmail.com>";
static char doc[] = "A utility to convert *.mosi files into images.";
static char args_doc[] = "FILE";
/// our options
static struct argp_option options[] = {
	{"dimensions",  'd', 0, 0, "Show image dimensions"},
	{"color", 'c', 0, 0,  "Produce colored output" },
	{"stream", 's', 0, 0, "Output mosaic in a stream fashion, perfect for \
piping into other program"},
	{ 0 }
};
/// Used by main to communicate with parse_opt
struct arguments {
	char *input;
	char dimensions, color, stream;
};
/// The parsing function
error_t parse_opt (int key, char *arg, struct argp_state *state) {
	struct arguments *argumentos = (struct arguments*) state->input;

	switch (key) {
		case 'c':
			argumentos->color = 1;
			break;
		case 'd':
			argumentos->dimensions = 1;
			break;
		case 's':
			argumentos->stream = 1;
			break;

		case ARGP_KEY_ARG:
			if (state->arg_num >= 1)
				/* Too many arguments. */
				argp_usage (state);

			argumentos->input = arg;
			break;

		case ARGP_KEY_END:
			if (state->arg_num < 1)
				/* Not enough arguments. */
				argp_usage (state);
			break;

		default:
			return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

/// Our argp parser
static struct argp argp = { options, parse_opt, args_doc, doc };

int main (int argc, char **argv) {
	struct arguments arguments;
	arguments.color = 0;
	arguments.dimensions = 0;
	arguments.stream = 0;
	// parse arguments
	argp_parse (&argp, argc, argv, 0, 0, &arguments);


	InitSDL ();

	QuitSDL ();
	return 0;
}
