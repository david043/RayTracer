//
//  Object.h
//  RayTracer
//
//  Created by David Abensur on 15/03/2017.
//  Copyright © 2017 David Abensur. All rights reserved.
//

#ifndef Object_h
#define Object_h
#include "Vector.h"
#include "Material.h"
#include "Ray.h"

class Object{
public:
    
    Object(){};
    virtual bool intersect(const Ray&r, Vector& P, double&t, Vector& N, Material &M) const = 0;
    
    Material material;
};

#endif /* Object_h */
