#include "GLWindow.h"
#include "Integrator.h"


/******************************************************************************
 * MAIN ROUTINE
 *****************************************************************************/
int main(int argc, char *argv[])
{

    // Simulation parameters
    CDeformableObject deform;
    deform.setStiffness(-50);
    deform.setDamping(-1);
    deform.setMass(1);
    deform.setTimeStep(0.01);

    if (deform.loadObjFile2("bunny.obj") == -1) {
        printf("Error on loading OBJ file\n");
        return -1;
    }

    deform.getNodes(0)->setFixed(true);

    CRigidSphere rigidSphere;
    rigidSphere.setPosition(-80, 0, 0);
    rigidSphere.setDiameter(30);
    rigidSphere.setVirtualStiffness(-1000.0);
    rigidSphere.setVirtualDamping(0);

    CIntegrator integrator;
    integrator.addDeformableObject(&deform);
    integrator.addRigidSphere(&rigidSphere);
    integrator.execSimThread();

    // Prepare graphic thingy
    glwSetRigidSphereStep(1);
    glwDeformableObjectToDraw(&deform);
    glwRigidSphereToDraw(&rigidSphere);    
    glwGraphInit(argc, argv, "Deformable Object");    
    
    return 0;
}




