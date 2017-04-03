//
//  BoxCSG.h
//  RayTracer
//
//  Created by David Abensur on 21/03/2017.
//  Copyright © 2017 David Abensur. All rights reserved.
//

#ifndef BoxCSG_h
#define BoxCSG_h
#include "ObjectCSG.h"

#define epsilon 0.001

class BoxCSG : public ObjectCSG
{
public:
    
    BoxCSG(Vector C, Vector v1, Vector v2, Vector Size)
    {
        this->C = C;
        v1.Normalize();
        this->v1 = v1;
        Vector v = (v2-dot(v2,v1)*v1);
        v.Normalize();
        this->v2 = v;
        this->v3 = v1.cross(v2);
        this->size = size;
    }
    
    Vector C;
    Vector v1;
    Vector v2;
    Vector v3;
    Vector size;
    
    bool isInside(const Vector &P) const;
    bool isAlmostInside(const Vector& P, bool localSystem) const;
    void intersect(const Ray & r, std::vector<IntersectpointsCSG *> & IP) const;
    void addIntersectionPoint(double t, Vector& E, Vector& D, std::vector<IntersectpointsCSG *> & IP) const;
};

bool BoxCSG::isInside(const Vector &P) const
{
    Vector deltaP = P-C;
    return std::abs(dot(deltaP,v1)) < size[0]/2 &&  std::abs(dot(deltaP,v2)) < size[1]/2 &&
    std::abs(dot(deltaP,v3)) < size[2]/2;
}

bool BoxCSG::isAlmostInside(const Vector &P, bool localSystem = false) const {
    if (!localSystem) {
        Vector deltaP = P-C;
        return std::abs(dot(deltaP,v1)) < size[0]/2+epsilon &&  std::abs(dot(deltaP,v2)) < size[1]/2+epsilon && std::abs(dot(deltaP,v3)) < size[2]/2+epsilon;
    }
    else {
        return std::abs(P[0]) < size[0]/2+epsilon &&  std::abs(P[1]) < size[1]/2+epsilon && std::abs(P[2]) < size[2]/2+epsilon;
    }
}

void BoxCSG::addIntersectionPoint(double t, Vector &E, Vector &D, std::vector<IntersectpointsCSG *> &IP) const {
    Vector P = D+t*E;
    Vector N;
    
    double min = std::numeric_limits<double>::max();
    double distance = std::abs(size[0] - std::abs(P[0]));
    if (distance < min) {
        min = distance;
        N = (P[0] > 0 ? 1 : -1)*v1;
    }
    distance = std::abs(size[1] - std::abs(P[1]));
    if (distance < min) {
        min = distance;
        N = (P[1] > 0 ? 1 : -1)*v2;
    }
    distance = std::abs(size[2] - std::abs(P[2]));
    if (distance < min) {
        min = distance;
        N = (P[2] > 0 ? 1 : -1)*v3;
    }
    
    Material m = this->material;
    if (this->isAlmostInside(P, true)) {
        Vector realP = P[0]*v1+P[1]*v2+P[2]*v3;
        
        IntersectpointsCSG* IP2 = new IntersectpointsCSG(realP, N, m);
        IP.push_back(IP2);
    }
}

void BoxCSG::intersect(const Ray &r, std::vector<IntersectpointsCSG *> &IP) const {
    
    //On change le repère
    Vector E(dot(r.u,v1), dot(r.u,v2), dot(r.u,v3));
    Vector D(dot((r.C-C),v1), dot((r.C-C),v2), dot((r.C-C),v3));
    
    double halfx = size[0]/2;
    double halfy = size[1]/2;
    double halfz = size[2]/2;
    
    double tmin, tmax, tymin, tymax, tzmin, tzmax;
    bool rsign0 = E[0] < 0;
    bool rsign1 = E[1] < 0;
    bool rsign2 = E[2] < 0;
    Vector bounds[2];
    bounds[0] = Vector(-halfx, -halfy, -halfz);
    bounds[1] = Vector( halfx,  halfy,  halfz);
    
    tmin = (bounds[rsign0][0] - D[0]) / E[0];
    tmax = (bounds[1-rsign0][0] - D[0]) / E[0];
    tymin = (bounds[rsign1][1] - D[1]) / E[1];
    tymax = (bounds[1-rsign1][1] - D[1]) / E[1];
    
    if ((tmin > tymax) || (tymin > tmax))
        return;
    
    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;
    
    tzmin = (bounds[rsign2][2] - D[2]) / E[2];
    tzmax = (bounds[1-rsign2][2] - D[2]) / E[2];
    
    if ((tmin > tzmax) || (tzmin > tmax))
        return;
    
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;
    
    
    addIntersectionPoint(tmin, E, D, IP);
    addIntersectionPoint(tmax, E, D, IP);
}



#endif /* BoxCSG_h */
