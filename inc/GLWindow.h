/**
 * @file GLWindow.h
 * @brief Contains OpenGL implementation for visualization.
 * @author Auralius Manurung <manurung.auralius@gmail.com>
 * @date 27.08.2012
 */

#ifndef GL_WINDOW_H_
#define GL_WINDOW_H_
    
#include "zpr.h"
#include "DeformableObject.h"
#include "RigidSphere.h"

/******************************************************************************
 * Function prototypes
 *****************************************************************************/

/* Restricted */
static void render(void);
static void pick(GLint name);
static void keyboard(GLubyte key, GLint x, GLint y);

/* Accessible */
void glwDeformableObjectToDraw(CDeformableObject * d);
void glwRigidSphereToDraw(CRigidSphere * s);
void glwSetRigidSphereStep(double step);
void glwGraphInit(GLint argc, char *argv[], char *title, GLfloat scale = 1.0);


#endif