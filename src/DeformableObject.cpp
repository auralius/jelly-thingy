#include "DeformableObject.h"


CDeformableObject::CDeformableObject(void)
{
    mUsingObjFile = false;
    mRunning = true;
    mCanDrawNodeAsSphere = true;
}

CDeformableObject::~CDeformableObject(void)
{    
    mRunning = false;
    mNodes.clear();
}

////////////////////////////////////////////////////////////////////////////////
// SET
////////////////////////////////////////////////////////////////////////////////

void CDeformableObject::setStiffness(double k)
{
    mStiffness = k;
}

void CDeformableObject::setDamping(double b)
{    
    mDamping = b;
}

void CDeformableObject::setMass(double m)
{
    mMass = m;
}

void CDeformableObject::setTimeStep(double ts)
{
    mTimeStep = ts;
}

void CDeformableObject::setCanDrawNodeAsSphere(bool f)
{
    mCanDrawNodeAsSphere = f;
}

////////////////////////////////////////////////////////////////////////////////
// GET
////////////////////////////////////////////////////////////////////////////////

int CDeformableObject::getNumberofNodes()
{
    return mNodes.size();
}

double CDeformableObject::getStiffness()
{
    return mStiffness;
}

double CDeformableObject::getDamping()
{
    return mDamping;
}

CNode *CDeformableObject::getNodes(int index)
{
    return mNodes.at(index);
}

bool CDeformableObject::getCanDrawNodeAsSphere()
{
    return mCanDrawNodeAsSphere;
}

void CDeformableObject::addNode(CNode *n)
{
    mNodes.push_back(n);
}

void CDeformableObject::applyTriangularConnection()
{
    // Firts clear upp all exisiting inter-node connections

    #pragma omp parallel for
    for (int i = 0; i < mNodes.size(); i++) {
        CNode *currentNode = mNodes.at(i);

        if (currentNode->getNumberofConnectedNodes() > 0)
            currentNode->clearAllConnections();
    }

    // Delaunay triangulation
    nodeTriangulation();
}

////////////////////////////////////////////////////////////////////////////////
// PROCESS
////////////////////////////////////////////////////////////////////////////////

void CDeformableObject::render()
{
    // Are we rendering an OBJ file ?
    if (mUsingObjFile == true) {
        glPushMatrix();
        glBegin(GL_TRIANGLES);
        for (unsigned int i = 0; i < mFaceIndex.size(); i = i + 3) {
            mat A = *mNodes.at(mFaceIndex.at(i))->getPosition();
            mat B = *mNodes.at(mFaceIndex.at(i + 1))->getPosition();
            mat C = *mNodes.at(mFaceIndex.at(i + 2))->getPosition();

            glColor4f(1.0, 1.0, 0.0, 0.75); // Yellow

            mat normal;
            normal.set_size(1,3);
            calculateFaceNormal(A, B, C, normal);
            //normal.print("normal=");

            glNormal3f(normal.at(0,0), normal.at(0,1), normal.at(0,2));

            //glBegin(GL_TRIANGLES);
            glVertex3f(A.at(0,0), A.at(0,1), A.at(0,2));
            glVertex3f(B.at(0,0), B.at(0,1), B.at(0,2));
            glVertex3f(C.at(0,0), C.at(0,1), C.at(0,2));
            //glEnd(); 
        }
        glEnd();
        glPopMatrix(); // end for
    } // end if

    else {
        // Draw triangulated lines
        glPushMatrix();

        glColor4f(0.0, 0.0, 0.0, 0.75); // Black
       
        for (unsigned int i = 0; i < mNodes.size(); i++) {        
            const mat *A = mNodes.at(i)->getPosition();
        
	        if (mNodes.at(i)->surfaceNode() == true) { 

		        glBegin(GL_LINE_STRIP);   
              
		        for (int j = 0; j < mNodes.at(i)->getNumberofConnectedNodes(); j++) {
			        const mat *B = mNodes.at(i)->getConnectedNodes(j)->getPosition();

			        if (mNodes.at(i)->getConnectedNodes(j)->surfaceNode() == true) {
				        glVertex3f(A->at(0,0), A->at(0,1), A->at(0,2));     
				        glVertex3f(B->at(0,0), B->at(0,1), B->at(0,2)); 
			        } // end if
		        }  //end for

		        glEnd();

            } // end if
        } // end for
        
        glPopMatrix();

        // Draw a small sphere to representate a node
        if (mCanDrawNodeAsSphere == true && mUsingObjFile == false) {
            for (unsigned int i = 0; i < mNodes.size(); i++) {        
                const mat *A = mNodes.at(i)->getPosition();

		        if ( mNodes.at(i)->surfaceNode() == true) {
			        glPushMatrix();

                    glColor4f(1.0, 1.0, 0.0, 100.0); // Yellow

                    glTranslated(A->at(0,0), A->at(0,1), A->at(0,2));
                    glutSolidSphere(mNodes.at(i)->getRadius(), 10, 10); 
        
			        glPopMatrix();
		        } // end if
        
            } // end for
        } // end if
    } // end if - else
   
}

void CDeformableObject::updateNodes()

{    
    // Compute forces at each node   
    for (int i = 0; i < mNodes.size(); i++) {
        CNode *currentNode = mNodes.at(i);

        mat force(1,3);
        force.zeros();

        #pragma omp parallel for
        for (int j = 0; j < currentNode->getNumberofConnectedNodes(); j++) {            
            mat lt;
            mat l0;
            currentNode->getActualLength(j, lt);
            currentNode->getNaturalLength(j, l0);

            double norm_lt = norm(lt, 2);            
            double norm_l0 = norm(l0, 2);                        

            mat forceByReturnSpring = (lt - l0) * mStiffness* 0.5;
            mat dampingForce = mDamping * *currentNode->getVelocity();

            force = force +  ((mStiffness * (norm_lt - norm_l0 * norm_l0 / norm_lt)) * (lt / norm_lt)) + 
                    forceByReturnSpring + dampingForce;
        } // end for 

        // Position, velocity, and acceleration update        
        if (currentNode->positionImposed() == false && currentNode->fixed() == false) {
          
            // Apply external force;
            force = force + *currentNode->getExternalForce();

            // Acceleration
            mat acc = force / mMass;            
            currentNode->setAcceleration(acc);            

            // Velocity
            mat vel = *currentNode->getVelocity() + (*currentNode->getAcceleration() * mTimeStep);            
            currentNode->setVelocity(vel);

            // Position
            mat pos = *currentNode->getPosition() + (*currentNode->getVelocity() *mTimeStep );            
            currentNode->setPosition(pos);
        } // end if     
    } // end for
}

int CDeformableObject::loadObjFile(char *fn)
{    
    ifstream objFile (fn);

    // Handle error on opening the file
    if (objFile.is_open() == false)        
        return - 1;

    string line;
    float *vertexBuffer = NULL;
    long totalConnectedPoints = 0;

    objFile.seekg(0, ios::end);
    size_t  fileSize = objFile.tellg();
    objFile.seekg(0, ios::beg);        

    vertexBuffer = (float*) malloc (fileSize);	
    long index = 0;

    while (! objFile.eof()) {
		getline(objFile,line);
 
		if (line.c_str()[0] == 'v') {
			line[0] = ' ';
 
            // Read floats from the line: v X Y Z
			sscanf(line.c_str(),"%f %f %f ",
				&vertexBuffer[totalConnectedPoints],
				&vertexBuffer[totalConnectedPoints+1], 
				&vertexBuffer[totalConnectedPoints+2]);

            CNode *node = new CNode;
            node[index].setInitialPosition(vertexBuffer[totalConnectedPoints],
				vertexBuffer[totalConnectedPoints+1], 
				vertexBuffer[totalConnectedPoints+2]);

            mNodes.push_back(&node[index]);
        
            totalConnectedPoints += 3;
        } // end if

        if (line.c_str()[0] == 'f') {
		    line[0] = ' ';												// Set first character to 0. This will allow us to use sscanf
 
			int vertexNumber[3] = { 0, 0, 0 };
            sscanf(line.c_str(),"%i%i%i",								// Read integers from the line:  f 1 2 3
				&vertexNumber[0],										// First point of our triangle. This is an 
				&vertexNumber[1],										// pointer to our vertexBuffer list
				&vertexNumber[2] );										// each point represents an X,Y,Z.
 
			vertexNumber[0] -= 1;										// OBJ file starts counting from 1
			vertexNumber[1] -= 1;										// OBJ file starts counting from 1
			vertexNumber[2] -= 1;	

            // Keep face index so we can draw them later, store them in 1D vector sequentially
            mFaceIndex.push_back(vertexNumber[0]);
            mFaceIndex.push_back(vertexNumber[1]);
            mFaceIndex.push_back(vertexNumber[2]);

            // Build connections between 2 nodes based on face data
            mNodes.at(vertexNumber[0])->addConnection(mNodes.at(vertexNumber[1]));
            mNodes.at(vertexNumber[1])->addConnection(mNodes.at(vertexNumber[0]));

            mNodes.at(vertexNumber[1])->addConnection(mNodes.at(vertexNumber[2]));
            mNodes.at(vertexNumber[2])->addConnection(mNodes.at(vertexNumber[1]));

            mNodes.at(vertexNumber[0])->addConnection(mNodes.at(vertexNumber[2]));
            mNodes.at(vertexNumber[2])->addConnection(mNodes.at(vertexNumber[0]));
        }
    } // end while
    

    objFile.close();	
    free(vertexBuffer);

    mUsingObjFile = true;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////

void CDeformableObject::nodeTriangulation()
{
#ifdef _DEBUG
     ofstream oFileT("output.txt",std::ios::out);    
#endif

    // Prepare datasets whice are position of all nodes
    list <Point> L;

    for (unsigned int i = 0; i < mNodes.size(); i++) {
        const mat *A = mNodes.at(i)->getInitialPosition();        
        //A->print("A=");
        L.push_front(Point(A->at(0,0), A->at(0,1), A->at(0,2)));        
    }

    // Triangulation
    Delaunay T(L.begin(), L.end());;
    
    Delaunay::Triangulation_3::Finite_edges_iterator eit = T.finite_edges_begin();
    Delaunay::Triangulation_3::Finite_edges_iterator end = T.finite_edges_end();

    // Process the results
    for(;eit != end; ++eit) {
        Delaunay_edge edge = *eit;
        Point p = edge.first->vertex(edge.second)->point();
        Point q = edge.first->vertex(edge.third)->point();

#ifdef _DEBUG
        oFileT << p << '\n' << q << std::endl;
#endif
        mat p_(1,3);
        mat q_(1,3);
        p_(0,0) = p.x();

        p_(0,1) = p.y();
        p_(0,2) = p.z();

        q_(0,0) = q.x();
        q_(0,1) = q.y();
        q_(0,2) = q.z();

        //p_.print("p_=");
        //q_.print("q_=");

        // Search which node are those two (p and q) and connect those two nodes
        bool pFound = false;
        bool qFound = false;
        CNode *pNode = NULL;
        CNode *qNode = NULL;

        bool done = false;
        #pragma omp parallel 
        {
            for (int i = 0; i < mNodes.size(); i++) {
                const mat *A = mNodes.at(i)->getInitialPosition(); 
                //A->print("A=");

                if (pFound == false)
                    if (norm(*A - p_, 2) < 0.001) {
                        pNode = mNodes.at(i); 
                        pFound = true;
                    }

                if (qFound == false)
                    if (norm(*A - q_, 2) < 0.001) {
                        qNode = mNodes.at(i); 
                        qFound = true;
                    }
            
                #pragma omp flush(done)
                if (pFound == true && qFound == true) {
                    done = true;
                    #pragma omp flush(done)
                    break;
                }
            }
        }

        // Create connection between pNode and qNode
        pNode->addConnection(qNode);
        qNode->addConnection(pNode);

    }
}

void CDeformableObject::calculateFaceNormal(mat &v1, mat &v2, mat &v3, mat &result)
{
    // Calculate vectors between 2 vertices
    mat d1 = v1 - v2;
    mat d2 = v2 - v3;
 
    // Get cross product of vectors
    mat n = cross(d1, d2);
 
    // Normalise final vector
    result = n / norm(n, 2);
}


