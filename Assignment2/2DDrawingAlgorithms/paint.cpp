/* 
 * paint.c
 *
 * This program illustrates the use of the glut library for
 * interfacing with a window system.  It came as part of the code
 * packages for one of the OpenGl books, probably Ed Angel's. 
 * 
 * I have modified it to localize areas that you will need to 
 * change to implement a line drawing (drawLine) and a polygon
 * fill (drawPolygon) routines.
 * 
 * The input to this program is totally interactive.  The left
 * mouse button is used to select draw modes.  One chooses what
 * to draw by left clicking in the appropriate object box.  One 
 * chooses WHERE to draw the object chosen by left-clicking in
 * the drawing area.  E.g., you must provide a starting and an
 * ending left click for both the line and the rectangle.  The
 * triangle requires 3 left clicks.  In these three cases, the
 * object is drawn only once.  To draw a second object, you must
 * select the object to draw again.  Points are a bit different:
 * Once selected you may draw as many points as you like before
 * changing drawing mode.
 * 
 * The right mouse button controls a set of cascading menus that
 * allow you to turn fill on and off, to change drawing color
 * and to clear the drawing screen or to quit.
 * 
 */
#include "drawStuff.h"
#include <windows.h>
#define NULL 0
#include <GL/glut.h>         /* glut.h includes gl.h and glu.h*/

void mouse( int, int, int, int );
void display( void );
void myReshape( GLsizei, GLsizei );

void myinit( void );

void drawLineLoop( int, int [], int [] );

void screen_box( int, int, int );
void right_menu( int );
void middle_menu( int );
void color_menu( int );
void fill_menu( int );

int pick( int, int );
void setPixel (int x, int y);

/* 
 * Global variables
 */

GLsizei windowHeight = 500, windowWidth = 500; /* initial window size */

GLfloat red = 1.0, green = 1.0, blue = 1.0;    /* drawing color */
int fill = 0;			               /* fill flag */


/* 
 * Callback function for reshaping window when resized or moved 
 */

void myReshape( GLsizei width, GLsizei height ) {

   /* 
    * Adjust clipping area, i.e., viewWindow 
    */
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity( ); 
   glOrtho( 0.0, (GLdouble) width, 0.0, (GLdouble) height, -1.0, 1.0 );

   /* 
    * Adjust viewport
    */
   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity( ); 
   glViewport( 0, 0, width, height );

   /*
    * Clear screen window
    */
   glClearColor ( 1.0, 1.0, 1.0, 1.0 );
   glClear( GL_COLOR_BUFFER_BIT );

   glutPostRedisplay( );
   glFlush( );

   /* 
    * Set global size for use by drawing routine 
    */
   windowWidth = width;
   windowHeight = height; 

}


/* 
 * Initialization 
 */

void myinit( void ) {

   /* 
    * Set viewport, GL_MODELVIEW matrix is default
    */
   glViewport( 0, 0, windowWidth, windowHeight );

   /* 
    * Set 2D clipping area to match size of X window 
    * This choice avoids having to scale object coordinates
    * each time window is resized 
    */
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity( ); 
   glOrtho( 0.0, (GLdouble) windowWidth , 0.0, 
          (GLdouble) windowHeight , -1.0, 1.0 );

   /* 
    * Set clear color to black and clear screen window 
    */
   glClearColor ( 0.0, 0.0, 0.0, 1.0 );
   glClear( GL_COLOR_BUFFER_BIT );

   /*
    * Flush GL buffers to the display (screen window) 
    */
   glFlush( );

}


/* 
 * Draw line loop of n lines
 */

void drawLineLoop( int n, int *x, int *y ) {

   int i;

   drawLine( x[n-1], y[n-1], x[0], y[0] );
   for (i = 0; i < n-1; i++ ) {
      drawLine( x[i], y[i], x[i+1], y[i+1] );
   }

}


/* 
 * Callback function for mouse interaction 
 */

void mouse( int button, int state, int x, int y ) {

   static int draw_mode = 0;    /* drawing mode */
   static int count;		/* left clicks made in this drawing mode */
   static int xp[4], yp[4];	/* holds location of clicks made in this
				   drawing mode */

   int where;			/* where the click was made */

   if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {

      /* 
       * Saves all current attributes 
       */
      glPushAttrib( GL_ALL_ATTRIB_BITS );
       
      where = pick( x, y );

      /* 
       * Use current color 
       */
      glColor3f( red, green, blue );

      /* 
       * No draw mode selected 
       */
      if ( where != 0 ) {

         count = 0;
         draw_mode = where;

      /* 
       * Draw line - starting point 
       */
      } else if ( draw_mode == 1  &&  count == 0 ) {
         count = 1;
         xp[0] = x;
         yp[0] = windowHeight - y;

      /* 
       * Draw line - ending point - draw line 
       */
      } else if ( draw_mode == 1  &&  count != 0 ) {

         drawLine( xp[0], yp[0], x, windowHeight - y );

         /* 
          * Turn off draw mode when done 
          */
         draw_mode = 0;
         count = 0;

      /* 
       * Draw rectangle - starting point 
       */
      } else if ( draw_mode == 2  &&  count == 0 ) {
         count = 1;
         xp[0] = x;
         yp[0] = windowHeight - y;

      /* 
       * Draw rectangle - ending point - draw rectangle 
       */
      } else if ( draw_mode == 2  &&  count != 0 ) {

         /* Fill in points at corners of rectangle */
         xp[1] = xp[0]; yp[1] = windowHeight - y;
         xp[2] = x;     yp[2] = windowHeight - y;
         xp[3] = x;     yp[3] = yp[0];

         if ( fill ) {
            drawPolygon( 4, xp, yp );
         } else {
            drawLineLoop( 4, xp, yp );
         }

	 /* 
          * Turn off draw mode when done 
          */
         draw_mode = 0;
         count = 0;

      /* 
       * Draw triangle - starting point 
       */
      } else if ( draw_mode == 3  &&  count == 0 ) {
         count = 1;
         xp[0] = x;
         yp[0] = windowHeight - y;

      /* 
       * Draw triangle - middle point 
       */
      } else if ( draw_mode == 3  &&  count == 1 ) {
         count = 2;
         xp[1] = x;
         yp[1] = windowHeight - y;

      /* 
       * Draw triangle - ending point - draw triangle 
       */
      } else if ( draw_mode == 3  &&  count == 2 ) {
         xp[2] = x;     yp[2] = windowHeight - y;
         if ( fill ) {
	    drawPolygon( 3, xp, yp );
         } else {
	    drawLineLoop( 3, xp, yp );
         }

	 /* Turn off draw mode when done */
         draw_mode = 0;
         count = 0;

      /* 
       * Draw points 
       */
      } else if ( draw_mode == 4 ) {
         y = windowHeight - y;
         setPixel (x, y);
         /*
         glPointSize( 3.0 );
         glBegin( GL_POINTS );
            glVertex2f( x , y );
         glEnd( );
         */

         count++;
      }

      glPopAttrib( );
      glFlush( );
   }

}


/* 
 * Selection of what to draw 
 */

int pick( int x, int y ) {

   y = windowHeight - y;
   if ( y < windowHeight - windowWidth / 10 ) 
      return 0;  /* draw nothing   */
   else if ( x < windowWidth / 10)            
      return 1;  /* draw line      */
   else if ( x < windowWidth / 5)             
      return 2;  /* draw rectangle */
   else if ( x < 3 * windowWidth/ 10)         
      return 3;  /* draw triangle  */
   else if ( x < 2 * windowWidth/ 5)          
      return 4;  /* draw point     */
   else 
      return 0;  /* draw nothing   */

}


/* 
 * Draws boxes representing what can be chosen to draw 
 */

void screen_box( int x, int y, int length ) {

    glBegin( GL_QUADS );
        glVertex2i( x, y );
        glVertex2i( x + length, y );
        glVertex2i( x + length, y + length );
        glVertex2i( x, y + length );
    glEnd( );

}


/* 
 * Handles selection from "right" menu 
 */ 

void right_menu( int id ) {

   if ( id == 1 ) {
      exit(1);  	/* quit */
   } else {
      display( );	/* clear */
   }

}


/* 
 * Handles color menu selection 
 */

void color_menu( int id ) {

   if ( id == 1 ) { 
      red = 1.0; green = 0.0; blue = 0.0; /* red     */
   } else if ( id == 2) { 
      red = 0.0; green = 1.0; blue = 0.0; /* green   */
   } else if ( id == 3) { 
      red = 0.0; green = 0.0; blue = 1.0; /* blue    */
   } else if ( id == 4) { 
      red = 0.0; green = 1.0; blue = 1.0; /* cyan    */
   } else if ( id == 5) { 
      red = 1.0; green = 0.0; blue = 1.0; /* magenta */
   } else if ( id == 6) { 
      red = 1.0; green = 1.0; blue = 0.0; /* yellow  */
   } else if ( id == 7) { 
      red = 1.0; green = 1.0; blue = 1.0; /* white   */
   } else if ( id == 8) { 
      red = 0.0; green = 0.0; blue = 0.0; /* black   */
   }

}


/* 
 * Handles selection from fill menu 
 */

void fill_menu( int id ) {

   if ( id == 1 ) {
       fill = 1; 	/* Turn fill on  */
   } else {
       fill = 0;	/* Turn fill off */
   }

}


/* 
 * Initialize and cleared display 
 */

void display( void ) {

   /* 
    * Save attributes
    */
   glPushAttrib( GL_ALL_ATTRIB_BITS );

   /* 
    * Clear Screen 
    */
   glClearColor ( 0.0, 0.0, 0.0, 1.0 );
   glClear( GL_COLOR_BUFFER_BIT );

   /* 
    * Pick box for drawing lines 
    */
   glColor3f( 1.0, 1.0, 1.0 );
   screen_box( 0, windowHeight - windowWidth / 10, windowWidth / 10 );

   /* 
    * Pick box for drawing rectangles 
    */
   glColor3f( 1.0, 0.0, 0.0 );
   screen_box( windowWidth / 10, windowHeight - windowWidth / 10, 
               windowWidth / 10 );

   /* 
    * Pick box for drawing triangles 
    */
   glColor3f( 0.0, 1.0, 0.0 );
   screen_box( windowWidth / 5, windowHeight - windowWidth / 10, 
               windowWidth / 10 );

   /* 
    * Pick box for drawing points 
    */
   glColor3f( 0.0, 0.0, 1.0 );
   screen_box( 3 * windowWidth / 10, windowHeight - windowWidth / 10, 
               windowWidth / 10 );

   /* 
    * Square on pick box indicating rectangle drawing 
    */
   glColor3f( 0.0, 0.0, 0.0 );
   screen_box( windowWidth / 10 + windowWidth / 40, 
               windowHeight - windowWidth / 10 + windowWidth / 40, 
               windowWidth / 20 );

   /* 
    * Line on pick box indicating line drawing 
    */
   glBegin( GL_LINES );
      glVertex2i( windowHeight / 40, windowHeight - windowWidth / 20 );
      glVertex2i( windowHeight / 40 + windowWidth / 20, 
                  windowHeight - windowWidth / 20 );
   glEnd( );

   /* 
    * Triangle on pick box indicating triangle drawing 
    */
   glBegin( GL_TRIANGLES );
      glVertex2i( windowWidth / 5 + windowWidth / 40, 
                  windowHeight - windowWidth / 10 + windowWidth / 40 );
      glVertex2i( windowWidth / 5 + windowWidth / 20, 
                  windowHeight - windowWidth / 40 );
      glVertex2i( windowWidth / 5 + 3 * windowWidth / 40, 
                  windowHeight - windowWidth / 10 + windowWidth / 40 );
   glEnd( );

   /* 
    * Square on pick box indicating point drawing 
    */
   glPointSize( 3.0 );
   glBegin( GL_POINTS );
      glVertex2i( 3 * windowWidth / 10 + windowWidth / 20, 
                  windowHeight - windowWidth / 20 );
   glEnd( );

   glFlush( );
   glPopAttrib( );

}


/* 
 * Main program - GLUT initialization 
 */

int main( int argc, char** argv ) {

   int c_menu, p_menu, f_menu;

   glutInit( &argc, argv );
   glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
   glutCreateWindow( "Paint" );
   glutDisplayFunc( display );

   /* 
    * Create color submenu 
    */
   c_menu = glutCreateMenu( color_menu );
   glutAddMenuEntry( "Red",     1 );
   glutAddMenuEntry( "Green",   2 );
   glutAddMenuEntry( "Blue",    3 );
   glutAddMenuEntry( "Cyan",    4 );
   glutAddMenuEntry( "Magenta", 5 );
   glutAddMenuEntry( "Yellow",  6 );
   glutAddMenuEntry( "White",   7 );
   glutAddMenuEntry( "Black",   8 );

   /* 
    * Create fill on/off submenu 
    */
   f_menu = glutCreateMenu( fill_menu );
   glutAddMenuEntry( "fill on", 1 );
   glutAddMenuEntry( "fill off", 2 );

   /* 
    * Create right_menu
    */
   glutCreateMenu( right_menu );
   glutAddSubMenu( "Colors", c_menu );
   glutAddSubMenu( "Fill", f_menu );
   glutAddMenuEntry( "clear", 2 );
   glutAddMenuEntry( "quit",  1 );
   glutAttachMenu( GLUT_RIGHT_BUTTON );

   myinit ( );

   glutReshapeFunc ( myReshape ); 
   glutMouseFunc ( mouse );

   glutMainLoop( );

   return 0;

}
