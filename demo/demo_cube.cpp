#include "GLWindow.h"
#include "Integrator.h"

#include "boost/multi_array.hpp"

/******************************************************************************
 * MAIN ROUTINE
 *****************************************************************************/
int main(int argc, char *argv[])
{
    // Parameters
    const int row = 5;
    const int col = 10;
	const int depth = 1;
    double step = 0.01;

    // Simulation parameters
    CDeformableObject deform;
    deform.setStiffness(-50);
    deform.setDamping(-0.5);
    deform.setMass(1);
    deform.setTimeStep(0.001);

    // Create nodes and arrange their position in the rectangular grid    
    typedef boost::multi_array<CNode, 3> CNode3D;
    CNode3D node(boost::extents[row][col][depth]);

    double leftX = -(col - 1) * step * 0.5;
    double leftY = -(row - 1) * step * 0.5;

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
			for (int k = 0; k < depth; k++) {
				node[i][j][k].setInitialPosition(leftX + step * j, leftY + step * i, step * k);

				// Lowest row is fixed, as if the object is put on top of floor
				if (i == 0)
					node[i][j][k].setFixed(true);
				else
					node[i][j][k].setFixed(false);

				node[i][j][k].setPositionImposed(false);

                // Node as a sphere
                node[i][j][k].setRadius(0.004);

				// Surface node?
				if (i == 0 || i == (row - 1) || 
					j == 0 || j == (col -1) ||
					k == 0 || k == (depth - 1))
					node[i][j][k].setSurfaceNode(true);
				else
					node[i][j][k].setSurfaceNode(false);

                deform.addNode(&node[i][j][k]);
			}
           
    // Deformation thingy
    deform.applyTriangularConnection();    

    CRigidSphere rigidSphere;
    rigidSphere.setPosition(-0.08, 0, 0);
    rigidSphere.setDiameter(0.01);
    rigidSphere.setVirtualStiffness(-500);
    rigidSphere.setVirtualDamping(-0);

    CIntegrator integrator;
    integrator.addDeformableObject(&deform);
    integrator.addRigidSphere(&rigidSphere);
    integrator.execSimThread();

    // Prepare graphic thingy
    glwSetRigidSphereStep(0.001);
    glwDeformableObjectToDraw(&deform);
    glwRigidSphereToDraw(&rigidSphere);    
    glwGraphInit(argc, argv, "Deformable Object", 10.0);    
    
    return 0;
}




