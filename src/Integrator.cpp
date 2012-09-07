#include "Integrator.h"
#include <omp.h>


CIntegrator::CIntegrator(void)
{
    mRunning = true;

    // openMP thingy
    int iCPU = omp_get_num_procs() - 2; // Leave 2 cores for something elses
    omp_set_num_threads(iCPU);
}


CIntegrator::~CIntegrator(void)
{
}

void CIntegrator::addDeformableObject(CDeformableObject *o)
{
    mDeformableObjects.push_back(o);
}

void CIntegrator::addRigidSphere(CRigidSphere *s)
{
     mRigidSpheres.push_back(s);
}

void CIntegrator::execSimThread()
{
    boost::thread t(simThread, (void*) this);    
}

////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////

void CIntegrator::simThread(void *arg)
{     
    CIntegrator *o = (CIntegrator *)arg;
    o->simThreadWorker();
}

void CIntegrator::simThreadWorker()
{
    while (mRunning) {
        for (unsigned int i = 0; i < mDeformableObjects.size(); i++) {
            CDeformableObject *currentDeformableObject = mDeformableObjects.at(i);
            calculateInteractionForce(i);
            currentDeformableObject->updateNodes();
        }

        //Sleep(1);
    }
}

void CIntegrator::calculateInteractionForce(int index)
{    
    mat force(1,3);
    CDeformableObject *currentDeformableObject = mDeformableObjects.at(index);
    
    for (unsigned int i = 0; i < mRigidSpheres.size(); i++) {
        CRigidSphere *currentRigidSphere = mRigidSpheres.at(i);
        double radius = currentRigidSphere->getDiameter() * 0.5;
                
        for (int j = 0; j < currentDeformableObject->getNumberofNodes(); j++) {
            CNode *currentNode = currentDeformableObject->getNodes(j);

            // Compute distance between node and the rigid sphere
            mat lt = *currentRigidSphere->getPosition() - *currentNode->getPosition();
            double ltNorm = norm(lt, 2);            

            // Apply virtual force     
            force.zeros();
            double virtualWallDist = radius + currentNode->getRadius();

            if (ltNorm < virtualWallDist) {                                     
                // Force by spring constant
                double f = currentRigidSphere->getVirtualStiffness() * (virtualWallDist - ltNorm);                
                force = f * (lt / ltNorm);

                // Apply damping
                force = force + *currentNode->getVelocity() * currentRigidSphere->getVirtualDamping();
                
                //force.print("f=");          
                currentNode->setExternalForce(force);                
            }
            else
                force.zeros();

        }
    }

}
