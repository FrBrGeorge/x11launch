#include <stdlib.h>
#include <argp.h>
#include <X11/Xlib.h>

const char *argp_program_version =
  "x11launch 0.1";
const char *argp_program_bug_address =
  "<george@altlinux.org>";

/* Program documentation. */
static char doc[] =
  "The x11launch tool is a simple X11 menu launcher that occupies one pixel area on creen edge and shows leveled menu when pointed by mouse. It executes an application by mouse click or just hover on corresponded menu item.\vTODO\n\n"
  "more detail description and example";

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

struct tab {
  char *label;
  char *cmd;
  int x, y, dx, dy, width, height, group;
  Window win;
};

struct tabgroups {
  struct tab *tabs;
  int ntabs;
  int ngroups;
} groups = {NULL, 0, 1};

/* Initial values of arguments */
static void
default_adguments (struct arguments *args) {
  args->verbose = 0;
  args->dump = 0;
  args->shift = 3.;
  args->border = 3;
  args->config = NULL;
  args->ink = "black";
  args->paper = "grey75";
  args->Ink = "white";
  args->Paper = "grey50";
  args->font = "fixed";
  args->geometry = NULL;
}


/* TODO dump actual arguments */
static void
dump_arguments(struct arguments *args) {
  printf("verbose: %d\n", args->verbose);
  printf("shift: %f\n", args->shift);
  printf("border: %d\n", args->border);
  printf("config: %s\n", args->config);
  printf("ink: %s\n", args->ink);
  printf("paper: %s\n", args->paper);
  printf("Ink: %s\n", args->Ink);
  printf("Paper: %s\n", args->Paper);
  printf("font: %s\n", args->font);
  printf("geometry: %s\n", args->geometry);
}

/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *args = state->input;

  switch (key)
    {
    case 'v':
      args->verbose = 1;
      break;
    case 'd':
      args->dump = 1;
      break;
    case 'c':
      args->config = arg;
      break;
    case 'i':
      args->ink = arg;
      break;
    case 'p':
      args->paper = arg;
      break;
    case 'I':
      args->Ink = arg;
      break;
    case 'P':
      args->Paper = arg;
      break;
    case 'f':
      args->font = arg;
      break;
    case 'g':
      args->geometry = arg;
      break;
    case 'b':
      args->border = atoi(arg);
      break;
    case 's':
      args->shift = atof(arg);
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

void
new_tab(struct tabgroups *grp) {
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc };

int
main (int argc, char **argv)
{
  struct arguments arguments;

  /* TODO: this is overprovisioning */
  groups.tabs = calloc(argc, sizeof(struct tab));

  /* Default values. */
  default_adguments(&arguments);

  /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
  argp_parse (&argp, argc, argv, ARGP_IN_ORDER, 0, &arguments);
  if(arguments.dump) dump_arguments(&arguments);

  exit (0);
}
