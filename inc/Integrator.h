#ifndef INTEGRATOR_H_
#define INTEGRATOR_H_

#include "boost/thread.hpp"
#include "DeformableObject.h"
#include "RigidSphere.h"


class CIntegrator
{
public:
    CIntegrator(void);
    ~CIntegrator(void);

    void addDeformableObject(CDeformableObject * o);
    void addRigidSphere(CRigidSphere *s);

    void execSimThread();
    

private:
    static void simThread(void *arg);    
    void simThreadWorker();  
    void calculateInteractionForce(int index);

    vector<CDeformableObject *> mDeformableObjects;
    vector<CRigidSphere *> mRigidSpheres;

    bool mRunning;
};

#endif

