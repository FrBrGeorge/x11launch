#include <stdlib.h>
#include <argp.h>

const char *argp_program_version =
  "x11launch 0.1";
const char *argp_program_bug_address =
  "<george@altlinux.org>";

/* Program documentation. */
static char doc[] =
  "The x11launch tool is a simple X11 menu launcher that occupies one pixel area on creen edge and shows leveled menu when pointed by mouse. It executes an application by mouse click or just hover on corresponded menu item.\vTODO\n\n"
  "more detail description";

/* A description of the arguments we accept. */
static char args_doc[] = "label|command ...\nlabel||command ...";

/* The options we understand. */
static struct argp_option options[] = {
  {"verbose",	'v', NULL,	0, "Produce verbose output" },
  {"config",	'c', "FILE",	0, "Read confioguretion from FILE" },
  {"dump",	'd', NULL,	0, "Dump actual parameters in config file format" },
  {"shift",	's', "NUMBER",	0, "Inactive menus shift step" },
  {"border",	'b', "NUMBER",	0, "Border width" },
  {"font",	'f', "FONT",	0, "Menu font"},
  {"ink",	'i', "COLOR",	0, "Current menu item foreground color"},
  {"paper",	'p', "COLOR",	0, "Current menu item background color"},
  {"Ink",	'I', "COLOR",	0, "Current menu item selected foreground color"},
  {"Paper",	'P', "COLOR",	0, "Current menu item selected background color"},
  {"geometry",	'g', "GEOM",	0, "Current menu block geometry"},
  { 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments
{
  int verbose, border, dump;
  float shift;
  char *config, *ink, *paper, *Ink, *Paper, *font, *geometry;
};

/* Initial values of arguments */
static void
default_adguments (struct arguments *arguments) {
  arguments->verbose = 0;
  arguments->dump = 0;
  arguments->shift = 3.;
  arguments->border = 3;
  arguments->config = NULL;
  arguments->ink = "black";
  arguments->paper = "grey75";
  arguments->Ink = "white";
  arguments->Paper = "grey50";
  arguments->font = "fixed";
  arguments->geometry = NULL;
}


/* TODO dump actual arguments */
static void
dump_arguments(struct arguments *arguments) {
  printf("verbose: %d\n", arguments->verbose);
  printf("shift: %f\n", arguments->shift);
  printf("border: %d\n", arguments->border);
  printf("config: %s\n", arguments->config);
  printf("ink: %s\n", arguments->ink);
  printf("paper: %s\n", arguments->paper);
  printf("Ink: %s\n", arguments->Ink);
  printf("Paper: %s\n", arguments->Paper);
  printf("font: %s\n", arguments->font);
  printf("geometry: %s\n", arguments->geometry);
}

/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;

  switch (key)
    {
    case 'v':
      arguments->verbose = 1;
      break;
    case 'd':
      arguments->dump = 1;
      break;
    case 'c':
      arguments->config = arg;
      break;
    case 'i':
      arguments->ink = arg;
      break;
    case 'p':
      arguments->paper = arg;
      break;
    case 'I':
      arguments->Ink = arg;
      break;
    case 'P':
      arguments->Paper = arg;
      break;
    case 'f':
      arguments->font = arg;
      break;
    case 'g':
      arguments->geometry = arg;
      break;
    case 'b':
      arguments->border = atoi(arg);
      break;
    case 's':
      arguments->shift = atof(arg);
      break;
    case ARGP_KEY_ARG:
      /*G*//*TODO*/
      printf("Menu: %s\n", arg);
      break;

    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc };

int
main (int argc, char **argv)
{
  struct arguments arguments;

  /* Default values. */
  default_adguments(&arguments);

  /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
  argp_parse (&argp, argc, argv, ARGP_IN_ORDER, 0, &arguments);
  if(arguments.dump) dump_arguments(&arguments);

  exit (0);
}
