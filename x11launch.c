#include <stdlib.h>
#include <argp.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string.h>
#include <error.h>

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

struct tab {
  char *label;
  char *cmd;
  char *sfont, *sink, *spaper, *sInk, *sPaper;
  int x, y, dx, dy;
  int group;
  int immediate;
  float shift;
  XColor *ink, *paper, *Ink, *Paper;
  XFontStruct *font;
  Window win;
};

/* global namespace */
struct tabgroups {
  struct tab *tabs;
  int ntabs;
  int ngroups;
  Display *dpy;
  Window root;
  int screen;
};

/* Used by main to communicate with parse_opt.  */
struct arguments
{
  int verbose, border, dump;
  float shift;
  char *config, *ink, *paper, *Ink, *Paper, *font, *geometry;
  struct tabgroups *grp;
};

/* Initial values of arguments */
static void
default_adguments (struct arguments *args, struct tabgroups *grp) {
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
  args->geometry = "1x0+0+0"; /* TODO: center is better */
  args->grp = grp;
}


/* TODO dump actual arguments */
static void
dump_arguments(struct arguments *args) {
  int i;
  struct tab *T = args->grp->tabs;

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
  printf("\tMenu:\n");
  for(i=0; i<args->grp->ntabs; i++) {
    printf("Label: %s | Command: %s\n", T[i].label, T[i].cmd);
    printf("\tGroup: %d\n", T[i].group);
    printf("\tShift: %f\n", T[i].shift);
    printf("\tImmediate: %d\n", T[i].immediate);
    printf("\tLocation: %dx%d%+d%+d\n", T[i].dx, T[i].dy, T[i].x, T[i].y);
    printf("\tFont: %s\n", T[i].sfont);
    printf("\tInk: %s/%s, paper: %s/%s\n", T[i].sink, T[i].sInk, T[i].spaper, T[i].sPaper);
  }
}


void
new_tab(struct arguments *args, char *labelcmd) {
      struct tab *T = args->grp->tabs + args->grp->ntabs;
      char *sep;
      unsigned int res, w, h;

      args->grp->ntabs++;
      /* parse label, cmd and immediate; TODO deal with \| */
      if((sep = strchr(labelcmd, '|')) && labelcmd[1]) {
	T->label = strndup(labelcmd, sep-labelcmd);
	if((T->immediate = (sep == strstr(labelcmd, "||"))))
	  T->cmd = strdup(sep+2);
	else
	  T->cmd = strdup(sep+1);
      } else {
	T->label = strdup(labelcmd);
	T->cmd = NULL;
	T->immediate = 0;
      }
      T->group = args->grp->ngroups-1;
      T->shift = args->shift;
      res = XParseGeometry(args->geometry, &T->x, &T->y, &w, &h);
      if(!res && strlen(args->geometry)>0)
	error(EINVAL, EINVAL, "Invalid geometry '%s'", args->geometry);
      T->dx = XNegative&res? -w : w;
      T->dy = YNegative&res? -h : h;
      T->sfont = args->font;
      T->sink = args->ink; T->spaper = args->paper;
      T->sInk = args->Ink; T->sPaper = args->Paper;
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
      args->grp->ngroups++;
      break;
    case 'b':
      args->border = atoi(arg);
      break;
    case 's':
      args->shift = atof(arg);
      break;
    case ARGP_KEY_ARG:
      new_tab(args, arg);
      break;

    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc };


#define NewRes(res, tres, obj, tobj, create, errmsg) \
  if(res!=tres) { \
    res = tres; \
    if(!(obj = create)) \
      error(EINVAL, EINVAL, errmsg, res); \
    tobj = obj; \
  }

void create_windows(struct tabgroups *grp) {
  int i;
  int g=-1;
  struct tab *T;
  char *sfont=NULL, *sink=NULL, *spaper=NULL, *sInk=NULL, *sPaper=NULL;
  XColor *ink, *paper, *Ink, *Paper;
  XFontStruct *font;
  Window win;

  /* TODO non-LTR window packing, deal with dx/dy */
  for(i=0; i<grp->ntabs; i++) {
    T = grp->tabs+i;
    NewRes(sfont, T->sfont, font, T->font,
	XLoadQueryFont(grp->dpy, sfont), "Cannot register font %s");
  }
}

int
main (int argc, char **argv)
{
  struct arguments arguments;
  struct tabgroups groups = {NULL, 0, 1, NULL};

  /* TODO: this is overprovisioning */
  groups.tabs = calloc(argc, sizeof(struct tab));

  /* Default values. */
  default_adguments(&arguments, &groups);

  /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
  argp_parse (&argp, argc, argv, ARGP_IN_ORDER, 0, &arguments);
  if(arguments.dump) dump_arguments(&arguments);

  if(!(groups.dpy = XOpenDisplay(NULL)))
    error(ECONNREFUSED, ECONNREFUSED, "Cannot connect to X server");
      
  groups.screen = DefaultScreen(groups.dpy);
  groups.root = RootWindow(groups.dpy, groups.screen);

  create_windows(&groups);

  exit (0);
}
