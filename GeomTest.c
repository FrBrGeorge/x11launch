#include <X11/Xlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <X11/Xutil.h>

/* Geometry / Label / Font */
int main(int argc, char *argv[]) {
  int x, y, res;
  unsigned int w, h, dw, dh;
  char *label;

  XEvent event;
  XGCValues gv;
  int screen;
  Window root, win;
  Display *dpy;
  XFontStruct *font;
  GC gc;
  XColor fg, bg, hw;
  XSetWindowAttributes attrib;

  res = XParseGeometry(argc>1? argv[1]: "2x3+100+10", &x, &y, &dw, &dh);
  label = argc>2? argv[2] : "Label";

  dpy = XOpenDisplay(NULL);
  screen = DefaultScreen(dpy);
  root = RootWindow(dpy, screen);

  font = XLoadQueryFont(dpy, argc>3 ? argv[3]: "fixed");
  h = font->ascent + font->descent;
  XAllocNamedColor(dpy, DefaultColormap(dpy, screen), "midnightblue", &bg, &hw);
  XAllocNamedColor(dpy, DefaultColormap(dpy, screen), "peachpuff", &fg, &hw);

  gv.font = font->fid;
  gv.foreground = fg.pixel;
  gv.background = bg.pixel;
  gv.function = GXcopy;

  gc = XCreateGC(dpy, root, GCFunction | GCForeground | GCBackground | GCFont, &gv);

  attrib.override_redirect = True;
  attrib.background_pixel = bg.pixel;

  w = XTextWidth(font, label, strlen(label));
  win = XCreateWindow(dpy, root, x, y, w+2*dw, h+2*dh, dw, 
      			CopyFromParent, InputOutput, CopyFromParent,
			CWOverrideRedirect | CWBackPixel, &attrib);


  XSelectInput(dpy, win, EnterWindowMask | ExposureMask);
  XMapWindow(dpy, win);

  printf("# Window %lx: font: %p, color: %lx on %lx\n", win, font, fg.pixel, bg.pixel);
  while(printf("#%d#\n", XNextEvent(dpy, &event))) {
    switch (event.type) {
      case EnterNotify: XRaiseWindow(dpy, win);
			printf("%lx: %d:%d / %d:%d\n", event.xcrossing.window,
			    event.xcrossing.x, event.xcrossing.y,
			    event.xcrossing.x_root, event.xcrossing.y_root);

      			break;
      default: printf("WAT\n");
    }
    XDrawString(dpy, win, gc, dw, h-dh, label, strlen(label));
  }

  return 0;
}
