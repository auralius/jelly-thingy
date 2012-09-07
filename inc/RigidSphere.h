/**
 * @file RigidManipulator.h
 * @brief Contains class for a rigid manipulator that will interact with the
 * specified deformable object.
 * @author Auralius Manurung <manurung.auralius@gmail.com>
 * @date 03.09.2012
 */

#ifndef RIGID_MANIPULATOR_H_
#define RIGID_MANIPULATOR_H_


#define ARMA_DONT_USE_BLAS

#include <vector>
#include "armadillo"
#include "../gl/freeglut.h"

using namespace arma;
using namespace std;


class CRigidSphere   
{
public:
    CRigidSphere();
    ~CRigidSphere();

    // Set

    void setDiameter(double d);
    void setPosition(mat &p);
    void setPosition(double x, double y, double z);
    void setRelativePosition(double dx, double dy, double dz);
    void setVirtualStiffness(double k);
    void setVirtualDamping(double b);   

    // Get

    const mat *getPosition();
    double getVirtualStiffness();
    double getVirtualDamping();
    double getDiameter();

    // Process

    void render();

private:
    mat mPos;
    double mVirtualStiffness;
    double mVirtualDamping;
    double mDiameter;
};

#endif
