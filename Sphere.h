//
//  Sphere.h
//  RayTracer
//
//  Created by David Abensur on 25/01/17.
//  Copyright © 2017 David Abensur. All rights reserved.
//

#ifndef Sphere_h
#define Sphere_h
#include "Geometry.h"
#include "Ray.h"


class Sphere : public Object{
public:
    
    Vector O;
    double R;
    
    Sphere(Vector O,double R){
        this->O=O;
        this->R=R;
    }
    
    
    Sphere(Vector O,double R, Material material)
    {
        this->O=O;
        this->R=R;
        this->material = material;
    }
    
    
    bool intersect(const Ray& r, Vector& P, double &t, Vector &N,Material &M) const{
        
        double a =1;
        double b = 2.*dot(r.u,r.C-O);
        double c = (r.C-O).squaredNorm() - R*R;
        double delta = b*b-4*a*c;
        
        if(delta>=0){
            
            double t1 = (-b-sqrt(delta))/(2*a);
            double t2 = (-b+sqrt(delta))/(2*a);
            
            
            if(t1>0) {
                
                P = r.C+t1*r.u;
                t=t1;
                N = (P-O);
                N.Normalize();
                M = this->material;
                return true;
            }
            else{
                
                if(t2>0) {
                    P = r.C + t2*r.u;
                    t=t2;
                    N = (P-O);
                    N.Normalize();
                    M = this->material;
                    return true;
                }
                
            }
        }
        
        return false;
        
    }
    
    
};


#endif /* Sphere_h */
