//
//  BoundingBox.h
//  RayTracer
//
//  Created by David Abensur on 20/03/2017.
//  Copyright © 2017 David Abensur. All rights reserved.
//

#ifndef BoundingBox_h
#define BoundingBox_h
#include "Ray.h"

class BoundingBox{
    
public:
    Vector bmin,bmax;
    
    BoundingBox(){};
    bool intersect(const Ray& r) const{
        
        double tx1 = (bmin[0] - r.C[0])/r.u[0];
        double tx2 = (bmax[0] - r.C[0])/r.u[0];
        double ty1 = (bmin[1] - r.C[1])/r.u[1];
        double ty2 = (bmax[1] - r.C[1])/r.u[1];
        double tz1 = (bmin[2] - r.C[2])/r.u[2];
        double tz2 = (bmax[2] - r.C[2])/r.u[2];
        
        double tmin = std::min(std::min(std::max(tx1,tx2),std::max(ty1,ty2)),std::max(tz1,tz2));
        double tmax = std::max(std::max(std::min(tx1,tx2),std::min(ty1,ty2)),std::min(tz1,tz2));
        
        return (tmin > tmax);
    }
    
};


#endif /* BoundingBox_h */
