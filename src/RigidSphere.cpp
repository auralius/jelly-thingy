#include "RigidSphere.h"

CRigidSphere::CRigidSphere(void)
{
    mPos.set_size(1,3);
    mPos.zeros();
}

CRigidSphere::~CRigidSphere(void)
{
   
}

void CRigidSphere::setDiameter(double d)
{
    mDiameter = d;
}

void CRigidSphere::setPosition(mat &p)
{
    mPos = p;
}

void CRigidSphere::setPosition(double x, double y, double z)
{
    mPos(0, 0) = x;
    mPos(0, 1) = y;
    mPos(0, 2) = z;
}

void CRigidSphere::setRelativePosition(double dx, double dy, double dz)
{
    mPos(0, 0) = mPos(0, 0) + dx;
    mPos(0, 1) = mPos(0, 1) + dy;
    mPos(0, 2) = mPos(0, 2) + dz;
}

void CRigidSphere::setVirtualStiffness(double k)
{
    mVirtualStiffness = k;
}

void CRigidSphere::setVirtualDamping(double b)
{
    mVirtualDamping = b;
}

 const mat *CRigidSphere::getPosition()
{
    return &mPos;
}

 double CRigidSphere::getVirtualStiffness()
{
    return mVirtualStiffness;
}


 double CRigidSphere::getVirtualDamping()
{
    return mVirtualDamping;
}

 double CRigidSphere::getDiameter()
{
    return mDiameter;
}

void CRigidSphere::render()
{

	glPushMatrix();

    glColor4f(0.0, 1.0, 0.0, 0.75);
    glTranslated(mPos.at(0,0), mPos.at(0,1), mPos.at(0,2));
	glutSolidSphere(mDiameter * 0.5, 10, 10); 
        
	glPopMatrix();

}
