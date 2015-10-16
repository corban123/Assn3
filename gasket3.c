#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <vector>
#include "loader.h"
#include <math.h>

typedef int BOOL;

using namespace std;

#define TRUE 1
#define FALSE 0


GLfloat colors[6][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0},
                        {0.0, 0.0, 1.0}, {1.0, 1.0, 0.0},
                        {1.0, 0.0, 1.0}, {1.0, 1.0, 1.0}};

int n;
static float g_lightPos[4] = { 10, 10, -100, 1 };  // Position of light
static GLfloat PI = atan(1) * 4;
static GLfloat CONVERT = PI/180;
static BOOL g_bButton1Down = FALSE;
static BOOL g_bButton2Down = FALSE;
static BOOL g_bButton3Down = false;
static int g_Width = 600;                          // Initial window width
static int g_Height = 600;                         // Initial window height
static int g_yClick = 0;
static int g_xClick = 0;
static vector<Trimesh*> v;
static float g_fyRotation = 0;
static float g_fxRotation = 0;
static GLfloat g_nearPlane = 1;
static GLfloat x_center = 0 ;
static GLfloat y_center = 0;
static GLfloat z_center = 0;
static GLfloat x_change = 0;
static GLfloat y_change = 0;
static GLfloat g_farPlane = 1000;
static GLfloat radius = 6;
static TrimeshLoader ti;
ifstream in;
static char* objFile;


void drawObject()
{
    
    
    int i;
    int m;
    int j;
    int k;
    for(m = 0; i < v.size(); m++){
        ti.loadOBJ(v[i]->name.c_str(), v[i]);
        glColor3fv(colors[0]); 


        for(i = 0; i < v[i]->sizeOfFaces(); i++){
            Face* x = v[i]->getFace(i);
            for(j = 0; j < x->vertices.size(); j++){
                glVertex3fv(x->getFacing(j));
            }
                
        }
    }
}




void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();



    GLfloat x_1 = radius*cos(CONVERT*g_fxRotation)*cos(CONVERT*g_fyRotation);
    GLfloat y_1 = radius*sin(CONVERT*g_fyRotation);
    GLfloat z_1 = radius*sin(CONVERT*g_fxRotation)*cos(CONVERT*g_fyRotation);
    gluLookAt(x_1+x_center, y_1+y_center, z_1+z_center, x_center ,y_center ,z_center, 0 ,1, 0);

    cout << "X_1 " << " " << x_1 << " y_1 " << " " << y_1 << " z_1 " << " " << z_1;  

    glBegin(GL_TRIANGLES);
    drawObject();
    glEnd();
    glutSwapBuffers();
}

void MouseButton(int button, int state, int x, int y) {
  // Respond to mouse button presses.
  // If button1 pressed, mark this state so we know in motion function.
  if (button == GLUT_LEFT_BUTTON)
    {
        g_bButton1Down = (state == GLUT_DOWN) ? TRUE : FALSE;
        g_yClick = y - 3*g_fyRotation;
        g_xClick = x - 3*g_fxRotation;
    }
    else if (button == GLUT_MIDDLE_BUTTON){
    
        g_bButton2Down = (state==GLUT_DOWN) ? TRUE: FALSE;
        g_yClick = y;
        g_xClick = x;
    }
    else if (button == GLUT_RIGHT_BUTTON){
        g_bButton3Down = (state==GLUT_DOWN) ?TRUE: FALSE;
        g_yClick = y;
    }
}

void MouseMotion(int x, int y) {
  // If button1 pressed, zoom in/out if mouse is moved up/down.
 if (g_bButton1Down)
    {
        g_fxRotation = (x- g_xClick) / 3.0;
        g_fyRotation = (y - g_yClick) / 3.0;
        glutPostRedisplay();

    }
    else if (g_bButton2Down){
        x_change = (x-g_xClick) / 1000.0;
        y_change = (y-g_yClick) / 1000.0;
        z_center -= (x_change*cos(CONVERT*g_fxRotation)) + (y_change *sin(CONVERT*g_fyRotation) * sin(CONVERT*g_fxRotation));
        y_center += y_change*cos(CONVERT*g_fyRotation);
        x_center -= (x_change*sin(CONVERT*g_fxRotation)) + (y_change * sin(CONVERT*g_fyRotation) * cos(CONVERT*g_fxRotation));

        glutPostRedisplay();


    }
    else if(g_bButton3Down){
        
        radius -= (y-g_yClick) / 600.0;
        
        if(radius > 6){
            radius = 6;
        }
        else if (radius < 2){
            radius = 2;
        }
        
        glutPostRedisplay();

    }

}

void Idle(){
    string key;
    getline(cin,key);
    if(key[0]){
    switch (key[0])
    {

        case 'L':
            if(key.size() > 2){
                v.push_back(new Trimesh(key.substr(2)));
            }
            break;

        case 'D':
            if(v.size() > 0){
                v.pop_back();
            }

        case 'Q':
          glutIdleFunc(NULL);
          break;
    }

  }
}



void Keyboard(unsigned char key, int x, int y) {
  switch (key)
  {
      case 'i':
          glutIdleFunc(Idle);
          break;
      }

}

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, (float)w / h, g_nearPlane, g_farPlane);

    glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char **argv)
{
 /* enter number of subdivision steps here */
	if (argc > 1) {
		n=atoi(argv[1]);
	}
	else n = 2;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(300, 300);
    glutCreateWindow("3D Gasket");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutMouseFunc (MouseButton);
    glutMotionFunc (MouseMotion);
    // glEnable(GL_LIGHTING);
    // glEnable(GL_LIGHT0);

    
    glEnable(GL_DEPTH_TEST);
    glClearColor (1.0, 1.0, 1.0, 1.0);
    glutMainLoop();
}
