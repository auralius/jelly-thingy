/**
 * @file DeformableObject.h
 * @brief Contains class for a deformable object which is made of several 
 *        connected nodes.
 * @author Auralius Manurung <manurung.auralius@gmail.com>
 * @date 27.08.2012
 */

#ifndef DEFORMABLE_OBJECT_H_
#define DEFORMABLE_OBJECT_H_


#include "Node.h"

// CGAL components for 3D triangulation
#include "CGAL/Exact_predicates_inexact_constructions_kernel.h"
#include "CGAL/Delaunay_triangulation_3.h"
#include "CGAL/Triangulation_vertex_base_with_info_3.h"


#include "freeglut.h"

#ifdef WIN32
#include <windows.h>
#endif

#include <list>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_3<CGAL::Color,K> Vb;
typedef CGAL::Triangulation_data_structure_3<Vb> Tds;
typedef CGAL::Delaunay_triangulation_3<K, Tds> Delaunay;
typedef Delaunay::Edge Delaunay_edge;
typedef Delaunay::Point Point;


class CDeformableObject
{
public:
    CDeformableObject(void);
    ~CDeformableObject(void);

    // Set
    void setStiffness(double k);
    void setDamping(double b);
    void setMass(double m);
    void setTimeStep(double ts);  
    void setCanDrawNodeAsSphere(bool f);
    
    // Get
    int getNumberofNodes();
    double getStiffness();
    double getDamping();
    CNode *getNodes(int index);
    bool getCanDrawNodeAsSphere();

    // Process
    void addNode(CNode *n);
    void applyTriangularConnection();
    void render();
    void updateNodes();
    int loadObjFile(char *fn);
    
private:        
    void nodeTriangulation();
    void calculateFaceNormal(mat &v1, mat &v2, mat &v3, mat &result);

    vector <CNode *> mNodes;

    double mStiffness;
    double mDamping;
    double mMass;
    double mTimeStep;    

    bool mRunning;
    bool mCanDrawNodeAsSphere;
    bool mUsingObjFile;

    vector <int> mFaceIndex;
    
};


#endif

