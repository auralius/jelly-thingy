#include "Node.h"


CNode::CNode()
{
    // Set dimension
    mInitialPos.set_size(1,3);
    mPos.set_size(1,3);
    mVel.set_size(1,3);
    mAcc.set_size(1,3);
    mExternalForce.set_size(1,3);

    // Initialize
    isPositionImposed = false;
    isFixed = false;
    isSurfaceNode = true;
    mRadius = 0.004;

    mPos.zeros();
    mVel.zeros();
    mAcc.zeros();
    mExternalForce.zeros();
}


CNode::~CNode(void)
{
}

////////////////////////////////////////////////////////////////////////////////
// SET
////////////////////////////////////////////////////////////////////////////////

void CNode::setInitialPosition(double x, double y, double z)
{
    mInitialPos(0, 0) =  x;
    mInitialPos(0, 1) =  y;
    mInitialPos(0, 2) =  z;

    mPos(0, 0) =  x;
    mPos(0, 1) =  y;
    mPos(0, 2) =  z;    
}

void CNode::setPosition(double x, double y, double z)
{
    mPos(0, 0) =  x;
    mPos(0, 1) =  y;
    mPos(0, 2) =  z;
}

void CNode::setPosition(mat &p)
{
    mPos = p;
}

void CNode::setVelocity(mat &v)
{
    mVel = v;
}

void CNode::setAcceleration(mat &a)
{
    mAcc = a;
}

void CNode::setExternalForce(mat &f)
{
    mExternalForce = f;
}

void CNode::setPositionImposed(bool f)
{
    isPositionImposed = f;
}

void CNode::setFixed(bool f)
{
    isFixed = f;
}

void CNode::setSurfaceNode(bool f)
{
	isSurfaceNode = f;
}

void CNode::setRadius(double r)
{
    mRadius = r;
}

////////////////////////////////////////////////////////////////////////////////
// GET
////////////////////////////////////////////////////////////////////////////////

const mat *CNode::getInitialPosition()
{
    return &mInitialPos;
}

const mat *CNode::getPosition()
{
    return &mPos;
}

const mat *CNode::getConnectedNodePosition(int node)
{
    return mConnectedNodes.at(node)->getPosition();
}

const mat *CNode::getVelocity()
{
    return &mVel;
}

const mat *CNode::getAcceleration()
{
    return &mAcc;
}

const mat *CNode::getExternalForce()
{
    return &mExternalForce;
}

void CNode::getNaturalLength(int link, mat &naturalLength)
{
    const mat *B = mConnectedNodes.at(link)->getInitialPosition();
    naturalLength = mInitialPos - *B;    
}

void CNode::getActualLength(int link, mat &actualLength)
{
    const mat *B = mConnectedNodes.at(link)->getPosition();
    actualLength = mPos - *B;    
}

double CNode::getNormNaturalLength(int link)
{
    const mat *B = mConnectedNodes.at(link)->getInitialPosition();
    return norm(mInitialPos - *B, 2);
}

double CNode::getNormActualLength(int link)
{
    const mat *B = mConnectedNodes.at(link)->getPosition();
    return norm(mPos - *B, 2);
}

int CNode::getNumberofConnectedNodes()
{
    return mConnectedNodes.size();
}

CNode *CNode::getConnectedNodes(int index)
{
    return mConnectedNodes.at(index);
}

double CNode::getRadius()
{
    return mRadius;
}

bool CNode::fixed()
{
    return isFixed;
}

bool CNode::positionImposed()
{
    return isPositionImposed;
}

bool CNode::surfaceNode()
{
	return isSurfaceNode;
}

////////////////////////////////////////////////////////////////////////////////
// PROCESS
////////////////////////////////////////////////////////////////////////////////

void CNode::addConnection(CNode *n)
{
    mConnectedNodes.push_back(n);
}

void CNode::clearAllConnections()
{
    mConnectedNodes.clear();
}
