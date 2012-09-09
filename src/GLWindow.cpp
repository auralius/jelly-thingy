#include "GLWindow.h"

/******************************************************************************
 * Data declaration
 *****************************************************************************/
static CDeformableObject * deformableObject;
static CRigidSphere * rigidSphere;
static GLfloat ballSize = 0.004f;
static double stepShift = 0.001;


/****************************************************************************** 
 * Callback function for drawing 
 *****************************************************************************/
static void 
render(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   rigidSphere->render();
   deformableObject->render();

   glutPostRedisplay();

   glutSwapBuffers();
}

/******************************************************************************
 * Callback function for pick-event handling from ZPR 
 *****************************************************************************/
static void 
pick(GLint name)
{
   //printf("Pick: %d\n",name);
   //fflush(stdout);
}

/******************************************************************************
 * Callback function to handle keyboard input 
 *****************************************************************************/
static 
void keyboard(GLubyte key, GLint x, GLint y)
{
switch(key)
	{
	// 'a' -> x minus 
	case 97 :
		rigidSphere->setRelativePosition(-stepShift, 0.0, 0.0);
		break;

	// 's' -> x positive 
	case 115 :
        rigidSphere->setRelativePosition(stepShift, 0.0, 0.0);
		break;

    // 'q' -> z negative 
	case 113 :
        rigidSphere->setRelativePosition(0.0, 0.0, -stepShift);
		break;

    // 'w' -> z positive 
	case 119 :
        rigidSphere->setRelativePosition(0.0, 0.0, stepShift);
		break;

    // 'y' -> y negative 
	case 121 :
        rigidSphere->setRelativePosition(0.0, -stepShift, 0.0);
		break;

    // 'x' -> y positive 
	case 120 :
        rigidSphere->setRelativePosition(0.0, stepShift, 0.0);
		break;

    // 'b' -> toggle filling-sphere view
	case 98 :
        deformableObject->setCanDrawNodeAsSphere(!deformableObject->getCanDrawNodeAsSphere());
		break;
	
    }
}

/****************************************************************************** 
 * Deformable object that willl be drawn
 *****************************************************************************/
void 
glwDeformableObjectToDraw(CDeformableObject * d)
{
    deformableObject = d;
}

/****************************************************************************** 
 * Rigid sphere that willl be drawn
 *****************************************************************************/
void
glwRigidSphereToDraw(CRigidSphere * s)
{
    rigidSphere = s;
}

/****************************************************************************** 
 * Step value when rigid sphere is shifted
 *****************************************************************************/
void 
glwSetRigidSphereStep(double step)
{
    stepShift = step;
}

/****************************************************************************** 
 * Entry point 
 *****************************************************************************/
void 
glwGraphInit(GLint argc, char *argv[], char *title, GLfloat scale)
{
    /* Initialize GLUT and create a window */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow(title);
    glScalef(scale, scale, scale);

    GLfloat diffuseMaterial[4] = {0.5, 0.5, 0.5, 1.0};
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};

    GLfloat light0_position[] = {200.0f, -100.0f, 200.0f, 0.0f };
	GLfloat light1_position[] = {200.0f, -100.0f, -200.0f, 0.0f };

    glClearColor (0.5, 0.5, 0.5, 0.0);
    glShadeModel (GL_SMOOTH);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 25.0);

    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

    glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    /* Configure GLUT callback functions */
    glutDisplayFunc(render);

    /* Configure ZPR module */
    zprInit();
    zprSelectionFunc(render);   /* Selection mode draw function */
    zprPickFunc(pick);          /* Pick event client callback   */  
    glutKeyboardFunc(keyboard); /* Keyboard input callback */

    /* Enter GLUT event loop */
    glutMainLoop();
}


/****************************************************************************** 
 * Scale 
 *****************************************************************************/
void 
glwScale(GLfloat scale)
{
    glScalef(scale, scale, scale);
}