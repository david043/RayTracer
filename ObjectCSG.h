//
//  ObjectCSG.h
//  RayTracer
//
//  Created by David Abensur on 19/03/2017.
//  Copyright © 2017 David Abensur. All rights reserved.
//

#ifndef ObjectCSG_h
#define ObjectCSG_h
#include "Object.h"
#include "Vector.h"
#include "Ray.h"
#include "IntersectpointsCSG.h"


class ObjectCSG : public Object
{
public:
    ObjectCSG(){
        this->material = Material();
    }
    
    virtual bool isInside(const Vector&) const=0;
    virtual void intersect(const Ray&, std::vector<IntersectpointsCSG*>&) const = 0;
    bool intersect(const Ray& r, Vector& P, double& t, Vector& N, Material &M) const;
};

bool ObjectCSG::intersect(const Ray& r, Vector& P, double& t, Vector& N, Material &M) const
{
    std::vector<IntersectpointsCSG* > IP;
    this->intersect(r, IP);
    double minT = 9E5;
    for (unsigned int i = 0; i < IP.size(); ++i) {
        IntersectpointsCSG* IP2 = IP[i];
        double T = dot((IP2->P-r.C),r.u);
        if (T > 0 && T < minT) {
            minT = T;
            P = IP2->P;
            N = IP2->N;
            M = IP2->M;
        }
    }
    t = minT;
    for (unsigned int i = 0; i < IP.size(); ++i) {
        IntersectpointsCSG* IP2 = IP[i];
        delete IP2;
    }
    return minT < 9E5;
}


#endif /* ObjectCSG_h */
