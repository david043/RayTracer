//
//  SphereCSG.h
//  RayTracer
//
//  Created by David Abensur on 19/03/2017.
//  Copyright © 2017 David Abensur. All rights reserved.
//

#ifndef SphereCSG_h
#define SphereCSG_h
#include "ObjectCSG.h"
#include "Vector.h"
#include "Material.h"
#include "IntersectpointsCSG.h"

class SphereCSG : public ObjectCSG
{
public:
    SphereCSG(Vector C, double R, Material M)
    {
        this->C = C;
        this->R = R;
        this->material = M;
    }
    bool isInside(const Vector & P) const {return (P-C).squaredNorm()<(R*R);}
    void intersect(const Ray &r, std::vector<IntersectpointsCSG *> &IP) const;
    
    Vector C;
    double R;
};

void SphereCSG::intersect(const Ray &r, std::vector<IntersectpointsCSG *> &IP) const
{
    Vector P;
    Vector N;
    double a =1;
    double b = 2.*dot(r.u,r.C-C);
    double c = (r.C-C).squaredNorm() - R*R;
    double delta = b*b - 4*a*c;
    if (delta>=0) {
        
        double t1 = (-b-sqrt(delta))/(2*a);
        double t2 = (-b+sqrt(delta))/(2*a);
        
        if (t1 > 0) {
            P = r.C+ t1*r.u;
            N = P-C;
            N.Normalize();
            Material m = Material(this->material);
            IntersectpointsCSG* IP2 = new IntersectpointsCSG(P, N, m);
            IP.push_back(IP2);
        }
        if (t2 > 0) {
            P = r.C+ t2*r.u;
            N = P-C;
            N.Normalize();
            Material m = Material(this->material);
            IntersectpointsCSG* IP2 = new IntersectpointsCSG(P, N, m);
            IP.push_back(IP2);
        }
    }
    
}


#endif /* SphereCSG_h */
