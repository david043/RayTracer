//
//  IntersectpointsCSG.h
//  RayTracer
//
//  Created by David Abensur on 19/03/2017.
//  Copyright © 2017 David Abensur. All rights reserved.
//

#ifndef IntersectpointsCSG_h
#define IntersectpointsCSG_h
#include "Vector.h"
#include "Material.h"

class IntersectpointsCSG
{
public:
    IntersectpointsCSG(Vector& P, Vector& N, Material& M){
        this->P = P;
        this->N = N;
        this->M = M;
    }
    
    Vector P;
    Vector N;
    Material M;
    
};

#endif /* IntersectpointsCSG_h */
