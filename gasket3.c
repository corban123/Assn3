#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <vector>
#include "loader.h"
#include <sstream>

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
TrimeshLoader ti;
ifstream in;
static char* objFile;


void drawObject(Trimesh * mesh)
{
    
    
    int i;
    int m;
    int j;
    int k;
    

    glColor3fv(colors[0]); 


        for(i = 0; i < mesh->sizeOfFaces(); i++){
            Face* x = mesh->getFace(i);
            for(j = 0; j < x->vertices->size(); j++){
                glVertex3fv(x->getFacing(j));
                //cout << x->getFacing(j)[(0)] << endl;
            }
            glNormal3f(-x->getNormal()[0], -x->getNormal()[1], -x->getNormal()[2]);

        }
       
}






void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
    glLoadIdentity();
    //v[0] ->name;


    GLfloat x_1 = radius*cos(CONVERT*g_fxRotation)*cos(CONVERT*g_fyRotation);
    GLfloat y_1 = radius*sin(CONVERT*g_fyRotation);
    GLfloat z_1 = radius*sin(CONVERT*g_fxRotation)*cos(CONVERT*g_fyRotation);
    gluLookAt(x_1+x_center, y_1+y_center, z_1+z_center, x_center ,y_center ,z_center, 0 ,1, 0);

    
    int m;

    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);
    // draw line for y axis
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);
    // draw line for Z axis
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 1.0);
    glEnd();
    glEnable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    // draw triangles here 
    // use glNormal3f() to submit a vertex normal with each vertex 
    for(m = 0; m <v.size() ; m++ ){
        glTranslatef(v.at(m)->translate->at(0), v.at(m)->translate->at(1), v.at(m)->translate->at(2));

        glBegin(GL_TRIANGLES);
        drawObject(v.at(m));
        glEnd();
    }

    glFlush();
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
    float x;
    float y;
    float z;
    cout << "Input New Key: ";
    getline(cin,key);
    istringstream putitin(key);
    char firstLetter;
    putitin >> firstLetter;

    if(firstLetter){

    switch (firstLetter)
    {

        case 'L':
            if(key.size() > 2){
                Trimesh* trimesh = new Trimesh(key.substr(2));
                ti.loadOBJ(trimesh->name.c_str(), trimesh);

                v.push_back(trimesh);

            }
            break;

        case 'D':
            if(v.size() > 0){
                v.pop_back();
            }
            break;

        case 'I':
            glLoadIdentity();
            v.back()->setAllZero();
            break;

        case 'T':
            
            putitin >> x;
            putitin>>y;
            putitin>>z;
            if(v.size() > 0){
                v.back()->addTranslate(x,y,z);
            }
            break;

        case 'R':
            float theta;
            putitin >> theta;
            putitin >> x;
            putitin >> y;
            putitin >> z;
            if(v.size() > 0){
                v.back()->addRotate(x,y,z,theta);
            }

        case 'S':
            putitin >> x;
            putitin>>y;
            putitin>>z;
            if(v.size() > 0){
                v.back()->addScale(x,y,z);
            }



        case 'Q':
          glutIdleFunc(NULL);
          break;
    }

  }
  glutPostRedisplay();
}



void Keyboard(unsigned char key, int x, int y) {
  switch (key)
  {
      case 27:
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
      glutKeyboardFunc (Keyboard);

    glutDisplayFunc(display);
    glutMouseFunc (MouseButton);
    glutMotionFunc (MouseMotion);
    // glEnable(GL_LIGHTING);
    // glEnable(GL_LIGHT0);

    
    glEnable(GL_DEPTH_TEST);
    glClearColor (1.0, 1.0, 1.0, 1.0);
    glutMainLoop();
}
