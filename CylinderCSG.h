//
//  CylinderCSG.h
//  RayTracer
//
//  Created by David Abensur on 20/03/2017.
//  Copyright © 2017 David Abensur. All rights reserved.
//

#ifndef CylinderCSG_h
#define CylinderCSG_h
#include "ObjectCSG.h"
#include "IntersectpointsCSG.h"
#include "Vector.h"

class CylinderCSG : public ObjectCSG
{
public:
    CylinderCSG(Vector A, Vector B, double R, Material M)
    {
        this->A = A;
        this->B = B;
        this->R = R;
        this->material = M;
    };
    
    Vector A;
    Vector B;
    double R;
    
    bool isInside(const Vector & P) const {
        Vector u = (B-A);
        u.Normalize();
        return ((P-A-dot((P-A),u)*u).squaredNorm()<(R*R)) && (dot((P-A),u)>0) && ((dot(u,P-B)) < 0);
    };
    
    void intersect(const Ray & r, std::vector<IntersectpointsCSG *> & IP) const;
};

void CylinderCSG::intersect(const Ray & r, std::vector<IntersectpointsCSG *> & IP) const
{
    Vector normale = (B-A);
    normale.Normalize();
    const double ndotr = dot(normale,r.u);
    
    Material m = this->material;
    
    //intersection demi-plan
    
    double tA = dot(normale,A-r.C)/ndotr;
    Vector PA = r.C+tA*r.u;
    if ((PA-A-dot((PA-A),normale)*normale).squaredNorm()<R*R && tA > 0) {
        Vector N = (-1)*normale;
        IP.push_back(new IntersectpointsCSG(PA, N, m));
    }
    
    double tB = dot(normale,B-r.C)/ndotr;
    Vector PB = r.C+tB*r.u;
    if ((PB-A-dot((PB-A),normale)*normale).squaredNorm()<R*R && tB > 0) {
        Vector N = normale;
        IP.push_back(new IntersectpointsCSG(PB, N, m));
    }
    
    // intersection cylindre
    Vector deltaC = r.C-A;
    double a = 1- ndotr*ndotr;
    double dcdotn = dot(normale,deltaC);
    double b = 2*dot((r.u-ndotr*normale),deltaC-dcdotn*normale);
    double c = (deltaC-dcdotn*normale).squaredNorm()-R*R;
    double delta = b*b-4*a*c;
    if (delta > 0) {
        double tc1 = (-b-sqrt(delta))/(2*a);
        Vector Pc1 = r.C+tc1*r.u;
        if (tc1 > 0 && dot((Pc1-A),normale) > 0 && dot((Pc1-B),normale) < 0) {
            Vector N = (Pc1-A-dot((Pc1-A),normale)*normale);
            N.Normalize();
            
            IP.push_back(new IntersectpointsCSG(Pc1,N,m));
        }
        
        double tc2 = (-b+sqrt(delta))/(2*a);
        Vector Pc2 = r.C+tc2*r.u;
        if (tc2 > 0 && dot((Pc2-A),normale) > 0 && dot((Pc2-B),normale) < 0) {
            Vector N = (Pc2-A-dot((Pc2-A),normale)*normale);
            N.Normalize();
            IP.push_back(new IntersectpointsCSG(Pc2,N,m));
        }
    }
    
}



#endif /* CylinderCSG_h */
