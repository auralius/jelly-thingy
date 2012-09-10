/**
 * @file Node.h
 * @brief Contains class for a node with its connection to other nodes.
 * @author Auralius Manurung <manurung.auralius@gmail.com>
 * @date 27.08.2012
 */

#ifndef NODE_H_
#define NODE_H_


#define ARMA_DONT_USE_BLAS

#include <vector>
#include "armadillo"

using namespace arma;
using namespace std;

class CNode
{
public:
    CNode();
    ~CNode(void);

    // Set
    void setInitialPosition(double x, double y, double z);
    void setInitialPosition(mat &p);
    void setPosition(double x, double y, double z);
    void setPosition(mat &p);
    void setVelocity(mat &v);
    void setAcceleration(mat &a);
    void setExternalForce(mat &f);
    void setPositionImposed(bool f);
    void setFixed(bool f);
	void setSurfaceNode(bool f);
    void setRadius(double r);

    // Get
    const mat *getInitialPosition();
    const mat *getPosition();
    const mat *getConnectedNodePosition(int node);
    const mat *getVelocity();
    const mat *getAcceleration();
    const mat *getExternalForce();
    void getNaturalLength(int link, mat &naturalLength);
    void getActualLength(int linkmat, mat &actualLength);
    double getNormNaturalLength(int link);
    double getNormActualLength(int link);
    int getNumberofConnectedNodes();
    CNode *getConnectedNodes(int index);
    double getRadius();
    
    // Get (Boolean Status)
    bool fixed();
    bool positionImposed();
	bool surfaceNode();
    
    // Process
    void addConnection(CNode *n);
    void clearAllConnections();


private:
    vector <CNode *> mConnectedNodes;
    mat mInitialPos;
    mat mPos;
    mat mVel;
    mat mAcc;
    mat mExternalForce;
    double mRadius;
    
    bool isPositionImposed;
    bool isFixed;

    bool isSurfaceNode;
};


#endif
