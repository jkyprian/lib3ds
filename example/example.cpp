#include "../src/lib3ds.h"
#include <gtk/gtk.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <gtkgl/gtkglarea.h>


#define VIEW_ASPECT 1.3


static gint doInit=0;          /* true if initgl not yet called */
static C3dsFile a3ds;          /* 3ds file data */
static float s_time=0.0;
static GtkWidget *window, *frame, *glarea;


void renderInst(C3dsInst *i) 
{
  int j,k;

  glMultMatrixd(&i->pInstNode->matrix[0][0]);
  glTranslated(-i->pInstNode->pivot[0],-i->pInstNode->pivot[1],-i->pInstNode->pivot[2]);
  C3dsMesh *m=i->pMesh;
  glColor3d(0,0,0);

  glBegin(GL_TRIANGLES);
    for (j=0; j<m->faces; j++) {
      C3dsFace *f=&m->faceL[j];
      //glNormal3d(f->normal[0], f->normal[1], f->normal[2]);
      for (k=0; k<3; k++) {
        glNormal3d(
          f->edgeL[k].normal[0],
          f->edgeL[k].normal[1],
          f->edgeL[k].normal[2]
        );
        glVertex3d(
          m->vertexL[f->edgeL[k].v].pos[0],
          m->vertexL[f->edgeL[k].v].pos[1],
          m->vertexL[f->edgeL[k].v].pos[2]
        );
      }
    }
  glEnd();
}


void initgl(void)
{
  GLfloat light0_pos[4]   = { 0.0, 0.0, 500.0, 0.0 };
  GLfloat light0_color[4] = { 1.0, 1.0, 1.0, 1.0 }; 

  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);
  
  glShadeModel(GL_SMOOTH);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

  glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_color);  
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
    
  //glColorMaterial(GL_FRONT,GL_DIFFUSE);
  glDisable(GL_COLOR_MATERIAL);  
}


gint glarea_expose(GtkWidget *widget, GdkEventExpose *event)
{
  //GLfloat m[4][4];

  GtkGLArea *glarea = GTK_GL_AREA(widget);

  /* draw only last expose */
  if (event->count > 0) {
    return TRUE;
  }

  /* OpenGL calls can be done only if make_current returns true */
  if (gtk_gl_area_make_current(glarea)) {
    /* basic initialization */
    if (doInit == TRUE) {
      initgl();
      doInit = FALSE;
    }

    /* view */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, VIEW_ASPECT, 1,20000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-90, 1.0,0,0);

    glTranslatef(0,1000,0);
    glRotatef(-30, 0,0,1);

    /* draw object */
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    {
      C3dsInstList::iterator p;
      for (p=a3ds.instL.begin(); p!=a3ds.instL.end(); ++p) {
        glPushMatrix();
        renderInst(*p);
        glPopMatrix();
      }
    }

    /* swap backbuffer to front */
    gtk_gl_area_swapbuffers(glarea);
  }

  return TRUE;
}


gint glarea_configure(GtkWidget *widget, GdkEventConfigure *event)
{
  /* OpenGL calls can be done only if make_current returns true */
  if (gtk_gl_area_make_current(GTK_GL_AREA(widget))) {
    glViewport(0,0, widget->allocation.width, widget->allocation.height);
  }
  return TRUE;
}


gint glarea_destroy(GtkWidget *widget)
{
  return TRUE;
}


gint window_count = 0; /* number of windows on screen */
gint window_destroy(GtkWidget *widget)
{
  /* if this was last window quit */
  if (--window_count == 0)
    gtk_main_quit();
  return TRUE;
}


gint idle_func(gpointer data)
{
  s_time+=1.0;
  if ((s_time<a3ds.frames.from)||(s_time>a3ds.frames.to)) {
    s_time=a3ds.frames.from;
  }
  a3ds.animate(s_time);
  gtk_widget_draw(glarea,NULL);
  return(TRUE);
}


gint show_3ds(const char *a3ds_name)
{
  /* read 3ds file */
  {
    try {
      FILE *f=fopen(a3ds_name,"rb");
      if (!f) {
        printf("ERROR open\n");
        exit(1);
      }
      load3DS(&a3ds,f);
      fclose(f);
      printf("O.K.\n");
    }
    catch(...) {
      printf("ERROR\n");
    }
  }

  /* create aspect frame */
  frame = gtk_aspect_frame_new(NULL, 0.5,0.5, VIEW_ASPECT, FALSE);

  /* create new OpenGL widget */
  glarea = gtk_gl_area_new_vargs(NULL, /* no sharing */
				 GDK_GL_RGBA,
				 GDK_GL_DOUBLEBUFFER,
				 GDK_GL_DEPTH_SIZE,1,
				 GDK_GL_RED_SIZE,1,
				 GDK_GL_GREEN_SIZE,1,
				 GDK_GL_BLUE_SIZE,1,
				 GDK_GL_NONE);  /* last argument must be GDK_GL_NONE */
  if (glarea == NULL) {
    g_print("Can't create GtkGLArea widget\n");
    return FALSE;
  }
  /* set up events and signals for OpenGL widget */
  gtk_widget_set_events(glarea,
			GDK_EXPOSURE_MASK|
			GDK_BUTTON_PRESS_MASK|
			GDK_BUTTON_RELEASE_MASK|
			GDK_POINTER_MOTION_MASK|
			GDK_POINTER_MOTION_HINT_MASK);
  gtk_signal_connect (GTK_OBJECT(glarea), "expose_event",
		      GTK_SIGNAL_FUNC(glarea_expose), NULL);
  gtk_signal_connect (GTK_OBJECT(glarea), "configure_event",
		      GTK_SIGNAL_FUNC(glarea_configure), NULL);
  gtk_signal_connect (GTK_OBJECT(glarea), "destroy",
		      GTK_SIGNAL_FUNC (glarea_destroy), NULL);

  gtk_widget_set_usize(glarea, 200,(int)(200/VIEW_ASPECT)); /* minimum size */

  /* set up mesh info */
  doInit = TRUE;

  /* create new top level window */
  window = gtk_window_new( GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "3DS viewer");
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  gtk_signal_connect (GTK_OBJECT(window), "destroy",
		      GTK_SIGNAL_FUNC(window_destroy), NULL);
  window_count++;

  /* destroy this window when exiting from gtk_main() */
  gtk_quit_add_destroy(1, GTK_OBJECT(window));


  /* put glarea into window and show it all */
  gtk_container_add(GTK_CONTAINER(window), frame);
  gtk_container_add(GTK_CONTAINER(frame),glarea);
  gtk_widget_show(glarea);
  gtk_widget_show(frame);
  gtk_widget_show(window);

  return TRUE;
}


int
main (int argc, char **argv)
{
  /* initialize gtk */
  gtk_init( &argc, &argv );

  /* Check if OpenGL (GLX extension) is supported. */
  if (gdk_gl_query() == FALSE) {
    g_print("OpenGL not supported\n");
    return 0;
  }

  /* help? */
  if (argc != 2) {
    g_print("%s", "Syntax: eample 3ds_file\n");
    return 0;
  }

  show_3ds(argv[1]);
  gtk_idle_add(idle_func,NULL);

  if (window_count > 0)
    gtk_main();

  //dmalloc_shutdown();
  return 0;
}
